#ifndef NOTE_HPP
#define NOTE_HPP

#include "Tag.hpp"
#include "NoteState.hpp"
#include "TimeTrack.hpp"
#include "Achievement.hpp"
#include "HavingId.hpp"

#include <QVector>
#include <QDateTime>

namespace deeper {

class Note : public Serializable, public HavingId
{
public:
    QJsonObject serializeToJson() const override;
    void deserializeFromJson(const QJsonObject &jsonRaw) override;

private:
    QDateTime m_creationTime;
    bool m_isArchived = false;
    QString m_categoryId = InvalidId;
    QString m_parentNodeId = InvalidId;

    QString m_title, m_text;
    QString m_stateId = InvalidId;
    QVector<int> m_tagIdList;

    QDateTime m_scheduledTime;
    QDateTime m_deadlineTime;

    QVector<TimeTrack> m_timeTracks;
    QVector<Achievement> m_achievements;
};

} // namespace deeper

#endif // NOTE_HPP
