#ifndef STORAGEJSON_HPP
#define STORAGEJSON_HPP

#include "AbstractStorage.hpp"

namespace deeper {

class StorageLocalJsonFile : public AbstractStorage
{
public:
    StorageLocalJsonFile(const QString &path);

    void clearAllData() override;

    QFuture<StorageBaseInfo> getBaseInfo() override;
    QFuture<QJsonArray> getNotes(const QString &categoryId, const StorageNotesFilter &filter) override;

    void saveCategory(const QJsonObject &json) override;
    void deleteCategory(const QString &id) override;

private:
    QString m_path;
    QJsonObject m_root;

    void saveToFile();
};

} // namespace deeper

#endif // STORAGEJSON_HPP
