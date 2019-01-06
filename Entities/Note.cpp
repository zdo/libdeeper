#include "Note.hpp"

namespace deeper {

QJsonObject Note::serializeToJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["title"] = m_title;
    json["text"] = m_text;

    json["parentId"] = m_parentId;
    json["orderIndex"] = m_orderIndex;

    json["categoryId"] = m_categoryId;

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
    m_id = json.value("id").toString(InvalidId);
    m_title = json["title"].toString();
    m_text = json["text"].toString();

    m_parentId = json.value("parentId").toString(InvalidId);
    m_orderIndex = json["orderIndex"].toInt();

    m_categoryId = json.value("categoryId").toString();

    m_creationTime = QDateTime::fromSecsSinceEpoch(qint64(json["creationTime"].toDouble()));
    m_isArchived = json["isArchived"].toBool();
    m_stateId = json["stateId"].toString(InvalidId);
    m_tagIdList = Serializable::fromArrayString(json["tags"].toArray());

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

QString Note::title() const
{
    return m_title;
}

void Note::setTitle(const QString &title)
{
    m_title = title;
}

QString Note::text() const
{
    return m_text;
}

void Note::setText(const QString &text)
{
    m_text = text;
}

QString Note::categoryId() const
{
    return m_categoryId;
}

void Note::setCategoryId(const QString &categoryId)
{
    m_categoryId = categoryId;
}

QDateTime Note::creationTime() const
{
    return m_creationTime;
}

void Note::setCreationTime(const QDateTime &creationTime)
{
    m_creationTime = creationTime;
}

} // namespace deeper
