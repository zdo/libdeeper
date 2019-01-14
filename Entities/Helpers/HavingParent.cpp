#include "HavingParent.hpp"

namespace deeper {

int HavingParent::parentId() const
{
    return m_parentId;
}

void HavingParent::setParentId(const int &parentId)
{
    m_parentId = parentId;
}

int HavingParent::orderIndex() const
{
    return m_orderIndex;
}

void HavingParent::setOrderIndex(int orderIndex)
{
    m_orderIndex = orderIndex;
}

} // namespace deeper
