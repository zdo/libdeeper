#include "Category.hpp"

namespace deeper {

QJsonObject Category::serializeToJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["children"] = Serializable::toArrayPtr(m_children);
    json["title"] = m_title;
    json["tags"] = Serializable::toArraySimple(m_tagIdList);
    return json;
}

void Category::deserializeFromJson(const QJsonObject &json)
{
    m_id = json["id"].toString(InvalidId);
    m_children = Serializable::fromArraySharedPointer<Category>(json["children"].toArray());
    m_title = json["title"].toString();
    m_tagIdList = Serializable::fromArrayInt(json["tags"].toArray());
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

QVector<QSharedPointer<Category> > Category::children() const
{
    return m_children;
}

void Category::setChildren(const QVector<QSharedPointer<Category> > &children)
{
    m_children = children;
}

} // namespace deeper
