#ifndef CATEGORYTEST_HPP
#define CATEGORYTEST_HPP

#include "TestHelper.hpp"

class BasicTest : public QObject
{
    Q_OBJECT

private:
    QString getTmpPath()
    {
        return QDir::temp().absoluteFilePath("libdeeper_basic_test.json");
    }

private slots:
    void checkDatabaseCreation1()
    {
        createDatabase();
    }

    void checkDatabaseCreation2()
    {
        createDatabaseFromTemplate();
    }

    void databaseBasicCheck()
    {
        auto db = createDatabaseFromTemplate();

        QCOMPARE(db->categories()->rootObjects().count(), 2);
        QVERIFY(db->categories()->objectWithId("someid") != nullptr);
        QVERIFY(db->categories()->objectWithId("child1") != nullptr);
        QVERIFY(db->categories()->objectWithId("child2") != nullptr);
    }

    void databaseCheckParent()
    {
        auto db = createDatabaseFromTemplate();

        auto root = db->categories()->objectWithId("someid");
        auto child1 = db->categories()->objectWithId("child1");
        auto child2 = db->categories()->objectWithId("child2");
        auto root2 = db->categories()->objectWithId("root2");

        QVERIFY(db->categories()->parent(child1) == root);
        QVERIFY(db->categories()->parent(child2) == root);
        QVERIFY(db->categories()->parent(root) == nullptr);
        QVERIFY(db->categories()->parent(root2) == nullptr);
    }

    void databaseParentChanging()
    {
        auto db = createDatabaseFromTemplate();

        auto root = db->categories()->objectWithId("someid");
        auto child1 = db->categories()->objectWithId("child1");
        auto child2 = db->categories()->objectWithId("child2");
        auto root2 = db->categories()->objectWithId("root2");

        db->setCategoryParent(child2, child1);
        QVERIFY(db->categories()->parent(child1) == root);
        QVERIFY(db->categories()->parent(child2) == child1);
        QVERIFY(db->categories()->parent(root) == nullptr);
        QVERIFY(db->categories()->parent(root2) == nullptr);

        db->setCategoryParent(root2, root, 0);
        QCOMPARE(db->categories()->rootObjects().count(), 1);
        QCOMPARE(db->categories()->rootObjects()[0]->id(), "someid");
        QCOMPARE(db->categories()->children(root).count(), 2);
        QCOMPARE(db->categories()->children(root)[0]->id(), "root2");
        QCOMPARE(db->categories()->children(root)[1]->id(), "child1");
    }

    void databaseParentChangingClampingRegressive()
    {
        auto db = createDatabaseFromTemplate();

        auto root = db->categories()->objectWithId("someid");
        auto child1 = db->categories()->objectWithId("child1");
        auto child2 = db->categories()->objectWithId("child2");
        auto root2 = db->categories()->objectWithId("root2");

        db->setCategoryParent(root2, nullptr, 100500); // too big index offset
        QCOMPARE(db->categories()->rootObjects().count(), 2);
        QCOMPARE(db->categories()->rootObjects()[0]->id(), "someid");
        QCOMPARE(db->categories()->rootObjects()[1]->id(), "root2");

        db->setCategoryParent(root, nullptr, -100500); // too small index offset
        QCOMPARE(db->categories()->rootObjects().count(), 2);
        QCOMPARE(db->categories()->rootObjects()[0]->id(), "root2");
        QCOMPARE(db->categories()->rootObjects()[1]->id(), "someid");
    }

    void databaseDeleteCategoryRegressive()
    {
        auto db = createDatabaseFromTemplate();

        auto root = db->categories()->objectWithId("someid");
        auto child1 = db->categories()->objectWithId("child1");
        auto child2 = db->categories()->objectWithId("child2");
        auto root2 = db->categories()->objectWithId("root2");

        db->deleteCategory(root);
        QCOMPARE(db->categories()->rootObjects().count(), 1);
        QCOMPARE(db->categories()->rootObjects()[0]->id(), "root2");
    }

    void basicNotesOperations()
    {
        auto db = createPersistentDatabaseFromTemplate(getTmpPath());

        auto root = db->categories()->objectWithId("someid");
        auto child1 = db->categories()->objectWithId("child1");
        auto child2 = db->categories()->objectWithId("child2");
        auto root2 = db->categories()->objectWithId("root2");

        auto note1 = db->createNote(root);
        note1->setTitle("Note1");
        db->saveNote(note1);

        auto notes = db->notes(root);
        QCOMPARE(notes->count(), 1);
        QCOMPARE(notes->rootObjects()[0]->title(), "Note1");


        auto note2 = db->createNote(root);
        note2->setTitle("Note2");
        db->saveNote(note2);

        notes = db->notes(root);
        QCOMPARE(notes->rootObjects().count(), 2);
        QCOMPARE(notes->rootObjects()[0]->title(), "Note1");
        QCOMPARE(notes->rootObjects()[1]->title(), "Note2");
    }

    void readNotesFromFile()
    {
        auto db = createPersistentDatabase(getTmpPath());

        auto root = db->categories()->objectWithId("someid");
        root->setTitle("Job");
        db->saveCategory(root);

        auto notes = db->notes(root);
        QCOMPARE(notes->rootObjects().count(), 2);
        QCOMPARE(notes->rootObjects()[0]->title(), "Note1");
        QCOMPARE(notes->rootObjects()[1]->title(), "Note2");

        db->setNoteParent(notes->rootObjects()[1],
                db->categories()->objectWithId(notes->rootObjects()[0]->categoryId()),
                notes->rootObjects()[0]);
        notes = db->notes(root);
        QCOMPARE(notes->rootObjects().count(), 1);

        QCOMPARE(notes->rootObjects()[0]->title(), "Note1");
        QCOMPARE(notes->children(notes->rootObjects()[0])[0]->title(), "Note2");

        db->deleteNote(notes->rootObjects()[0]);
        QCOMPARE(notes->rootObjects().count(), 0);
    }
};

#endif // CATEGORYTEST_HPP
