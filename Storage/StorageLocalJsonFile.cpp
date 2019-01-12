#include "StorageLocalJsonFile.hpp"
#include <QFile>
#include <QJsonDocument>
#include <cmath>

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
    m_root["timeTracks"] = QJsonObject();
    m_root["milestones"] = QJsonObject();

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

QJsonObject StorageLocalJsonFile::activeTimeTrack()
{
    QJsonObject timeTracks = m_root["timeTracks"].toObject();
    for (QJsonValue timeTrackRaw : timeTracks) {
        auto timeTrack = timeTrackRaw.toObject();
        if (fabs(timeTrack["end"].toDouble()) <= 1.0) {
            return timeTrack;
        }
    }

    return QJsonObject();
}

QJsonArray StorageLocalJsonFile::timeTracksForNote(const QString &noteId)
{
    QJsonArray result;
    QJsonObject timeTracks = m_root["timeTracks"].toObject();
    for (QJsonValue timeTrackRaw : timeTracks) {
        auto timeTrack = timeTrackRaw.toObject();
        if (timeTrack["noteId"].toString() == noteId) {
            result.append(timeTrack);
        }
    }

    return result;
}

void StorageLocalJsonFile::saveTimeTrack(const QJsonObject &tt)
{
    QJsonObject timeTracks = m_root["timeTracks"].toObject();
    timeTracks[tt["id"].toString()] = tt;
    m_root["timeTracks"] = timeTracks;
    this->saveToFile();
}

void StorageLocalJsonFile::deleteTimeTrack(const QString &id)
{
    QJsonObject timeTracks = m_root["timeTracks"].toObject();
    timeTracks.remove(id);
    m_root["timeTracks"] = timeTracks;
    this->saveToFile();
}

const QString &StorageLocalJsonFile::path() const
{
    return m_path;
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

QJsonArray StorageLocalJsonFile::notes(const QString &categoryId)
{
    QJsonArray notesResult;

    auto notes = m_root["notes"].toObject();
    for (auto noteRaw : notes) {
        auto note = noteRaw.toObject();
        if (note["categoryId"].toString() == categoryId) {
            notesResult.append(note);
        }
    }
    return notesResult;
}

QJsonObject StorageLocalJsonFile::note(const QString &id)
{
    auto notes = m_root["notes"].toObject();
    for (auto noteRaw : notes) {
        auto note = noteRaw.toObject();
        if (note["id"].toString() == id) {
            return note;
        }
    }
    throw StorageException(QObject::tr("No such note with id \"%1\"").arg(id));
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
