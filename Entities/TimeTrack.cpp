#include "TimeTrack.hpp"

namespace deeper {

QJsonObject TimeTrack::serializeToJson() const
{
    QJsonObject json;

    Q_ASSERT(this->start.isValid());
    json["start"] = double(this->start.toSecsSinceEpoch());

    if (this->end.isValid()) {
        json["end"] = double(this->end.toSecsSinceEpoch());
    } else {
        json["end"] = 0.0;
    }
    return json;
}

void TimeTrack::deserializeFromJson(const QJsonObject &json)
{
    this->start = QDateTime::fromSecsSinceEpoch(qint64(json["start"].toDouble()));

    double endTime = json["end"].toDouble();
    if (endTime > 0.0) {
        this->end = QDateTime::fromSecsSinceEpoch(qint64(endTime));
    } else {
        this->end = QDateTime();
    }
}

} // namespace deeper
