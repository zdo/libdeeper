#include "StorageLocalJsonFile.hpp"
#include <QFile>
#include <QJsonDocument>

namespace deeper {

static QJsonObject ArrayToObjectWithIdentifiableObjects(const QJsonArray &array)
{
    QJsonObject result;
    for (auto raw : array) {
        QJsonObject o = raw.toObject();
        result[o["id"].toString()] = o;
    }
    return result;
}

static QJsonArray ObjectToArrayWithIdentifiableObjects(const QJsonObject &obj)
{
    QJsonArray result;
    for (auto key : obj.keys()) {
        QJsonObject o = obj[key].toObject();
        result.append(o);
    }
    return result;
}

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
        this->clearAllData();

        auto info = AbstractStorage::defaultBaseInfo();

        m_root["categories"] = ArrayToObjectWithIdentifiableObjects(info.categories);
        m_root["tags"] = ArrayToObjectWithIdentifiableObjects(info.tags);
        m_root["goals"] = ArrayToObjectWithIdentifiableObjects(info.goals);
        m_root["noteStates"] = ArrayToObjectWithIdentifiableObjects(info.noteStates);
        m_root["notes"] = QJsonObject();

        this->saveToFile();
    }
}

void StorageLocalJsonFile::clearAllData()
{
    m_root["categories"] = QJsonObject();
    m_root["tags"] = QJsonObject();
    m_root["goals"] = QJsonObject();
    m_root["noteStates"] = QJsonObject();
    m_root["notes"] = QJsonObject();

    this->saveToFile();
}

StorageBaseInfo StorageLocalJsonFile::getBaseInfo()
{
    StorageBaseInfo info;
    info.categories = ObjectToArrayWithIdentifiableObjects(m_root["categories"].toObject());
    info.tags = ObjectToArrayWithIdentifiableObjects(m_root["tags"].toObject());
    info.goals = ObjectToArrayWithIdentifiableObjects(m_root["goals"].toObject());
    info.noteStates = ObjectToArrayWithIdentifiableObjects(m_root["noteStates"].toObject());
    return info;
}

void StorageLocalJsonFile::saveNote(const QJsonObject &json)
{
    QJsonObject obj = m_root["notes"].toObject();
    obj[json["id"].toString()] = json;
    m_root["notes"] = obj;
    this->saveToFile();
}

void StorageLocalJsonFile::deleteNote(const QString &id)
{
    QJsonObject obj = m_root["notes"].toObject();
    obj.remove(id);
    m_root["notes"] = obj;
    this->saveToFile();
}

void StorageLocalJsonFile::saveCategory(const QJsonObject &json)
{
    QJsonObject obj = m_root["categories"].toObject();
    obj[json["id"].toString()] = json;
    m_root["categories"] = obj;
    this->saveToFile();
}

void StorageLocalJsonFile::deleteCategory(const QString &id)
{
    QJsonObject obj = m_root["categories"].toObject();
    obj.remove(id);
    m_root["categories"] = obj;
    this->saveToFile();
}

QJsonArray StorageLocalJsonFile::notes(const QString &categoryId, const QString &parentNoteId)
{
    QJsonArray notesResult;
    for (auto noteRaw : m_root["notes"].toArray()) {
        auto note = noteRaw.toObject();
        if (note["parentId"].toString() == parentNoteId && note["categoryId"].toString() == categoryId) {
            notesResult.append(note);
        }
    }
    return notesResult;
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
