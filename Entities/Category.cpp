#include "Category.hpp"
#include "../Backend/AbstractBackend.hpp"

namespace deeper {

QString Category::title() const
{
    return m_title;
}

void Category::setTitle(const QString &title)
{
    m_title = title;
}

QSharedPointer<Category> Category::parent()
{
    return this->getBackendOrError()->categoryWithId(m_parentId);
}

QList<QSharedPointer<Category>> Category::children()
{
    return this->getBackendOrError()->categoryChildren(this->id());
}

void Category::save()
{
    this->getBackendOrError()->saveCategory(this->id());
}

void Category::remove()
{
    this->getBackendOrError()->removeCategory(this->id());
}

void Category::move(const QSharedPointer<Category> &newParent, int index)
{
    this->getBackendOrError()->moveCategory(this->id(), newParent.isNull() ? InvalidId : newParent->id(), index);
}

bool Category::isArchived() const
{
    return m_isArchived;
}

void Category::setIsArchived(bool isArchived)
{
    m_isArchived = isArchived;
}

} // namespace deeper
