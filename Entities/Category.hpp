#ifndef CATEGORY_HPP
#define CATEGORY_HPP

#include "Serializable.hpp"
#include "HavingId.hpp"
#include <QVector>

namespace deeper {

class Category : public Serializable, public HavingId
{
public:
    QJsonObject serializeToJson() const override;
    void deserializeFromJson(const QJsonObject &jsonRaw) override;

    QString title() const;
    void setTitle(const QString &title);

    QVector<int> tagIdList() const;
    void setTagIdList(const QVector<int> &tagIdList);

    QVector<QSharedPointer<Category>> children() const;
    void setChildren(const QVector<QSharedPointer<Category>> &children);

private:
    QVector<QSharedPointer<Category>> m_children;

    QString m_title;
    QVector<int> m_tagIdList;
};

} // namespace deeper

#endif // CATEGORY_HPP
