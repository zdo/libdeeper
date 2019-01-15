#include "BackendFamilyEntity.hpp"

namespace deeper {

int BackendFamilyEntity::parentId() const
{
    return m_parentId;
}

void BackendFamilyEntity::setParentId(const int &parentId)
{
    m_parentId = parentId;
}

int BackendFamilyEntity::orderIndex() const
{
    return m_orderIndex;
}

void BackendFamilyEntity::setOrderIndex(int orderIndex)
{
    m_orderIndex = orderIndex;
}

} // namespace deeper
