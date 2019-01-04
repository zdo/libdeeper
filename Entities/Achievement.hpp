#ifndef ACHIEVEMENT_HPP
#define ACHIEVEMENT_HPP

#include "Serializable.hpp"
#include <QDateTime>

namespace deeper {

struct Achievement : public Serializable
{
    QDateTime creationTime;

    QJsonObject serializeToJson() const override;
    void deserializeFromJson(const QJsonObject &jsonRaw) override;
};

} // namespace deeper

#endif // ACHIEVEMENT_HPP
