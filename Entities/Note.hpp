#ifndef NOTE_HPP
#define NOTE_HPP

#include "Tag.hpp"
#include "NoteState.hpp"
#include "TimeTrack.hpp"
#include "Achievement.hpp"

#include <QVector>
#include <QDateTime>

namespace deeper {

class Note : public Serializable
{
public:
    QJsonObject serializeToJson() const override;
    void deserializeFromJson(const QJsonObject &jsonRaw) override;

private:
    int m_id = 0;
    QDateTime m_creationTime;
    bool m_isArchived = false;

    QString m_title, m_text;
    int m_stateId = NoteState::InvalidId;
    QVector<int> m_tagIdList;

    QDateTime m_scheduledTime;
    QDateTime m_deadlineTime;

    QVector<TimeTrack> m_timeTracks;
    QVector<Achievement> m_achievements;
};

} // namespace deeper

#endif // NOTE_HPP
