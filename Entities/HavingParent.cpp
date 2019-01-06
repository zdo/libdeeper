#include "HavingParent.hpp"

namespace deeper {

QString HavingParent::parentId() const
{
    return m_parentId;
}

void HavingParent::setParentId(const QString &parentId)
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
