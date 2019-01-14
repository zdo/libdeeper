#ifndef GOAL_HPP
#define GOAL_HPP

#include "./Helpers/HavingId.hpp"
#include <QString>

namespace deeper {

class Goal : public HavingId
{
public:
    enum TimePeriod {
        TimePeriod_Day = 1,
        TimePeriod_Week,
        TimePeriod_Month
    };

private:
    QString m_title;

    bool m_isArchived = false;
    int m_noteId = InvalidId;
    TimePeriod m_timePeriod = TimePeriod_Day;

    int m_targetMilestonesCount = 0;
    int m_targetMinutesTracked = 0;
};

} // namespace deeper

#endif // GOAL_HPP
