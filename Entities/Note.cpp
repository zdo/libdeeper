#include "Note.hpp"

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

} // namespace deeper
