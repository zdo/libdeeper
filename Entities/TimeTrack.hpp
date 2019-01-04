#ifndef TIMETRACK_HPP
#define TIMETRACK_HPP

#include "Serializable.hpp"

#include <QDateTime>

namespace deeper {

struct TimeTrack : public Serializable
{
    QDateTime start, end;

    QJsonObject serializeToJson() const override;
    void deserializeFromJson(const QJsonObject &jsonRaw) override;
};

} // namespace deeper

#endif // TIMETRACK_HPP
