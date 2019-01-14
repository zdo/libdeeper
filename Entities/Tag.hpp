#ifndef TAG_HPP
#define TAG_HPP

#include <QString>
#include "./Helpers/HavingId.hpp"

namespace deeper {

class Tag :  public HavingId
{
public:
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
