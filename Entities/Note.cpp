#include "Note.hpp"

namespace deeper {

QJsonObject Note::serializeToJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["title"] = m_title;
    json["text"] = m_text;

    Q_ASSERT(m_creationTime.isValid());
    json["creationTime"] = double(m_creationTime.toSecsSinceEpoch());

    json["isArchived"] = m_isArchived;
    json["stateId"] = m_stateId;
    json["tags"] = Serializable::toArraySimple(m_tagIdList);

    if (m_scheduledTime.isValid()) {
        json["scheduledTime"] = double(m_scheduledTime.toSecsSinceEpoch());
    }
    if (m_deadlineTime.isValid()) {
        json["deadlineTime"] = double(m_deadlineTime.toSecsSinceEpoch());
    }

    json["timeTracks"] = Serializable::toArray(m_timeTracks);
    json["achievements"] = Serializable::toArray(m_achievements);

    return json;
}

void Note::deserializeFromJson(const QJsonObject &json)
{
    m_id = json.value("id").toInt();
    m_title = json["title"].toString();
    m_text = json["text"].toString();
    m_creationTime = QDateTime::fromSecsSinceEpoch(qint64(json["creationTime"].toDouble()));
    m_isArchived = json["isArchived"].toBool();
    m_stateId = json["stateId"].toInt(NoteState::InvalidId);
    m_tagIdList = Serializable::fromArrayInt(json["tags"].toArray());

    double t = json["scheduledTime"].toDouble();
    if (t > 0.0) {
        m_scheduledTime = QDateTime::fromSecsSinceEpoch(qint64(t));
    }

    t = json["deadlineTime"].toDouble();
    if (t > 0.0) {
        m_deadlineTime = QDateTime::fromSecsSinceEpoch(qint64(t));
    }

    m_timeTracks = Serializable::fromArray<TimeTrack>(json["timeTracks"].toArray());
    m_achievements = Serializable::fromArray<Achievement>(json["achievements"].toArray());
}

} // namespace deeper
