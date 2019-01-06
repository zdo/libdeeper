#include "Database.hpp"
#include "Storage/StorageFactory.hpp"

#include <QFutureWatcher>

namespace deeper {

template <typename T>
static QVector<QSharedPointer<T>> fromArrayToSharedPointers(const QJsonArray &src) {
    QVector<QSharedPointer<T>> result;
    for (const QJsonValue &v : src) {
        QSharedPointer<T> t = QSharedPointer<T>::create();
        t->deserializeFromJson(v.toObject());
        result.append(t);
    }
    return result;
}

Database::Database(const QUrl &url)
{
    m_storage = StorageFactory::createStorage(url);
    Q_ASSERT(m_storage != nullptr);
}

Database::Database(QSharedPointer<AbstractStorage> storage)
{
    m_storage = storage;
    Q_ASSERT(m_storage != nullptr);
}

void Database::switchAndOverwriteStorage(QSharedPointer<AbstractStorage> storage)
{
    m_storage = storage;

    m_storage->clearAllData();

    for (auto c : m_categoryPerId) {
        this->saveCategory(c);
    }
}

QVector<QSharedPointer<Category>> Database::rootCategories()
{
    return HavingParent::children(m_categoryPerId, QSharedPointer<Category>());
}

QSharedPointer<Category> Database::categoryWithId(const QString &id)
{
    return m_categoryPerId.value(id);
}

QSharedPointer<Category> Database::parentOfCategory(const QSharedPointer<Category> &category)
{
    if (category->hasParent()) {
        return this->categoryWithId(category->parentId());
    } else {
        return nullptr;
    }
}

QVector<QSharedPointer<Category> > Database::childrenOfCategory(const QSharedPointer<Category> &category)
{
    return HavingParent::children(m_categoryPerId, category);
}

void Database::deleteCategory(const QSharedPointer<Category> &category)
{
    HavingParent::remove(m_categoryPerId, category);
    m_categoryPerId.remove(category->id());
    m_storage->deleteCategory(category->id());
}

QVector<QSharedPointer<Tag>> Database::tags()
{
    QVector<QSharedPointer<Tag>> result;
    for (auto t : m_tags) {
        result.append(t);
    }
    return result;
}

QSharedPointer<Category> Database::createCategory(const QSharedPointer<Category> &parent)
{
    auto newCategory = QSharedPointer<Category>::create();
    newCategory->generateRandomId();
    newCategory->setTitle(QObject::tr("New category"));

    m_categoryPerId[newCategory->id()] = newCategory;
    HavingParent::insert(m_categoryPerId, newCategory, parent, -1);

    this->saveCategory(newCategory);

    return newCategory;
}

void Database::saveCategory(const QSharedPointer<Category> &category)
{
    m_storage->saveCategory(category->serializeToJson());
}

void Database::refresh(bool sync)
{
    auto fn = [=](StorageBaseInfo baseInfo) {
        auto categories = fromArrayToSharedPointers<Category>(baseInfo.categories);

        m_categoryPerId.clear();
        for (auto c : categories) {
            m_categoryPerId[c->id()] = c;
        }

        m_tags = fromArrayToSharedPointers<Tag>(baseInfo.tags);
        m_noteStates = fromArrayToSharedPointers<NoteState>(baseInfo.noteStates);
        m_goals = fromArrayToSharedPointers<Goal>(baseInfo.goals);

        emit this->onRefresh();
    };

    if (sync) {
        fn(m_storage->getBaseInfo().result());
    } else {
        auto watcher = new QFutureWatcher<StorageBaseInfo>(this);
        this->connect(watcher, &QFutureWatcher<StorageBaseInfo>::finished, this, [=]() {

            watcher->deleteLater();
            fn(watcher->result());
        });
        watcher->setFuture(m_storage->getBaseInfo());
    }
}

bool Database::setCategoryParent(const QSharedPointer<Category> &category,
                                 const QSharedPointer<Category> &parentCategory,
                                 int index)
{
    auto oldParent = this->parentOfCategory(category);
    bool v = HavingParent::insert(m_categoryPerId, category, parentCategory, index);

    this->saveCategory(category);
    for (auto oldSibling : this->childrenOfCategory(oldParent)) {
        this->saveCategory(oldSibling);
    }
    for (auto newSibling : this->childrenOfCategory(parentCategory)) {
        this->saveCategory(newSibling);
    }

    return v;
}

} // namespace deeper
