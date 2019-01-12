#include "TimeTrack.hpp"

namespace deeper {

QJsonObject TimeTrack::serializeToJson() const
{
    QJsonObject json;

    Q_ASSERT(this->m_start.isValid());
    json["id"] = m_id;
    json["noteId"] = m_noteId;
    json["start"] = double(this->m_start.toSecsSinceEpoch());

    if (this->m_end.isValid()) {
        json["end"] = double(this->m_end.toSecsSinceEpoch());
    } else {
        json["end"] = 0.0;
    }
    return json;
}

void TimeTrack::deserializeFromJson(const QJsonObject &json)
{
    m_id = json["id"].toString();
    m_noteId = json["noteId"].toString();
    m_start = QDateTime::fromSecsSinceEpoch(qint64(json["start"].toDouble()));

    double endTime = json["end"].toDouble();
    if (endTime > 0.0) {
        m_end = QDateTime::fromSecsSinceEpoch(qint64(endTime));
    } else {
        m_end = QDateTime();
    }
}

QDateTime TimeTrack::start() const
{
    return m_start;
}

void TimeTrack::setStart(const QDateTime &start)
{
    m_start = start;
}

QDateTime TimeTrack::end() const
{
    return m_end;
}

void TimeTrack::setEnd(const QDateTime &end)
{
    if (!end.isNull() && (end < m_start)) {
        throw std::runtime_error("TimeTrack: end should be >= start");
    }
    m_end = end;
}

void TimeTrack::setEndToNow()
{
     this->setEnd(QDateTime::currentDateTime());
}

int TimeTrack::durationInSeconds() const
{
    QDateTime end;
    if (m_end.isNull()) {
        end = QDateTime::currentDateTime();
    } else {
        end = m_end;
    }
    return int(m_start.secsTo(end));
}

TimeTrack::Duration TimeTrack::duration() const
{
    Duration d;
    d.seconds = this->durationInSeconds();

    int minutes = d.seconds / 60;
    if (minutes > 0) {
        d.minutes = minutes;
        d.seconds -= d.minutes * 60;

        int hours = d.minutes / 60;
        if (hours > 0) {
            d.hours = hours;
            d.minutes -= d.hours * 60;
        }
    }

    return d;
}

QString TimeTrack::noteId() const
{
    return m_noteId;
}

void TimeTrack::setNoteId(const QString &noteId)
{
    m_noteId = noteId;
}

} // namespace deeper
