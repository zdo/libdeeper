#include "TimeTrack.hpp"

namespace deeper {

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
    Duration d(this->durationInSeconds());
    return d;
}

int TimeTrack::noteId() const
{
    return m_noteId;
}

void TimeTrack::setNoteId(const int &noteId)
{
    m_noteId = noteId;
}

TimeTrack::Duration::Duration(int totalSeconds)
{
    this->seconds = totalSeconds;

    int minutesNew = this->seconds / 60;
    if (minutesNew > 0) {
        this->minutes = minutesNew;
        this->seconds -= this->minutes * 60;

        int hoursNew = this->minutes / 60;
        if (hoursNew > 0) {
            this->hours = hoursNew;
            this->minutes -= this->hours * 60;

            int daysNew = this->hours / 24;
            if (daysNew > 0) {
                this->days = daysNew;
                this->hours -= this->days * 24;
            }
        }
    }
}

QString TimeTrack::Duration::toString() const
{
    QString str;
    if (days > 0) {
        str = QString("%1d %2h %3m").arg(days).arg(hours).arg(minutes);
    } else if (hours > 0) {
        str = QString("%1h %2m").arg(hours).arg(minutes);
    } else if (minutes > 0) {
        str = QString("%1m").arg(minutes);
    } else {
        str = QString("%1s").arg(seconds);
    }
    return str;
}

} // namespace deeper
