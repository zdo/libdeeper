#ifndef CATEGORY_HPP
#define CATEGORY_HPP

#include "./Helpers/BackendFamilyEntity.hpp"
#include <QString>
#include <QSharedPointer>

namespace deeper {

class Category : public BackendFamilyEntity
{
public:
    QString title() const;
    void setTitle(const QString &title);

    QSharedPointer<Category> parent();
    QList<QSharedPointer<Category>> children();

    void save() override;
    void remove();
    void move(const QSharedPointer<Category> &newParent, int index = -1);

private:
    QString m_title;
};

} // namespace deeper

#endif // CATEGORY_HPP
