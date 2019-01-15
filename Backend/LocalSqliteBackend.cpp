#include "LocalSqliteBackend.hpp"
#include <QFile>
#include <cmath>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QSqlDriver>
#include <QDebug>

namespace deeper {

static QSharedPointer<Category> categoryFromSql(QSqlQuery &q)
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
    return category;
}

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

    this->doInTransaction([=]() {
        this->exec("CREATE TABLE categories ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `title` TEXT NOT NULL, `parent_id` INTEGER, `order_index` INTEGER, FOREIGN KEY(`parent_id`) REFERENCES `categories`(`id`) ON DELETE CASCADE )");
    });
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
            category = categoryFromSql(q);
            m_categories[category->id()] = category;
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
            category = categoryFromSql(q);
            category->__setBackend(this);
            m_categories[category->id()] = category;
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
    auto category = m_categories.value(id);
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

void LocalSqliteBackend::exec(const QString &q)
{
    auto query = m_db.exec(q);
    if (query.lastError().isValid()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

void LocalSqliteBackend::bindIdOrNull(QSqlQuery &q, const QString &id, int value)
{
    q.bindValue(id, value != BackendEntity::InvalidId ? value : QVariant(QVariant::String));
}

void LocalSqliteBackend::bindEntityIdOrNull(QSqlQuery &q, const QString &id, BackendEntity *ptr)
{
    q.bindValue(id, ptr == nullptr ? QVariant(QVariant::String) : ptr->id());
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
