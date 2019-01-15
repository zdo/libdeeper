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

    bool isArchived() const;
    void setIsArchived(bool isArchived);

private:
    QString m_title;
    bool m_isArchived = false;
};

} // namespace deeper

#endif // CATEGORY_HPP
