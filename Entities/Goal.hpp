#ifndef GOAL_HPP
#define GOAL_HPP

#include "Serializable.hpp"
#include "HavingId.hpp"

namespace deeper {

class Goal : public Serializable, public HavingId
{
public:
    enum TimePeriod {
        TimePeriod_Day = 1,
        TimePeriod_Week,
        TimePeriod_Month
    };

    QJsonObject serializeToJson() const override;
    void deserializeFromJson(const QJsonObject &jsonRaw) override;

private:
    QString m_title;

    bool m_isArchived = false;
    QString m_noteId = InvalidId;
    TimePeriod m_timePeriod = TimePeriod_Day;

    int m_targetMilestonesCount = 0;
    int m_targetMinutesTracked = 0;
};

} // namespace deeper

#endif // GOAL_HPP
