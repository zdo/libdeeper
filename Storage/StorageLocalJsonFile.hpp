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

    QJsonArray notes(const QString &categoryId) override;
    QJsonObject note(const QString &id) override;
    void saveNote(const QJsonObject &json) override;
    void deleteNote(const QString &id) override;

    QJsonObject activeTimeTrack() override;
    QJsonArray timeTracksForNote(const QString &noteId) override;
    void saveTimeTrack(const QJsonObject &tt) override;
    void deleteTimeTrack(const QString &id) override;

    const QString & path() const;

private:
    QString m_path;
    QJsonObject m_root;

    void saveToFile();
};

} // namespace deeper

#endif // STORAGEJSON_HPP
