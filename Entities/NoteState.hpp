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

    QString title() const;
    void setTitle(const QString &title);

    QString color() const;
    void setColor(const QString &color);

    bool isFinished() const;
    void setIsFinished(bool isFinished);

    int orderIndex() const;
    void setOrderIndex(int orderIndex);

private:
    QString m_title;
    QString m_color;
    int m_orderIndex = 0;
    bool m_isFinished = false;
};

} // namespace deeper

#endif // NOTESTATE_HPP
