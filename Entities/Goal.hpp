#ifndef GOAL_HPP
#define GOAL_HPP

#include "Serializable.hpp"

namespace deeper {

class Goal : public Serializable
{
public:
    enum TimePeriod {
        TimePeriod_Day = 1,
        TimePeriod_Week,
        TimePeriod_Month
    };

    QJsonObject serializeToJson() const override;
    void deserializeFromJson(const QJsonObject &jsonRaw) override;

    int id() const;

private:
    int m_id = 0;
    QString m_title;

    bool m_isArchived = false;
    int m_noteId = 0;
    TimePeriod m_timePeriod = TimePeriod_Day;

    int m_targetAchievementsCount = 0;
    int m_targetMinutesTracked = 0;
};

} // namespace deeper

#endif // GOAL_HPP
