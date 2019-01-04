#include "Database.hpp"
#include "Storage/StorageFactory.hpp"

#include <QFutureWatcher>

namespace deeper {

template <typename T>
static QMap<int, QSharedPointer<T>> fromArrayToSharedPointerMap(const QJsonArray &src) {
    QMap<int, QSharedPointer<T>> result;
    for (const QJsonValue &v : src) {
        QSharedPointer<T> t = QSharedPointer<T>::create();
        t->deserializeFromJson(v.toObject());
        result[t->id()] = t;
    }
    return result;
}

Database::Database(const QUrl &url)
{
    m_storage = StorageFactory::createStorage(url);
    Q_ASSERT(m_storage != nullptr);
}

QVector<QSharedPointer<Category>> Database::rootCategories()
{
    QVector<QSharedPointer<Category>> result;
    for (auto c : m_categories) {
        if (!c->hasParent()) {
            result.append(c);
        }
    }
    return result;
}

QSharedPointer<Category> Database::categoryWithId(int id)
{
    if (m_categories.contains(id)) {
        return m_categories[id];
    } else {
        return nullptr;
    }
}

void Database::saveCategory(const QSharedPointer<Category> &category)
{
    m_storage->saveCategory(category->serializeToJson());
}

void Database::deleteCategory(int id)
{
    m_storage->deleteCategory(id);
    m_categories.remove(id);
}

QVector<QSharedPointer<Tag>> Database::tags()
{
    QVector<QSharedPointer<Tag>> result;
    for (auto t : m_tags) {
        result.append(t);
    }
    return result;
}

void Database::refresh()
{
    auto watcher = new QFutureWatcher<StorageBaseInfo>(this);
    this->connect(watcher, &QFutureWatcher<StorageBaseInfo>::finished, this, [=]() {
        auto baseInfo = watcher->result();

        m_categories = fromArrayToSharedPointerMap<Category>(baseInfo.categories);
        m_tags = fromArrayToSharedPointerMap<Tag>(baseInfo.tags);
        m_noteStates = fromArrayToSharedPointerMap<NoteState>(baseInfo.noteStates);
        m_goals = fromArrayToSharedPointerMap<Goal>(baseInfo.goals);

        watcher->deleteLater();
    });
    watcher->setFuture(m_storage->getBaseInfo());
}

} // namespace deeper
