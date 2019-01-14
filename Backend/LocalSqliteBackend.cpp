#include "LocalSqliteBackend.hpp"
#include <QFile>
#include <cmath>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

#include <QDebug>

namespace deeper {


static QSharedPointer<Category> categoryFromSql(QSqlQuery &q)
{
    auto category = QSharedPointer<Category>::create();
    category->setId(q.value("id").toInt());
    category->setParentId(q.value("parent_id").toInt());
    category->setOrderIndex(q.value("order_index").toInt());
    category->setTitle(q.value("title").toString());
    return category;
}

LocalSqliteBackend::LocalSqliteBackend(const QString &path)
{
    m_path = path;

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    bool ok = m_db.open();
    if (!ok) {
        throw std::runtime_error(QObject::tr("Can't open file at path \"%1\"").arg(m_path).toStdString());
    }

    m_db.exec("BEGIN TRANSACTION");
    m_db.exec("CREATE TABLE `categories` ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `title` TEXT NOT NULL, `parent_id` INTEGER, `order_index` INTEGER )");
    m_db.exec("COMMIT");
}

QList<QSharedPointer<Category>> LocalSqliteBackend::rootCategories()
{
    return this->childrenCategories(nullptr);
}

QList<QSharedPointer<Category> > LocalSqliteBackend::childrenCategories(const QSharedPointer<Category> &parent)
{
    QList<QSharedPointer<Category>> result;

    QSqlQuery q;
    if (parent.isNull()) {
        q = this->prepare("SELECT * FROM categories WHERE parent_id IS NULL");
    } else {
        q = this->prepare("SELECT * FROM categories WHERE parent_id=:id");
        q.bindValue(":id", parent->id());
    }

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

int LocalSqliteBackend::childrenCategoriesCount(const QSharedPointer<Category> &parent)
{
    QSqlQuery qCount;
    if (parent.isNull()) {
        qCount = this->prepare("SELECT COUNT(*) FROM categories WHERE parent_id IS NULL");
    } else {
        qCount = this->prepare("SELECT COUNT(*) FROM categories WHERE parent_id=:id");
        qCount.bindValue(":id", parent->id());
    }
    this->exec(qCount);
    qCount.next();
    int count = qCount.value(0).toInt();
    return count;
}

QSharedPointer<Category> LocalSqliteBackend::createCategory(const QSharedPointer<Category> &parent)
{
    int orderIndex = this->childrenCategoriesCount(parent);

    auto q = this->prepare("INSERT INTO categories (title, parent_id, order_index) VALUES (:title, :parent_id, :order_index)");
    q.bindValue(":title", "New category");
    q.bindValue(":parent_id", parent.isNull() ? QVariant(QVariant::String) : parent->parentId());
    q.bindValue(":order_index", orderIndex);
    this->exec(q);

    int id = q.lastInsertId().toInt();
    auto category = this->category(id);
    return category;
}

QSharedPointer<Category> LocalSqliteBackend::category(int id)
{
    auto category = m_categories.value(id);
    if (category.isNull()) {
        auto q = this->prepare("SELECT * FROM categories WHERE id=:id LIMIT 1");
        q.bindValue(":id", id);
        q.exec();

        if (q.next()) {
            category = categoryFromSql(q);
            m_categories[category->id()] = category;
        }
    }
    return category;
}

void LocalSqliteBackend::saveCategory(const QSharedPointer<Category> &category)
{
    auto q = this->prepare("UPDATE categories SET title=:title, parent_id=:parent_id, order_index=:order_index WHERE id=:id");
    q.bindValue(":id", category->id());
    q.bindValue(":title", category->title());
    q.bindValue(":parent_id", category->parentId() > 0 ? category->parentId() : QVariant(QVariant::String));
    q.bindValue(":order_index", category->orderIndex());
    this->exec(q);
}

void LocalSqliteBackend::deleteCategory(const QSharedPointer<Category> &category)
{
    auto q = this->prepare("DELETE FROM categories WHERE id=:id");
    q.bindValue(":id", category->id());
    this->exec(q);

    m_categories.remove(category->id());
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

} // namespace deeper
