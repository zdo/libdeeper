#ifndef STORAGEJSON_HPP
#define STORAGEJSON_HPP

#include "AbstractStorage.hpp"

namespace deeper {

class StorageLocalJsonFile : public AbstractStorage
{
public:
    StorageLocalJsonFile(const QString &path);

    QFuture<StorageBaseInfo> getBaseInfo() override;
    QFuture<QVector<QJsonObject>> getNotes(const StorageNotesFilter &filter) override;

    void saveCategoryTree(const QJsonArray &json) override;

private:
    QString m_path;
    QJsonObject m_root;

    void updateOrAppendBasedOnId(const QString &listKey, const QJsonObject &json, const QString &recursiveKey = "");
    bool updateOrAppendBasedOnId_r(QJsonArray &list, const QJsonObject &json, const QString &recursiveKey = "");

    void deleteBasedOnId(const QString &listKey, const QString &id, const QString &recursiveKey = "");
    bool deleteBasedOnId_r(QJsonArray &list, const QString &id, const QString &recursiveKey = "");

    void saveToFile();
};

} // namespace deeper

#endif // STORAGEJSON_HPP
