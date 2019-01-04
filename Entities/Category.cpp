#include "Category.hpp"

namespace deeper {

QJsonObject Category::serializeToJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["parentId"] = m_parentId;
    json["title"] = m_title;
    json["tags"] = Serializable::toArraySimple(m_tagIdList);
    return json;
}

void Category::deserializeFromJson(const QJsonObject &json)
{
    m_id = json["id"].toInt(InvalidId);
    m_parentId = json["parentId"].toInt(InvalidId);
    m_title = json["title"].toString();
    m_tagIdList = Serializable::fromArrayInt(json["tags"].toArray());
}

int Category::id() const
{
    return m_id;
}

int Category::parentId() const
{
    return m_parentId;
}

void Category::setParentId(int parentId)
{
    m_parentId = parentId;
}

QString Category::title() const
{
    return m_title;
}

void Category::setTitle(const QString &title)
{
    m_title = title;
}

QVector<int> Category::tagIdList() const
{
    return m_tagIdList;
}

void Category::setTagIdList(const QVector<int> &tagIdList)
{
    m_tagIdList = tagIdList;
}

} // namespace deeper
