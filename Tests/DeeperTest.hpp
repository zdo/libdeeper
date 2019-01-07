#ifndef CATEGORYTEST_HPP
#define CATEGORYTEST_HPP

#include <QObject>
#include <QTest>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

#include "Entities/Database.hpp"
#include "Storage/StorageLocalJsonFile.hpp"

namespace deeper {

static QString readResourceFile(const QString &path)
{
    QFile file(":/test/category.json");
    bool ok = file.open(QFile::ReadOnly);
    if (!ok) {
        throw std::runtime_error(QString("Can't open %1").arg(path).toUtf8().constData());
    }
    return QString::fromUtf8(file.readAll());
}

class DeeperTest : public QObject
{
    Q_OBJECT

private:
    QString m_jsonTmpPath = QDir::temp().absoluteFilePath("libdeeper_test.json");
    QSharedPointer<Database> m_database;

private slots:
    void initTestCase()
    {
        qDebug() << "Path to temporary JSON file" << m_jsonTmpPath;
    }

    void testStorageCreation()
    {
        auto storage = QSharedPointer<StorageLocalJsonFile>::create(":/test/database.json");
        m_database = QSharedPointer<Database>::create(storage);
        m_database->refresh();
    }

    void reloadDatabaseToInitialState()
    {
        auto storage = QSharedPointer<StorageLocalJsonFile>::create(":/test/database.json");
        m_database = QSharedPointer<Database>::create(storage);
        m_database->refresh();

        auto writableStorage = QSharedPointer<StorageLocalJsonFile>::create(m_jsonTmpPath);
        m_database->switchAndOverwriteStorage(writableStorage);
    }

    void categoryDeserialize()
    {
        QJsonObject json = QJsonDocument::fromJson(readResourceFile(":/test/category.json").toUtf8()).object();

        Category category;
        category.deserializeFromJson(json);

        QCOMPARE(category.id(), "someid");
        QCOMPARE(category.title(), u8"Имя категории");
        QCOMPARE(category.parentId(), "someparent");
        QCOMPARE(category.orderIndex(), 100500);
        QCOMPARE(category.tagIdList()[1], "2");
    }

    void categorySerialize()
    {
        QJsonObject json = QJsonDocument::fromJson(readResourceFile(":/test/category.json").toUtf8()).object();

        Category category;
        category.deserializeFromJson(json);
        auto jsonOut = category.serializeToJson();

        for (auto key : json.keys()) {
            QCOMPARE(json[key].type(), jsonOut[key].type());
        }

        Category category2;
        category2.deserializeFromJson(jsonOut);

        QCOMPARE(category.id(), category2.id());
        QCOMPARE(category.title(), category2.title());

        auto jsonOut2 = category2.serializeToJson();
        QCOMPARE(jsonOut, jsonOut2);
    }

    void databaseBasicCheck()
    {
        this->reloadDatabaseToInitialState();
        QCOMPARE(m_database->categories()->rootObjects().count(), 2);
        QVERIFY(m_database->categories()->objectWithId("someid") != nullptr);
        QVERIFY(m_database->categories()->objectWithId("child1") != nullptr);
        QVERIFY(m_database->categories()->objectWithId("child2") != nullptr);
    }

    void databaseCheckParent()
    {
        auto root = m_database->categories()->objectWithId("someid");
        auto child1 = m_database->categories()->objectWithId("child1");
        auto child2 = m_database->categories()->objectWithId("child2");
        auto root2 = m_database->categories()->objectWithId("root2");

        QVERIFY(m_database->categories()->parent(child1) == root);
        QVERIFY(m_database->categories()->parent(child2) == root);
        QVERIFY(m_database->categories()->parent(root) == nullptr);
        QVERIFY(m_database->categories()->parent(root2) == nullptr);
    }

    void databaseParentChanging()
    {
        auto root = m_database->categories()->objectWithId("someid");
        auto child1 = m_database->categories()->objectWithId("child1");
        auto child2 = m_database->categories()->objectWithId("child2");
        auto root2 = m_database->categories()->objectWithId("root2");

        m_database->setCategoryParent(child2, child1);
        QVERIFY(m_database->categories()->parent(child1) == root);
        QVERIFY(m_database->categories()->parent(child2) == child1);
        QVERIFY(m_database->categories()->parent(root) == nullptr);
        QVERIFY(m_database->categories()->parent(root2) == nullptr);

        m_database->setCategoryParent(root2, root, 0);
        QCOMPARE(m_database->categories()->rootObjects().count(), 1);
        QCOMPARE(m_database->categories()->rootObjects()[0]->id(), "someid");
        QCOMPARE(m_database->categories()->children(root).count(), 2);
        QCOMPARE(m_database->categories()->children(root)[0]->id(), "root2");
        QCOMPARE(m_database->categories()->children(root)[1]->id(), "child1");
    }

    void databaseParentChangingClampingRegressive()
    {
        this->reloadDatabaseToInitialState();

        auto root = m_database->categories()->objectWithId("someid");
        auto child1 = m_database->categories()->objectWithId("child1");
        auto child2 = m_database->categories()->objectWithId("child2");
        auto root2 = m_database->categories()->objectWithId("root2");

        m_database->setCategoryParent(root2, nullptr, 100500); // too big index offset
        QCOMPARE(m_database->categories()->rootObjects().count(), 2);
        QCOMPARE(m_database->categories()->rootObjects()[0]->id(), "someid");
        QCOMPARE(m_database->categories()->rootObjects()[1]->id(), "root2");

        m_database->setCategoryParent(root, nullptr, -100500); // too small index offset
        QCOMPARE(m_database->categories()->rootObjects().count(), 2);
        QCOMPARE(m_database->categories()->rootObjects()[0]->id(), "root2");
        QCOMPARE(m_database->categories()->rootObjects()[1]->id(), "someid");
    }

    void databaseDeleteCategoryRegressive()
    {
        this->reloadDatabaseToInitialState();

        auto root = m_database->categories()->objectWithId("someid");
        auto child1 = m_database->categories()->objectWithId("child1");
        auto child2 = m_database->categories()->objectWithId("child2");
        auto root2 = m_database->categories()->objectWithId("root2");

        m_database->deleteCategory(root);
        QCOMPARE(m_database->categories()->rootObjects().count(), 1);
        QCOMPARE(m_database->categories()->rootObjects()[0]->id(), "root2");
    }

    void basicNotesOperations()
    {
        this->reloadDatabaseToInitialState();

        auto root = m_database->categories()->objectWithId("someid");
        auto child1 = m_database->categories()->objectWithId("child1");
        auto child2 = m_database->categories()->objectWithId("child2");
        auto root2 = m_database->categories()->objectWithId("root2");

        auto note1 = m_database->createNote(root);
        note1->setTitle("Note1");
        m_database->saveNote(note1);

        auto notes = m_database->notes(root);
        QCOMPARE(notes->count(), 1);
        QCOMPARE(notes->rootObjects()[0]->title(), "Note1");


        auto note2 = m_database->createNote(root);
        note2->setTitle("Note2");
        m_database->saveNote(note2);

        notes = m_database->notes(root);
        QCOMPARE(notes->rootObjects().count(), 2);
        QCOMPARE(notes->rootObjects()[0]->title(), "Note1");
        QCOMPARE(notes->rootObjects()[1]->title(), "Note2");
    }

    void readNotesFromFile()
    {
        auto writableStorage = QSharedPointer<StorageLocalJsonFile>::create(m_jsonTmpPath);
        auto database = QSharedPointer<Database>::create(writableStorage);
        database->refresh();

        auto root = database->categories()->objectWithId("someid");
        auto notes = database->notes(root);
        QCOMPARE(notes->rootObjects().count(), 2);
        QCOMPARE(notes->rootObjects()[0]->title(), "Note1");
        QCOMPARE(notes->rootObjects()[1]->title(), "Note2");

        database->setNoteParent(notes->rootObjects()[1],
                database->categories()->objectWithId(notes->rootObjects()[0]->categoryId()),
                notes->rootObjects()[0]);
        notes = database->notes(root);
        QCOMPARE(notes->rootObjects().count(), 1);

        QCOMPARE(notes->rootObjects()[0]->title(), "Note1");
        QCOMPARE(notes->children(notes->rootObjects()[0])[0]->title(), "Note2");

        database->deleteNote(notes->rootObjects()[0]);
        QCOMPARE(notes->rootObjects().count(), 0);
    }

    void cleanupTestCase()
    {
        QFile::remove(m_jsonTmpPath);
    }
};

} // namespace deeper

#endif // CATEGORYTEST_HPP
