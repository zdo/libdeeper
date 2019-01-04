#include "Achievement.hpp"

namespace deeper {

QJsonObject Achievement::serializeToJson() const
{
    QJsonObject json;
    Q_ASSERT(this->creationTime.isValid());
    json["creationTime"] = double(this->creationTime.toSecsSinceEpoch());
    return json;
}

void Achievement::deserializeFromJson(const QJsonObject &json)
{
    this->creationTime = QDateTime::fromSecsSinceEpoch(qint64(json.value("creationTime").toDouble()));
}

} // namespace deeper
