#ifndef HAVINGPARENT_HPP
#define HAVINGPARENT_HPP

#include "HavingId.hpp"
#include <QMap>

namespace deeper {

class HavingParent : public HavingId
{
public:
    QString parentId() const;
    void setParentId(const QString &parentId);

    int orderIndex() const;
    void setOrderIndex(int orderIndex);

    bool isRoot() const { return m_parentId == InvalidId; }
    bool hasParent() const { return !this->isRoot(); }

    template <typename T>
    static void sortByOrderIndex(QVector<T> &list)
    {
        std::sort(list.begin(), list.end(), [](const T &t1, const T &t2) {
            return t1->orderIndex() < t2->orderIndex();
        });
    }

    template <typename T>
    static QVector<T> children(QMap<QString, T> &objects, T o)
    {
        QVector<T> result;
        for (const T &t : objects) {
            if ((o != nullptr && t->parentId() == o->id())
                    || (o == nullptr && t->parentId() == InvalidId)) {
                result.append(t);
            }
        }
        HavingParent::sortByOrderIndex(result);
        return result;
    }

    template <typename T>
    static void remove(QMap<QString, T> &objects, T o)
    {
        int index = o->orderIndex();
        objects.remove(o->id());

        // Fix indices of other siblings.
        for (T &t : objects) {
            bool isSameParent = (t->parentId() == o->parentId());
            if (!isSameParent) {
                continue;
            }

            if (t->orderIndex() > index) {
                t->setOrderIndex(t->orderIndex() - 1);
            }
        }
    }

    template <typename T>
    static bool insert(QMap<QString, T> &objects, T o, T newParent, int index)
    {
        auto superParent = newParent;
        while (superParent != nullptr) {
            if (superParent->id() == o->id()) {
                // Can't move to its child.
                return false;
            }

            superParent = objects.value(superParent->parentId());
        }

        // Fix indices of old siblings.
        o->setParentId(InvalidId);
        for (T &t : objects) {
            if (t == o) {
                continue;
            }

            bool isSameParent = (t->parentId() == o->parentId());
            if (!isSameParent) {
                continue;
            }

            if (t->orderIndex() > o->orderIndex()) {
                t->setOrderIndex(t->orderIndex() - 1);
            }
        }

        // Fix indices of new siblings.
        auto newSiblings = HavingParent::children(objects, newParent);
        newSiblings.removeOne(o);

        if (index < 0) {
            index = newSiblings.count();
        }
        index = qBound(0, index, newSiblings.count());

        for (T &t : newSiblings) {
            if (t == o) {
                continue;
            }

            if (t->orderIndex() >= index) {
                t->setOrderIndex(t->orderIndex() + 1);
            }
        }

        o->setParentId(newParent != nullptr ? newParent->id() : InvalidId);
        o->setOrderIndex(index);
        return true;
    }

protected:
    QString m_parentId = InvalidId;
    int m_orderIndex = 0;
};

} // namespace deeper

#endif // HAVINGPARENT_HPP
