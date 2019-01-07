#ifndef CATEGORY_HPP
#define CATEGORY_HPP

#include "Serializable.hpp"
#include "HavingParent.hpp"
#include <QList>

namespace deeper {

class Category : public Serializable, public HavingParent
{
public:
    QJsonObject serializeToJson() const override;
    void deserializeFromJson(const QJsonObject &jsonRaw) override;

    QString title() const;
    void setTitle(const QString &title);

    QList<QString> tagIdList() const;
    void setTagIdList(const QList<QString> &tagIdList);

private:
    QList<QString> m_childrenIds;
    QString m_title;
    QList<QString> m_tagIdList;
};

} // namespace deeper

#endif // CATEGORY_HPP
