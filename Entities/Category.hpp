#ifndef CATEGORY_HPP
#define CATEGORY_HPP

#include "Serializable.hpp"
#include <QVector>

namespace deeper {

class Category : public Serializable
{
public:
    static const int InvalidId = -1;

    QJsonObject serializeToJson() const override;
    void deserializeFromJson(const QJsonObject &jsonRaw) override;

    int id() const;

    int parentId() const;
    void setParentId(int parentId);
    bool hasParent() const { return m_parentId != InvalidId; }

    QString title() const;
    void setTitle(const QString &title);

    QVector<int> tagIdList() const;
    void setTagIdList(const QVector<int> &tagIdList);

private:
    int m_id = InvalidId;
    int m_parentId = InvalidId;
    QString m_title;
    QVector<int> m_tagIdList;
};

} // namespace deeper

#endif // CATEGORY_HPP
