#ifndef TAG_HPP
#define TAG_HPP

#include <QString>
#include "Serializable.hpp"
#include "HavingId.hpp"

namespace deeper {

class Tag : public Serializable, public HavingId
{
public:
    QJsonObject serializeToJson() const override;
    void deserializeFromJson(const QJsonObject &jsonRaw) override;

    QString title() const;
    void setTitle(const QString &title);

    QString color() const;
    void setColor(const QString &color);

private:
    QString m_title;
    QString m_color;
};

} // namespace deeper

#endif // TAG_HPP
