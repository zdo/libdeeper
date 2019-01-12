#include "TestHelper.hpp"
#include <QFile>

static QString JsonTmpPath = QDir::temp().absoluteFilePath("libdeeper_test.json");

class StorageAutoRemoveOnDestructionLocalJsonFile : public StorageLocalJsonFile {
public:
    StorageAutoRemoveOnDestructionLocalJsonFile(const QString &path) : StorageLocalJsonFile(path) {

    }

    ~StorageAutoRemoveOnDestructionLocalJsonFile() override {
        QFile::remove(this->path());
    }
};

QString readFileToString(const QString &path)
{
    QFile file(path);
    bool ok = file.open(QFile::ReadOnly);
    if (!ok) {
        throw std::runtime_error(QString("Can't open %1").arg(path).toUtf8().constData());
    }
    return QString::fromUtf8(file.readAll());
}

QSharedPointer<Database> createDatabase(const QString &path)
{
    auto storage = QSharedPointer<StorageAutoRemoveOnDestructionLocalJsonFile>::create(path.isEmpty() ? JsonTmpPath : path);
    auto database = QSharedPointer<Database>::create(storage);
    database->refresh();
    return database;
}

QSharedPointer<Database> createPersistentDatabase(const QString &path)
{
    auto storage = QSharedPointer<StorageLocalJsonFile>::create(path.isEmpty() ? JsonTmpPath : path);
    auto database = QSharedPointer<Database>::create(storage);
    database->refresh();
    return database;
}

QSharedPointer<Database> createDatabaseFromTemplate(const QString &path)
{
    auto database = createDatabase(":/test/database.json");

    auto writableStorage = QSharedPointer<StorageAutoRemoveOnDestructionLocalJsonFile>::create(path.isEmpty() ? JsonTmpPath : path);
    database->switchAndOverwriteStorage(writableStorage);
    return database;
}

QJsonObject jsonObjectFromFile(const QString &path)
{
    QJsonObject json = QJsonDocument::fromJson(readFileToString(path).toUtf8()).object();
    return json;
}

QSharedPointer<Database> createPersistentDatabaseFromTemplate(const QString &path)
{
    auto database = createDatabase(":/test/database.json");

    auto writableStorage = QSharedPointer<StorageLocalJsonFile>::create(path.isEmpty() ? JsonTmpPath : path);
    database->switchAndOverwriteStorage(writableStorage);
    return database;
}
