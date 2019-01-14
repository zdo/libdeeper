#include "Milestone.hpp"

namespace deeper {

QDateTime Milestone::creationTime() const
{
    return m_creationTime;
}

void Milestone::setCreationTime(const QDateTime &creationTime)
{
    m_creationTime = creationTime;
}

QString Milestone::comment() const
{
    return m_comment;
}

void Milestone::setComment(const QString &comment)
{
    m_comment = comment;
}

int Milestone::noteId() const
{
    return m_noteId;
}

void Milestone::setNoteId(const int &noteId)
{
    m_noteId = noteId;
}

} // namespace deeper
