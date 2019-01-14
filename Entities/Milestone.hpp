#ifndef MILESTONE_HPP
#define MILESTONE_HPP

#include "./Helpers/HavingId.hpp"
#include <QDateTime>

namespace deeper {

class Milestone : public HavingId
{
public:
    QDateTime creationTime() const;
    void setCreationTime(const QDateTime &creationTime);

    QString comment() const;
    void setComment(const QString &comment);

    int noteId() const;
    void setNoteId(const int &noteId);

private:
    QDateTime m_creationTime;
    int m_noteId;
    QString m_comment;
};

} // namespace deeper

#endif // MILESTONE_HPP
