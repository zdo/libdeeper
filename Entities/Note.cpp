#include "Note.hpp"
#include "../Backend/AbstractBackend.hpp"
#include "Category.hpp"

namespace deeper {

QString Note::title() const
{
    return m_title;
}

void Note::setTitle(const QString &title)
{
    m_title = title;
}

QString Note::text() const
{
    return m_text;
}

void Note::setText(const QString &text)
{
    m_text = text;
}

int Note::categoryId() const
{
    return m_categoryId;
}

void Note::setCategoryId(const int &categoryId)
{
    m_categoryId = categoryId;
}

QDateTime Note::creationTime() const
{
    return m_creationTime;
}

void Note::setCreationTime(const QDateTime &creationTime)
{
    m_creationTime = creationTime;
}

QDateTime Note::deadlineTime() const
{
    return m_deadlineTime;
}

void Note::setDeadlineTime(const QDateTime &deadlineTime)
{
    m_deadlineTime = deadlineTime;
}

Note::DateTimeWithDuration Note::scheduledTime() const
{
    return m_scheduledTime;
}

void Note::setScheduledTime(const DateTimeWithDuration &scheduledTime)
{
    m_scheduledTime = scheduledTime;
}

bool Note::isArchived() const
{
    return m_isArchived;
}

void Note::setIsArchived(bool isArchived)
{
    m_isArchived = isArchived;
}

void Note::save()
{
    this->getBackendOrError()->saveNote(this->id());
}

void Note::remove()
{
    this->getBackendOrError()->removeNote(this->id());
}

void Note::move(const QSharedPointer<Category> &newCategory, const QSharedPointer<Note> &newParentNote, int index)
{
    this->getBackendOrError()->moveNote(this->id(), newCategory->id(),
                                        newParentNote.isNull() ? InvalidId : newParentNote->id(),
                                        index);
}

QSharedPointer<Category> Note::category() const
{
    return this->getBackendOrError()->categoryWithId(m_categoryId);
}

QSharedPointer<Note> Note::parent() const
{
    if (m_parentId == InvalidId) {
        return nullptr;
    }
    return this->getBackendOrError()->noteWithId(m_parentId);
}

QList<QSharedPointer<Note> > Note::children() const
{
    return this->getBackendOrError()->noteChildren(m_categoryId, this->id());
}

QSharedPointer<Note> Note::createChildNote()
{
    return this->getBackendOrError()->createNote(m_categoryId, this->id());
}

} // namespace deeper
