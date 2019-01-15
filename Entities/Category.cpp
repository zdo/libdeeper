#include "Category.hpp"
#include "../Backend/AbstractBackend.hpp"
#include "./Note.hpp"

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
    if (m_parentId == InvalidId) {
        return nullptr;
    }
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

QList<QSharedPointer<Tag> > Category::tags() const
{
    return this->getBackendOrError()->tagsForCategory(this->id());
}

void Category::assignTag(const QSharedPointer<Tag> &tag)
{
    this->getBackendOrError()->assignTagToCategory(this->id(), tag->id());
}

void Category::removeTag(const QSharedPointer<Tag> &tag)
{
    this->getBackendOrError()->removeTagFromCategory(this->id(), tag->id());
}

QList<QSharedPointer<Note> > Category::rootNotes() const
{
    return this->getBackendOrError()->rootNotesForCategory(this->id());
}

} // namespace deeper
