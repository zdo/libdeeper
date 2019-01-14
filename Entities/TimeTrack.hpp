#ifndef TIMETRACK_HPP
#define TIMETRACK_HPP

#include "./Helpers/HavingId.hpp"
#include <QDateTime>

namespace deeper {

class TimeTrack : public HavingId
{
public:
    struct Duration {
        int days = 0;
        int hours = 0;
        int minutes = 0;
        int seconds = 0;

        Duration(int totalSeconds);
        QString toString() const;
    };

    QDateTime start() const;
    void setStart(const QDateTime &start);

    QDateTime end() const;
    void setEnd(const QDateTime &end);
    void setEndToNow();

    int durationInSeconds() const;
    Duration duration() const;

    int noteId() const;
    void setNoteId(const int &noteId);

private:
    QDateTime m_start, m_end;
    int m_noteId;
};

} // namespace deeper

#endif // TIMETRACK_HPP
