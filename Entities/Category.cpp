#include "Category.hpp"

namespace deeper {

QJsonObject Category::serializeToJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["parentId"] = m_parentId;
    json["orderIndex"] = m_orderIndex;
    json["title"] = m_title;
    json["tags"] = Serializable::toArraySimple(m_tagIdList);
    return json;
}

void Category::deserializeFromJson(const QJsonObject &json)
{
    m_id = json["id"].toString(InvalidId);
    m_parentId = json["parentId"].toString(InvalidId);
    m_orderIndex = json["orderIndex"].toInt();
    m_title = json["title"].toString();
    m_tagIdList = Serializable::fromArrayString(json["tags"].toArray());
}

QString Category::title() const
{
    return m_title;
}

void Category::setTitle(const QString &title)
{
    m_title = title;

}

QVector<QString> Category::tagIdList() const
{
    return m_tagIdList;
}

void Category::setTagIdList(const QVector<QString> &tagIdList)
{
    m_tagIdList = tagIdList;
}

} // namespace deeper
