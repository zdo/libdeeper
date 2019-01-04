#ifndef TAG_HPP
#define TAG_HPP

#include <QString>
#include "Serializable.hpp"

namespace deeper {

class Tag : public Serializable
{
public:
    QJsonObject serializeToJson() const override;
    void deserializeFromJson(const QJsonObject &jsonRaw) override;

    int id() const;

private:
    int m_id = 0;
    QString m_title;
    QString m_color;
};

} // namespace deeper

#endif // TAG_HPP
