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

void Database::switchAndSaveToStorage(QSharedPointer<AbstractStorage> storage)
{
    m_storage = storage;
    this->saveCategoryTree();
}

QVector<QSharedPointer<Category>> Database::categoriesTree()
{
    return m_categoriesTree;
}

QSharedPointer<Category> Database::categoryWithId(const QString &id)
{
    return m_categoryPerId.value(id);
}

void Database::saveCategoryTree()
{
    m_storage->saveCategoryTree(Serializable::toArrayPtr(m_categoriesTree));
}

QSharedPointer<Category> Database::parentOfCategory(const QSharedPointer<Category> &category)
{
    QSharedPointer<Category> parent;

    std::function<void (QSharedPointer<Category> currentParent, QVector<QSharedPointer<Category>> categories)> fn =
            [&](QSharedPointer<Category> currentParent, QVector<QSharedPointer<Category>> categories) {
        bool found = false;
        for (auto c : categories) {
            if (c->id() == category->id()) {
                parent = currentParent;
                found = true;
                break;
            }
        }

        if (!found) {
            for (auto c : categories) {
                fn(c, c->children());
            }
        }
    };

    fn(nullptr, m_categoriesTree);

    return parent;
}

void Database::deleteCategory(const QSharedPointer<Category> &category)
{
    auto parentCategory = this->parentOfCategory(category);

    if (parentCategory.isNull()) {
        m_categoriesTree.removeOne(category);
    } else {
        auto children = parentCategory->children();
        children.removeOne(category);
        parentCategory->setChildren(children);
    }

    m_categoryPerId.remove(category->id());
    this->saveCategoryTree();
}

QVector<QSharedPointer<Tag>> Database::tags()
{
    QVector<QSharedPointer<Tag>> result;
    for (auto t : m_tags) {
        result.append(t);
    }
    return result;
}

QSharedPointer<Category> Database::createCategory(const QString &parentId)
{
    auto parentCategory = m_categoryPerId.value(parentId);
    if (parentCategory.isNull()) {
        qCritical() << "Try to create category with invalid parent id" << parentId;
        return nullptr;
    }

    auto newCategory = QSharedPointer<Category>::create();
    newCategory->generateRandomId();
    newCategory->setTitle(QObject::tr("New category"));
    m_categoryPerId[newCategory->id()] = newCategory;

    auto children = parentCategory->children();
    children.append(newCategory);
    parentCategory->setChildren(children);

    this->saveCategoryTree();

    return newCategory;
}

void Database::refresh(bool sync)
{
    auto fn = [=](StorageBaseInfo baseInfo) {
        m_categoriesTree = fromArrayToSharedPointers<Category>(baseInfo.categoriesTree);

        m_categoryPerId.clear();
        std::function<void (QVector<QSharedPointer<Category>>)> registerCategoriesFn = [&](QVector<QSharedPointer<Category>> list) {
            for (auto c : list) {
                m_categoryPerId[c->id()] = c;

                registerCategoriesFn(c->children());
            }
        };
        registerCategoriesFn(m_categoriesTree);

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
    auto superParent = parentCategory;
    while (!superParent.isNull()) {
        if (superParent->id() == category->id()) {
            // Can't move category to its child.
            return false;
        }
        superParent = this->parentOfCategory(superParent);
    }

    // Remove from the parent.
    auto oldParentCategory = this->parentOfCategory(category);
    if (oldParentCategory.isNull()) {
        m_categoriesTree.removeOne(category);
    } else {
        auto children = oldParentCategory->children();
        children.removeOne(category);
        oldParentCategory->setChildren(children);
    }

    // Add to new parent.
    if (parentCategory) {
        auto children = parentCategory->children();
        if (index < 0) {
            index = children.size();
        }
        index = qBound(0, index, children.count());
        children.insert(index, category);
        parentCategory->setChildren(children);
    } else {
        if (index < 0) {
            index = m_categoriesTree.size();
        }
        index = qBound(0, index, m_categoriesTree.count());
        m_categoriesTree.insert(index, category);
    }

    this->saveCategoryTree();
    return true;
}

} // namespace deeper
