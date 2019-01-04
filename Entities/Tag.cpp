#include "Tag.hpp"

namespace deeper {

QJsonObject Tag::serializeToJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["title"] = m_title;
    json["color"] = m_color;
    return json;
}

void Tag::deserializeFromJson(const QJsonObject &json)
{
    m_id = json.value("id").toInt();
    m_title = json.value("title").toString();
    m_color = json.value("color").toString();
}

int Tag::id() const
{
    return m_id;
}

} // namespace deeper
