#ifndef NOTESTATE_HPP
#define NOTESTATE_HPP

#include "Serializable.hpp"
#include "HavingId.hpp"

namespace deeper {

class NoteState : public Serializable, public HavingId
{
public:
    QJsonObject serializeToJson() const override;
    void deserializeFromJson(const QJsonObject &jsonRaw) override;

private:
    QString m_title;
    QString m_color;
    bool m_isFinished = false;
};

} // namespace deeper

#endif // NOTESTATE_HPP
