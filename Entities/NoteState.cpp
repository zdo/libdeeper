#include "NoteState.hpp"

namespace deeper {

QJsonObject NoteState::serializeToJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["title"] = m_title;
    json["color"] = m_color;
    json["isFinished"] = m_isFinished;
    return json;
}

void NoteState::deserializeFromJson(const QJsonObject &json)
{
    m_id = json.value("id").toInt();
    m_title = json.value("title").toString();
    m_color = json.value("color").toString();
    m_isFinished = json.value("isFinished").toBool();
}

int NoteState::id() const
{
    return m_id;
}

} // namespace deeper
