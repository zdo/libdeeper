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

// Storage SHOULD NOT use QFuture<> for any operation.
// If the operation will be blocked - it's ok.
// Database class is responsible for an async processing.
class AbstractStorage
{
public:
    virtual ~AbstractStorage();

    virtual StorageBaseInfo getBaseInfo() = 0;
    virtual void clearAllData() = 0;

    virtual void saveCategory(const QJsonObject &json) = 0;
    virtual void deleteCategory(const QString &id) = 0;

    virtual QJsonArray notes(const QString &categoryId) = 0;
    virtual QJsonObject note(const QString &id) = 0;
    virtual void saveNote(const QJsonObject &json) = 0;
    virtual void deleteNote(const QString &id) = 0;

    virtual QJsonObject activeTimeTrack() = 0;
    virtual QJsonArray timeTracksForNote(const QString &noteId) = 0;
    virtual void saveTimeTrack(const QJsonObject &tt) = 0;
    virtual void deleteTimeTrack(const QString &id) = 0;
protected:
    static StorageBaseInfo defaultBaseInfo();
};

} // namespace deeper

#endif // ABSTRACTSTORAGE_HPP
