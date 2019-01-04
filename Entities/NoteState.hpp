#ifndef NOTESTATE_HPP
#define NOTESTATE_HPP

#include "Serializable.hpp"

namespace deeper {

class NoteState : public Serializable
{
public:
    static const int InvalidId = -1;

    QJsonObject serializeToJson() const override;
    void deserializeFromJson(const QJsonObject &jsonRaw) override;

    int id() const;

private:
    int m_id = 0;
    QString m_title;
    QString m_color;
    bool m_isFinished = false;
};

} // namespace deeper

#endif // NOTESTATE_HPP
