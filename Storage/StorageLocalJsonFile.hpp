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

    void saveCategory(const QJsonObject &json) override;
    void deleteCategory(int id) override;

private:
    QString m_path;
    QJsonObject m_root;

    void updateOrAppendBasedOnId(const QString &listKey, const QJsonObject &json);
    void deleteBasedOnId(const QString &listKey, int id);

    void saveToFile();
};

} // namespace deeper

#endif // STORAGEJSON_HPP
