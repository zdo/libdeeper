#ifndef CATEGORY_HPP
#define CATEGORY_HPP

#include "Serializable.hpp"
#include "HavingParent.hpp"
#include <QVector>

namespace deeper {

class Category : public Serializable, public HavingParent
{
public:
    QJsonObject serializeToJson() const override;
    void deserializeFromJson(const QJsonObject &jsonRaw) override;

    QString title() const;
    void setTitle(const QString &title);

    QVector<QString> tagIdList() const;
    void setTagIdList(const QVector<QString> &tagIdList);

private:
    QVector<QString> m_childrenIds;
    QString m_title;
    QVector<QString> m_tagIdList;
};

} // namespace deeper

#endif // CATEGORY_HPP
