#include "Milestone.hpp"

namespace deeper {

QJsonObject Milestone::serializeToJson() const
{
    QJsonObject json;
    Q_ASSERT(m_creationTime.isValid());
    json["id"] = m_id;
    json["noteId"] = m_noteId;
    json["creationTime"] = double(m_creationTime.toSecsSinceEpoch());
    json["comment"] = m_comment;
    return json;
}

void Milestone::deserializeFromJson(const QJsonObject &json)
{
    m_id = json["id"].toString();
    m_noteId = json["noteId"].toString();
    m_comment = json["comment"].toString();
    m_creationTime = QDateTime::fromSecsSinceEpoch(qint64(json.value("creationTime").toDouble()));
}

QDateTime Milestone::creationTime() const
{
    return m_creationTime;
}

void Milestone::setCreationTime(const QDateTime &creationTime)
{
    m_creationTime = creationTime;
}

QString Milestone::comment() const
{
    return m_comment;
}

void Milestone::setComment(const QString &comment)
{
    m_comment = comment;
}

QString Milestone::noteId() const
{
    return m_noteId;
}

void Milestone::setNoteId(const QString &noteId)
{
    m_noteId = noteId;
}

} // namespace deeper
