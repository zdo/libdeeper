#ifndef STORAGEJSON_HPP
#define STORAGEJSON_HPP

#include "AbstractStorage.hpp"

namespace deeper {

class StorageLocalJsonFile : public AbstractStorage
{
public:
    StorageLocalJsonFile(const QString &path);

    void clearAllData() override;
    StorageBaseInfo getBaseInfo() override;

    void saveCategory(const QJsonObject &json) override;
    void deleteCategory(const QString &id) override;

    QJsonArray notes(const QString &categoryId, const QString &parentNoteId) override;
    void saveNote(const QJsonObject &json) override;
    void deleteNote(const QString &id) override;

private:
    QString m_path;
    QJsonObject m_root;

    void saveToFile();
};

} // namespace deeper

#endif // STORAGEJSON_HPP
