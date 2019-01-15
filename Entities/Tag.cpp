#include "Tag.hpp"
#include "../Backend/AbstractBackend.hpp"

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

void Tag::save()
{
    this->getBackendOrError()->saveTag(this->id());
}

void Tag::remove()
{
    this->getBackendOrError()->removeTag(this->id());
}

} // namespace deeper
