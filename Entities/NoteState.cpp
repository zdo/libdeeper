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
    m_id = json.value("id").toString();
    m_title = json.value("title").toString();
    m_color = json.value("color").toString();
    m_isFinished = json.value("isFinished").toBool();
}

QString NoteState::title() const
{
    return m_title;
}

void NoteState::setTitle(const QString &title)
{
    m_title = title;
}

QString NoteState::color() const
{
    return m_color;
}

void NoteState::setColor(const QString &color)
{
    m_color = color;
}

bool NoteState::isFinished() const
{
    return m_isFinished;
}

void NoteState::setIsFinished(bool isFinished)
{
    m_isFinished = isFinished;
}

int NoteState::orderIndex() const
{
    return m_orderIndex;
}

void NoteState::setOrderIndex(int orderIndex)
{
    m_orderIndex = orderIndex;
}

} // namespace deeper
