#ifndef ABSTRACTSTORAGE_HPP
#define ABSTRACTSTORAGE_HPP

#include <QFuture>
#include <QJsonObject>
#include <QJsonArray>

#include "StorageException.hpp"

namespace deeper {

struct StorageBaseInfo {
    QJsonArray categories;
    QJsonArray tags;
    QJsonArray noteStates;
    QJsonArray goals;
};

struct StorageNotesFilter {
    QJsonArray tagIds;
};

class AbstractStorage
{
public:
    virtual ~AbstractStorage();

    virtual QFuture<StorageBaseInfo> getBaseInfo() = 0;
    virtual void clearAllData() = 0;

    virtual void saveCategory(const QJsonObject &json) = 0;
    virtual void deleteCategory(const QString &id) = 0;

    virtual QFuture<QJsonArray> getNotes(const QString &categoryId, const StorageNotesFilter &filter) = 0;
protected:
    static StorageBaseInfo defaultBaseInfo();
};

} // namespace deeper

#endif // ABSTRACTSTORAGE_HPP
