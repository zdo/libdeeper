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
        initWithDefault = !(m_root.contains("categoriesTree"));
    }

    if (initWithDefault) {
        // TODO: Fill with default data.
        auto info = AbstractStorage::defaultBaseInfo();
        m_root["categoriesTree"] = info.categoriesTree;
        m_root["tags"] = info.tags;
        m_root["goals"] = info.goals;
        m_root["noteStates"] = info.noteStates;

        this->saveToFile();
    }
}

QFuture<StorageBaseInfo> StorageLocalJsonFile::getBaseInfo()
{
    return QtConcurrent::run([=]() {
        StorageBaseInfo info;
        info.categoriesTree = m_root["categoriesTree"].toArray();
        info.tags = m_root["tags"].toArray();
        info.goals = m_root["goals"].toArray();
        info.noteStates = m_root["noteStates"].toArray();
        return info;
    });
}

QFuture<QVector<QJsonObject> > StorageLocalJsonFile::getNotes(const StorageNotesFilter &filter)
{
}

void StorageLocalJsonFile::saveCategoryTree(const QJsonArray &json)
{
    m_root["categoriesTree"] = json;
    this->saveToFile();
}

void StorageLocalJsonFile::updateOrAppendBasedOnId(const QString &listKey, const QJsonObject &json, const QString &recursiveKey)
{
    QJsonArray list = m_root[listKey].toArray();
    if (this->updateOrAppendBasedOnId_r(list, json, recursiveKey)) {
        m_root[listKey] = list;
    }
}

bool StorageLocalJsonFile::updateOrAppendBasedOnId_r(QJsonArray &list, const QJsonObject &json, const QString &recursiveKey)
{
    bool found = false;

    for (auto i = 0; i < list.count(); ++i) {
        bool same = list[i].toObject()["id"].toString() == json["id"].toString();
        if (same) {
            list[i] = json;
            found = true;
            break;
        }
    }

    if (!found && !recursiveKey.isEmpty()) {
        for (auto i = 0; i < list.count(); ++i) {
            QJsonObject child = list[i].toObject();
            QJsonArray superChildList = child[recursiveKey].toArray();
            found = this->updateOrAppendBasedOnId_r(superChildList, json, recursiveKey);
            if (found) {
                child[recursiveKey] = superChildList;
                list[i] = child;
                break;
            }
        }
    }

    return found;
}

bool StorageLocalJsonFile::deleteBasedOnId_r(QJsonArray &list, const QString &id, const QString &recursiveKey)
{
    bool found = false;

    for (auto i = 0; i < list.count(); ++i) {
        bool same = list[i].toObject()["id"].toString() == id;
        if (same) {
            list.removeAt(i);
            found = true;
            break;
        }
    }

    if (!found && !recursiveKey.isEmpty()) {
        for (auto i = 0; i < list.count(); ++i) {
            QJsonObject child = list[i].toObject();
            QJsonArray superChildList = child[recursiveKey].toArray();
            found = this->deleteBasedOnId_r(superChildList, id, recursiveKey);
            if (found) {
                child[recursiveKey] = superChildList;
                list[i] = child;
                break;
            }
        }
    }

    return found;
}

void StorageLocalJsonFile::deleteBasedOnId(const QString &listKey, const QString &id, const QString &recursiveKey)
{
    QJsonArray list = m_root[listKey].toArray();
    if (this->deleteBasedOnId_r(list, id, recursiveKey)) {
        m_root[listKey] = list;
    }
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
