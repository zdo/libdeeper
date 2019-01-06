#include "StorageLocalJsonFile.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QtConcurrent>

namespace deeper {

StorageLocalJsonFile::StorageLocalJsonFile(const QString &path)
{
    m_path = path;

    bool initWithDefault = true;

    QFile file(m_path);
    if (file.exists()) {
        file.open(QFile::ReadOnly);

        auto jsonData = file.readAll();
        auto jsonDoc = QJsonDocument::fromJson(jsonData);
        m_root = jsonDoc.object();
        initWithDefault = !(m_root.contains("categories"));
    }

    if (initWithDefault) {
        // TODO: Fill with default data.
        auto info = AbstractStorage::defaultBaseInfo();
        m_root["categories"] = info.categories;
        m_root["tags"] = info.tags;
        m_root["goals"] = info.goals;
        m_root["noteStates"] = info.noteStates;

        this->saveToFile();
    }
}

void StorageLocalJsonFile::clearAllData()
{
    m_root["categories"] = QJsonArray();
    m_root["tags"] = QJsonArray();
    m_root["goals"] = QJsonArray();
    m_root["noteStates"] = QJsonArray();
    this->saveToFile();
}

QFuture<StorageBaseInfo> StorageLocalJsonFile::getBaseInfo()
{
    return QtConcurrent::run([=]() {
        StorageBaseInfo info;
        info.categories = m_root["categories"].toArray();
        info.tags = m_root["tags"].toArray();
        info.goals = m_root["goals"].toArray();
        info.noteStates = m_root["noteStates"].toArray();
        return info;
    });
}

QFuture<QJsonArray> StorageLocalJsonFile::getNotes(const QString &categoryId, const StorageNotesFilter &filter)
{

}

void StorageLocalJsonFile::saveCategory(const QJsonObject &json)
{
    bool found = false;

    QJsonArray categoriesJson = m_root["categories"].toArray();
    for (int i = 0; i < categoriesJson.count(); ++i) {
        auto categoryJsonRaw = categoriesJson[i];
        auto categoryJson = categoryJsonRaw.toObject();
        if (categoryJson["id"].toString() == json["id"].toString()) {
            found = true;
            categoriesJson[i] = json;
            break;
        }
    }

    if (!found) {
        categoriesJson.append(json);
    }

    m_root["categories"] = categoriesJson;
    this->saveToFile();
}

void StorageLocalJsonFile::deleteCategory(const QString &id)
{
    QJsonArray categoriesJson = m_root["categories"].toArray();
    for (int i = 0; i < categoriesJson.count(); ++i) {
        auto categoryJsonRaw = categoriesJson[i];
        auto categoryJson = categoryJsonRaw.toObject();
        if (categoryJson["id"].toString() == id) {
            categoriesJson.removeAt(i);
            break;
        }
    }

    m_root["categories"] = categoriesJson;
    this->saveToFile();
}

void StorageLocalJsonFile::saveToFile()
{
    QFile file(m_path);
    bool ok = file.open(QFile::ReadWrite | QFile::Truncate);
    if (!ok) {
        throw StorageException(QObject::tr("Can't open file at path \"%1\"").arg(m_path));
    }

    auto data = QJsonDocument(m_root).toJson();
    auto written = file.write(data);
    if (written != data.size()) {
        throw StorageException(QObject::tr("Can't write to file at path \"%1\"").arg(m_path));
    }
}

} // namespace deeper
