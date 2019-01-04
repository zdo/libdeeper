#include "StorageLocalJsonFile.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QtConcurrent>

namespace deeper {

StorageLocalJsonFile::StorageLocalJsonFile(const QString &path)
{
    m_path = path;

    QFile file(m_path);
    bool ok = file.open(QFile::ReadOnly);
    if (!ok) {
        throw StorageException(QObject::tr("Can't open file at path \"%1\"").arg(m_path));
    }

    auto jsonData = file.readAll();
    auto jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isNull()) {
        // TODO: Fill with default data.
    } else {
        m_root = jsonDoc.object();
    }
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

QFuture<QVector<QJsonObject> > StorageLocalJsonFile::getNotes(const StorageNotesFilter &filter)
{
}

void StorageLocalJsonFile::saveCategory(const QJsonObject &json)
{
    this->updateOrAppendBasedOnId("categories", json);
    this->saveToFile();
}

void StorageLocalJsonFile::deleteCategory(int id)
{
    this->deleteBasedOnId("categories", id);
    this->saveToFile();
}

void StorageLocalJsonFile::updateOrAppendBasedOnId(const QString &listKey, const QJsonObject &json)
{
    bool found = false;

    QJsonArray list = m_root[listKey].toArray();
    for (auto i = 0; i < list.count(); ++i) {
        bool same = list[i].toObject()["id"].toInt() == json["id"].toInt();
        if (same) {
            found = true;
            list[i] = json;
            break;
        }
    }

    if (!found) {
        list.append(json);
    }
    m_root[listKey] = list;
}

void StorageLocalJsonFile::deleteBasedOnId(const QString &listKey, int id)
{
    QJsonArray list = m_root[listKey].toArray();
    for (auto i = 0; i < list.count(); ++i) {
        bool same = list[i].toObject()["id"].toInt() == id;
        if (same) {
            list.removeAt(i);
            break;
        }
    }
    m_root[listKey] = list;
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
