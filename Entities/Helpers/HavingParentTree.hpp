#ifndef HAVINGPARENTTREE_HPP
#define HAVINGPARENTTREE_HPP

#include "BackendFamilyEntity.hpp"
#include <QSharedPointer>
#include <type_traits>

namespace deeper {

template<typename T>
class HavingParentTree
{
public:
    static_assert (std::is_base_of<BackendFamilyEntity, T>::value, "T must inherit from HavingParent");


    QList<QSharedPointer<T>> rootObjects()
    {
        return this->children(nullptr);
    }

    int count() const
    {
        return m_objectPerId.count();
    }

    QList<QSharedPointer<T>> allObjects()
    {
        return m_objectPerId.values();
    }

    QList<QSharedPointer<T>> children(QSharedPointer<T> parent)
    {
        int parentId = BackendEntity::InvalidId;
        if (!parent.isNull()) {
            parentId = parent->id();
        }

        QList<QSharedPointer<T>> list;
        for (auto o : m_objectPerId) {
            if (o->parentId() == parentId) {
                list.append(o);
            }
        }

        std::sort(list.begin(), list.end(), [](const QSharedPointer<T> &t1, const QSharedPointer<T> &t2) {
            return t1->orderIndex() < t2->orderIndex();
        });

        return list;
    }

    QList<QSharedPointer<T>> allDeepChildren(QSharedPointer<T> parent)
    {
        QList<QSharedPointer<T>> list;
        this->allDeepChildren_r(parent, list);
        return list;
    }

    QSharedPointer<T> objectWithId(const QString &id)
    {
        return m_objectPerId.value(id);
    }

    QSharedPointer<T> parent(const QSharedPointer<T> &o)
    {
        return m_objectPerId.value(o->parentId());
    }

    bool hasChildSomewhereInHierarchy(const QSharedPointer<T> &o, const QSharedPointer<T> &possibleChild)
    {
        auto superParent = possibleChild;
        while (superParent != nullptr) {
            if (superParent->id() == o->id()) {
                return true;
            }

            superParent = this->parent(superParent);
        }

        return false;
    }

protected:
    friend class Database;

    void clearAndFillWith(const QList<QSharedPointer<T>> &objects)
    {
        for (auto o : objects) {
            m_objectPerId[o->id()] = o;
        }
    }


    bool move(const QSharedPointer<T> &o, const QSharedPointer<T> &newParent, int index = -1,
              QList<QSharedPointer<T>> *affectedObjects = nullptr)
    {
        // Reregister in case of updated object.
        m_objectPerId[o->id()] = o;

        if (this->hasChildSomewhereInHierarchy(o, newParent)) {
            // Can't move the objects to its child.
            return false;
        }

        if (affectedObjects) {
            affectedObjects->append(o);
        }

        // Determine old and new parents' ids.
        auto oldParentId = o->parentId();

        auto newParentId = BackendEntity::InvalidId;
        if (!newParent.isNull()) {
            newParentId = newParent->id();
        }

        // Fix indices of old siblings.
        for (const QSharedPointer<T> &t : m_objectPerId) {
            if (t == o) {
                continue;
            }

            if (t->parentId() == oldParentId) {
                if (t->orderIndex() > o->orderIndex()) {
                    t->setOrderIndex(t->orderIndex() - 1);

                    if (affectedObjects) {
                        affectedObjects->append(t);
                    }
                }
            }
        }

        // Determine new order index.
        auto newSiblingsWithoutTheObject = this->children(newParent);
        newSiblingsWithoutTheObject.removeOne(o);
        if (index < 0) {
            index = newSiblingsWithoutTheObject.count();
        }
        index = qBound(0, index, newSiblingsWithoutTheObject.count());
        o->setOrderIndex(index);
        o->setParentId(newParentId);

        // Fix indices of new siblings.
        for (const QSharedPointer<T> &t : newSiblingsWithoutTheObject) {
            if (t->orderIndex() >= index) {
                t->setOrderIndex(t->orderIndex() + 1);

                if (affectedObjects) {
                    affectedObjects->append(t);
                }
            }
        }

        return true;
    }

    void remove(QSharedPointer<T> o, QList<QSharedPointer<T>> *affectedObjects = nullptr)
    {
        if (affectedObjects) {
            affectedObjects->append(o);
        }

        int index = o->orderIndex();
        m_objectPerId.remove(o->id());

        // Fix indices of other siblings.
        for (const QSharedPointer<T> &t : m_objectPerId) {
            bool isSameParent = (t->parentId() == o->parentId());
            if (!isSameParent) {
                continue;
            }

            if (t->orderIndex() > index) {
                t->setOrderIndex(t->orderIndex() - 1);

                if (affectedObjects != nullptr) {
                    affectedObjects->append(t);
                }
            }
        }
    }

private:
    QMap<int, QSharedPointer<T>> m_objectPerId;

    void allDeepChildren_r(const QSharedPointer<T> &parent, QList<QSharedPointer<T>> &list)
    {
        auto children = this->children(parent);
        list.append(children);

        for (auto c : children) {
            this->allDeepChildren_r(c, list);
        }
    }
};

} // namespace deeper

#endif // HAVINGPARENTTREE_HPP
