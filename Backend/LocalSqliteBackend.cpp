#include "LocalSqliteBackend.hpp"
#include <QFile>
#include <cmath>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QSqlDriver>
#include <QDebug>
#include <QDirIterator>

namespace deeper {

static const QVariant DbNull = QVariant(QVariant::String);

LocalSqliteBackend::LocalSqliteBackend(const QString &path)
{
    m_path = path;

    QSqlDatabase::removeDatabase(path);
    m_db = QSqlDatabase::addDatabase("QSQLITE", path);
    m_db.setDatabaseName(path);
    bool ok = m_db.open();
    if (!ok) {
        throw std::runtime_error(QObject::tr("Can't open file at path \"%1\"").arg(m_path).toStdString());
    }

    if (!m_db.driver()->hasFeature(QSqlDriver::NamedPlaceholders))
        throw std::runtime_error("Driver doesn't support NamedPlaceholders feature");
    if (!m_db.driver()->hasFeature(QSqlDriver::LastInsertId))
        throw std::runtime_error("Driver doesn't support LastInsertId feature");
    if (!m_db.driver()->hasFeature(QSqlDriver::Transactions))
        throw std::runtime_error("Driver doesn't support Transactions feature");

    this->exec("PRAGMA foreign_keys = ON");
    this->applyMigrations();
}

LocalSqliteBackend::~LocalSqliteBackend()
{
    m_db.close();
}

QList<QSharedPointer<Category>> LocalSqliteBackend::rootCategories()
{
    return this->categoryChildren(BackendEntity::InvalidId);
}

QList<QSharedPointer<Category> > LocalSqliteBackend::categoryChildren(int parentId)
{
    QList<QSharedPointer<Category>> result;

    QSqlQuery q;
    if (parentId == BackendEntity::InvalidId) {
        q = this->prepare("SELECT * FROM categories WHERE parent_id IS NULL");
    } else {
        q = this->prepare("SELECT * FROM categories WHERE parent_id=:id");
        q.bindValue(":id", parentId);
    }
    this->exec(q);

    while (q.next()) {
        int id = q.value("id").toInt();

        auto category = m_categories.value(id);
        if (category.isNull()) {
            category = this->createCategoryFromSql(q);
        }

        result.append(category);
    }

    return result;
}

int LocalSqliteBackend::categoryChildrenCount(int parentId)
{
    QSqlQuery qCount;
    if (parentId == BackendEntity::InvalidId) {
        qCount = this->prepare("SELECT COUNT(*) FROM categories WHERE parent_id IS NULL");
    } else {
        qCount = this->prepare("SELECT COUNT(*) FROM categories WHERE parent_id=:id");
        qCount.bindValue(":id", parentId);
    }
    this->exec(qCount);
    qCount.next();
    int count = qCount.value(0).toInt();
    return count;
}

QSharedPointer<Category> LocalSqliteBackend::createCategory(int parentId)
{
    int orderIndex = this->categoryChildrenCount(parentId);

    auto q = this->prepare("INSERT INTO categories (title, parent_id, order_index) VALUES (:title, :parent_id, :order_index)");
    q.bindValue(":title", "New category");
    this->bindIdOrNull(q, ":parent_id", parentId);
    q.bindValue(":order_index", orderIndex);
    this->exec(q);

    int id = q.lastInsertId().toInt();
    auto category = this->categoryWithId(id);
    return category;
}

QSharedPointer<Category> LocalSqliteBackend::categoryWithId(int id)
{
    if (id == BackendEntity::InvalidId) {
        return nullptr;
    }

    auto category = m_categories.value(id);
    if (category.isNull()) {
        auto q = this->prepare("SELECT * FROM categories WHERE id=:id LIMIT 1");
        q.bindValue(":id", id);
        q.exec();

        if (q.next()) {
            category = this->createCategoryFromSql(q);
        }
    }
    return category;
}

void LocalSqliteBackend::saveCategory(int id)
{
    auto category = m_categories.value(id);
    if (category.isNull()) {
        throw std::runtime_error("Invalid category ID");
    }

    auto q = this->prepare("UPDATE categories SET title=:title, parent_id=:parent_id, order_index=:order_index WHERE id=:id");
    q.bindValue(":id", category->id());
    q.bindValue(":title", category->title());
    this->bindIdOrNull(q, ":parent_id", category->parentId());
    q.bindValue(":order_index", category->orderIndex());
    this->exec(q);
}

void LocalSqliteBackend::removeCategory(int id)
{
    auto category = this->categoryWithId(id);
    if (category.isNull()) {
        throw std::runtime_error("Invalid category ID");
    }

    this->doInTransaction([=]() {
        QList<int> childrenIdToRemove = this->getAllDeepCategoryChildrenIds(id);

        for (auto sibling : this->categoryChildren(category->parentId())) {
            if (sibling == category) {
                continue;
            }

            if (sibling->orderIndex() > category->orderIndex()) {
                sibling->setOrderIndex(sibling->orderIndex() - 1);
                sibling->save();
            }
        }

        auto q = this->prepare("DELETE FROM categories WHERE id=:id");
        q.bindValue(":id", category->id());
        this->exec(q);

        childrenIdToRemove.append(id);
        for (auto categoryId : childrenIdToRemove) {
            auto categoryToRemove = m_categories.value(categoryId);
            if (!categoryToRemove.isNull()) {
                categoryToRemove->setId(BackendEntity::InvalidId);
                m_categories.remove(categoryId);
            }
        }
    });
}

void LocalSqliteBackend::moveCategory(int id, int newParentId, int index)
{
    auto category = this->categoryWithId(id);
    if (category.isNull()) {
        throw std::runtime_error("Invalid category ID");
    }

    if (newParentId != BackendEntity::InvalidId) {
        auto parentChecked = this->categoryWithId(newParentId);
        while (!parentChecked.isNull()) {
            if (parentChecked == category) {
                // We're trying add parent to its child. That's forbidden.
                return;
            }
            parentChecked = parentChecked->parent();
        }
    }

    this->doInTransaction([=]() {
        // Fix indices of old siblings.
        auto oldParent = category->parent();
        for (auto sibling : oldParent->children()) {
            if (sibling == category) {
                continue;
            }

            if (sibling->orderIndex() > category->orderIndex()) {
                sibling->setOrderIndex(sibling->orderIndex() - 1);
                sibling->save();
            }
        }

        // Validate new orderIndex.
        auto newSiblings = this->categoryChildren(newParentId);

        int newIndex = index;
        if (newIndex < 0) {
            newIndex = newSiblings.count();
        }
        newIndex = qBound(0, newIndex, newSiblings.count());

        category->setOrderIndex(newIndex);
        category->setParentId(newParentId);
        category->save();

        // Fix indices of new siblings.
        for (auto sibling : newSiblings) {
            if (sibling == category) {
                continue;
            }

            if (sibling->orderIndex() >= category->orderIndex()) {
                sibling->setOrderIndex(sibling->orderIndex() + 1);
                sibling->save();
            }
        }
    });
}

QSharedPointer<Tag> LocalSqliteBackend::createTag(const QString &title)
{
    auto q = this->prepare("INSERT INTO tags (title) VALUES (:title)");
    q.bindValue(":title", title);
    this->exec(q);
    return this->tagWithId(q.lastInsertId().toInt());
}

QList<QSharedPointer<Tag>> LocalSqliteBackend::tags()
{
    QList<QSharedPointer<Tag>> result;
    auto q = this->exec("SELECT * FROM tags ORDER BY title");
    while (q.next()) {
        auto tagId = q.value("id").toInt();
        auto tag = m_tags.value(tagId);
        if (tag.isNull()) {
            tag = this->createTagFromSql(q);
        }

        result.append(tag);
    }
    return result;
}

QSharedPointer<Tag> LocalSqliteBackend::tagWithId(int id)
{
    auto tag = m_tags.value(id);
    if (tag.isNull()) {
        auto q = this->prepare("SELECT * FROM tags WHERE id=:id");
        q.bindValue(":id", id);
        this->exec(q);

        if (q.next()) {
            tag = this->createTagFromSql(q);
        }
    }
    return tag;
}

void LocalSqliteBackend::saveTag(int id)
{
    auto tag = m_tags.value(id);
    if (tag.isNull()) {
        throw std::runtime_error("Invalid tag ID");
    }

    auto q = this->prepare("UPDATE tags SET title=:title, color=:color WHERE id=:id");
    q.bindValue(":id", id);
    q.bindValue(":title", tag->title());
    q.bindValue(":color", tag->color().isEmpty() ? DbNull : tag->color());
    this->exec(q);
}

void LocalSqliteBackend::removeTag(int id)
{
    auto tag = this->tagWithId(id);
    if (tag.isNull()) {
        throw std::runtime_error("Invalid tag ID");
    }

    this->doInTransaction([=]() {
        auto q = this->prepare("DELETE FROM tags WHERE id=:id");
        q.bindValue(":id", tag->id());
        this->exec(q);
    });
}

QList<QSharedPointer<Tag>> LocalSqliteBackend::tagsForCategory(int categoryId)
{
    QList<QSharedPointer<Tag>> tags;

    auto q = this->prepare("SELECT tag_id FROM categories_tags WHERE category_id=:category_id");
    q.bindValue(":category_id", categoryId);
    this->exec(q);

    while (q.next()) {
        auto tag = this->tagWithId(q.value("tag_id").toInt());
        tags.append(tag);
    }

    return tags;
}

void LocalSqliteBackend::assignTagToCategory(int categoryId, int tagId)
{
    auto q = this->prepare("SELECT COUNT(*) FROM categories_tags WHERE category_id=:category_id AND tag_id=:tag_id");
    q.bindValue(":category_id", categoryId);
    q.bindValue(":tag_id", tagId);
    this->exec(q);

    q.next();
    bool alreadyExist = (q.value(0).toInt() > 0);
    if (alreadyExist) {
        return;
    }

    q = this->prepare("INSERT INTO categories_tags (category_id, tag_id) VALUES(:category_id, :tag_id)");
    q.bindValue(":category_id", categoryId);
    q.bindValue(":tag_id", tagId);
    this->exec(q);
}

void LocalSqliteBackend::removeTagFromCategory(int categoryId, int tagId)
{
    auto q = this->prepare("DELETE FROM categories_tags WHERE category_id=:category_id AND tag_id=:tag_id");
    q.bindValue(":category_id", categoryId);
    q.bindValue(":tag_id", tagId);
    this->exec(q);
}

QSqlQuery LocalSqliteBackend::prepare(const QString &s)
{
    QSqlQuery q(m_db);
    q.prepare(s);
    return q;
}

void LocalSqliteBackend::exec(QSqlQuery &q)
{
    bool ok = q.exec();
    if (!ok) {
        throw std::runtime_error(q.lastError().text().toStdString());
    }
}

QSqlQuery LocalSqliteBackend::exec(const QString &q)
{
    auto query = m_db.exec(q);
    if (query.lastError().isValid()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
    return query;
}

void LocalSqliteBackend::bindIdOrNull(QSqlQuery &q, const QString &id, int value)
{
    q.bindValue(id, value != BackendEntity::InvalidId ? value : DbNull);
}

void LocalSqliteBackend::bindEntityIdOrNull(QSqlQuery &q, const QString &id, BackendEntity *ptr)
{
    q.bindValue(id, ptr == nullptr ? DbNull : ptr->id());
}

QList<int> LocalSqliteBackend::getAllDeepCategoryChildrenIds(int id)
{
    QList<int> result;

    auto q = this->prepare("SELECT id FROM categories WHERE parent_id=:parent_id");
    q.bindValue(":parent_id", id);
    this->exec(q);

    while (q.next()) {
        int childId = q.value("id").toInt();
        result.append(childId);
        result.append(this->getAllDeepCategoryChildrenIds(childId));
    }

    return result;
}

void LocalSqliteBackend::applyMigrations()
{
    int currentVersion = 0;
    try {
        auto q = this->exec("SELECT value FROM deeper_config WHERE key='version'");
        currentVersion = q.value("value").toString().toInt();
    } catch (...) {
        currentVersion = 0;
    }

    QDirIterator it(":/db/migrations");
    while (it.hasNext()) {
        auto migrationFilePath = it.next();
        auto migrationFileName = QFileInfo(migrationFilePath).fileName();

        auto migratonVersionStr = migrationFileName.split('-').first();
        while (migratonVersionStr.startsWith('0')) {
            migratonVersionStr = migratonVersionStr.mid(1);
        }

        bool ok = false;
        int migrationVersion = migratonVersionStr.toInt(&ok);
        if (!ok) {
            throw std::runtime_error(("Can't parse migration version: " + migrationFileName).toStdString());
        }

        if (currentVersion < migrationVersion) {
            qDebug() << "Apply migration" << migrationVersion;
            this->doInTransaction([=]() {
                this->execSqlFromFile(migrationFilePath);
            });
        } else {
            qDebug() << "Skip migration" << migrationVersion;
        }
    }
}

void LocalSqliteBackend::execSqlFromFile(const QString &path)
{
    QFile file(path);
    bool ok = file.open(QFile::ReadOnly);
    if (!ok) {
        throw std::runtime_error(("Can't open file " + path).toStdString());
    }

    QString content = file.readAll();
    auto queries = content.split(';');

    for (auto query : queries) {
        auto queryFixed = query.trimmed();
        if (queryFixed.length() > 0) {
            this->exec(query);
        }
    }
}

QSharedPointer<Category> LocalSqliteBackend::createCategoryFromSql(QSqlQuery &q)
{
    auto category = QSharedPointer<Category>::create();
    category->setId(q.value("id").toInt());

    bool ok = true;
    category->setParentId(q.value("parent_id").toInt(&ok));
    if (!ok) {
        category->setParentId(BackendEntity::InvalidId);
    }

    category->setOrderIndex(q.value("order_index").toInt());
    category->setTitle(q.value("title").toString());
    category->setIsArchived(q.value("is_archived").toBool());

    category->__setBackend(this);
    m_categories[category->id()] = category;

    return category;
}

QSharedPointer<Tag> LocalSqliteBackend::createTagFromSql(QSqlQuery &q)
{
    auto tag = QSharedPointer<Tag>::create();

    tag->setId(q.value("id").toInt());
    tag->setTitle(q.value("title").toString());
    tag->setColor(q.value("color").toString());

    tag->__setBackend(this);
    m_tags[tag->id()] = tag;

    return tag;
}

void LocalSqliteBackend::doInTransaction(std::function<void ()> fn)
{
    m_db.transaction();

    try {
        fn();
        m_db.commit();
    } catch (...) {
        m_db.rollback();
        throw;
    }
}

} // namespace deeper
