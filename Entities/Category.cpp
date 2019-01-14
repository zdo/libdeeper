#include "Category.hpp"
#include "../Backend/AbstractBackend.hpp"

namespace deeper {

QString Category::title() const
{
    return m_title;
}

void Category::setTitle(const QString &title)
{
    m_title = title;

}

QSharedPointer<Category> Category::parent()
{
    this->getBackendOrThrowError()->category(m_parentId);
}

} // namespace deeper
