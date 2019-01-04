#include "Goal.hpp"

namespace deeper {

QJsonObject Goal::serializeToJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["title"] = m_title;
    json["isArchived"] = m_isArchived;
    json["noteId"] = m_noteId;
    json["timePeriod"] = m_timePeriod;
    json["targetMinutesTracked"] = m_targetMinutesTracked;
    json["targetAchievementsCount"] = m_targetAchievementsCount;
    return json;
}

void Goal::deserializeFromJson(const QJsonObject &json)
{
    m_id = json["id"].toInt();
    m_title = json["title"].toString();
    m_isArchived = json["isArchived"].toBool();
    m_noteId = json["noteId"].toInt();
    m_timePeriod = TimePeriod(json["timePeriod"].toInt());
    m_targetMinutesTracked = json["targetMinutesTracked"].toInt();
    m_targetAchievementsCount = json["targetAchievementsCount"].toInt();
}

int Goal::id() const
{
    return m_id;
}

} // namespace deeper
