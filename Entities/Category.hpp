#ifndef CATEGORY_HPP
#define CATEGORY_HPP

#include "./Helpers/HavingParent.hpp"
#include <QString>
#include <QSharedPointer>

namespace deeper {

class Category : public HavingParent
{
public:
    QString title() const;
    void setTitle(const QString &title);

    QSharedPointer<Category> parent();

private:
    QString m_title;
};

} // namespace deeper

#endif // CATEGORY_HPP
