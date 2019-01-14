#include "Tag.hpp"

namespace deeper {

QString Tag::title() const
{
    return m_title;
}

void Tag::setTitle(const QString &title)
{
    m_title = title;
}

QString Tag::color() const
{
    return m_color;
}

void Tag::setColor(const QString &color)
{
    m_color = color;
}

} // namespace deeper
