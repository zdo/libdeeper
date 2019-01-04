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
    virtual QFuture<QVector<QJsonObject>> getNotes(const StorageNotesFilter &filter) = 0;

    virtual void saveCategory(const QJsonObject &json) = 0;
    virtual void deleteCategory(int id) = 0;
};

} // namespace deeper

#endif // ABSTRACTSTORAGE_HPP
