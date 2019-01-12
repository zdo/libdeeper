#ifndef MILESTONE_HPP
#define MILESTONE_HPP

#include "Serializable.hpp"
#include "HavingId.hpp"
#include <QDateTime>

namespace deeper {

class Milestone : public Serializable, public HavingId
{
public:
    QJsonObject serializeToJson() const override;
    void deserializeFromJson(const QJsonObject &jsonRaw) override;

    QDateTime creationTime() const;
    void setCreationTime(const QDateTime &creationTime);

    QString comment() const;
    void setComment(const QString &comment);

    QString noteId() const;
    void setNoteId(const QString &noteId);

private:
    QDateTime m_creationTime;
    QString m_noteId;
    QString m_comment;
};

} // namespace deeper

#endif // MILESTONE_HPP
