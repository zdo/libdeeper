#ifndef TIMETRACK_HPP
#define TIMETRACK_HPP

#include "Serializable.hpp"
#include "HavingId.hpp"
#include <QDateTime>

namespace deeper {

class TimeTrack : public Serializable, public HavingId
{
public:
    struct Duration {
        int hours = 0, minutes = 0, seconds = 0;
    };

    QJsonObject serializeToJson() const override;
    void deserializeFromJson(const QJsonObject &jsonRaw) override;

    QDateTime start() const;
    void setStart(const QDateTime &start);

    QDateTime end() const;
    void setEnd(const QDateTime &end);
    void setEndToNow();

    int durationInSeconds() const;
    Duration duration() const;

    QString noteId() const;
    void setNoteId(const QString &noteId);

private:
    QDateTime m_start, m_end;
    QString m_noteId;
};

} // namespace deeper

#endif // TIMETRACK_HPP
