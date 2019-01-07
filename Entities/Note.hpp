#ifndef NOTE_HPP
#define NOTE_HPP

#include "Tag.hpp"
#include "NoteState.hpp"
#include "TimeTrack.hpp"
#include "Achievement.hpp"
#include "HavingParent.hpp"

#include <QList>
#include <QDateTime>

namespace deeper {

class Note : public Serializable, public HavingParent
{
public:
    QJsonObject serializeToJson() const override;
    void deserializeFromJson(const QJsonObject &jsonRaw) override;

    QString title() const;
    void setTitle(const QString &title);

    QString text() const;
    void setText(const QString &text);

    QString categoryId() const;
    void setCategoryId(const QString &categoryId);

    QDateTime creationTime() const;
    void setCreationTime(const QDateTime &creationTime);

private:
    QDateTime m_creationTime;
    bool m_isArchived = false;
    QString m_categoryId = InvalidId;

    QString m_title, m_text;
    QString m_stateId = InvalidId;
    QList<QString> m_tagIdList;

    QDateTime m_scheduledTime;
    QDateTime m_deadlineTime;

    QList<TimeTrack> m_timeTracks;
    QList<Achievement> m_achievements;
};

} // namespace deeper

#endif // NOTE_HPP
