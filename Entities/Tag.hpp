#ifndef TAG_HPP
#define TAG_HPP

#include <QString>
#include "./Helpers/BackendEntity.hpp"

namespace deeper {

class Tag :  public BackendEntity
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
