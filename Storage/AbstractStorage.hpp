#ifndef ABSTRACTSTORAGE_HPP
#define ABSTRACTSTORAGE_HPP

#include <QFuture>
#include <QJsonObject>
#include <QJsonArray>

#include "StorageException.hpp"

namespace deeper {

struct StorageBaseInfo {
    QJsonArray categoriesTree;
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
    virtual QFuture<QVector<QJsonObject>> getNotes(const StorageNotesFilter &filter) = 0;

    virtual void saveCategoryTree(const QJsonArray &json) = 0;

protected:
    static StorageBaseInfo defaultBaseInfo();
};

} // namespace deeper

#endif // ABSTRACTSTORAGE_HPP
