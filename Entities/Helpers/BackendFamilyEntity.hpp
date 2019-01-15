#ifndef HAVINGPARENT_HPP
#define HAVINGPARENT_HPP

#include "BackendEntity.hpp"
#include <QMap>

namespace deeper {

class BackendFamilyEntity : public BackendEntity
{
public:
    int parentId() const;
    void setParentId(const int &parentId);

    int orderIndex() const;
    void setOrderIndex(int orderIndex);

    bool isRoot() const { return m_parentId == InvalidId; }
    bool hasParent() const { return !this->isRoot(); }

protected:
    int m_parentId = InvalidId;
    int m_orderIndex = 0;
};

} // namespace deeper

#endif // HAVINGPARENT_HPP
