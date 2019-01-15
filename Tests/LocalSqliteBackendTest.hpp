#ifndef LOCALSQLITEBACKENDTEST_HPP
#define LOCALSQLITEBACKENDTEST_HPP

#include "./TestHelper.hpp"
#include "Backend/LocalSqliteBackend.hpp"

class LocalSqliteBackendTest : public QObject
{
    Q_OBJECT

private slots:
    void basicTest()
    {
        auto backend = LocalSqliteBackend(":memory:");

        auto root = backend.createCategory();
        root->setTitle("Root category");
        root->save();

        QCOMPARE(root->id(), 1);
        QCOMPARE(root->title(), "Root category");
        QCOMPARE(backend.categoryWithId(1), root);


        auto root2 = backend.createCategory();
        root2->setTitle("Root category 2");
        root2->save();

        QCOMPARE(root2->id(), 2);
        QCOMPARE(root2->orderIndex(), 1);
        QCOMPARE(root2->title(), "Root category 2");
        QCOMPARE(backend.categoryWithId(2), root2);
    }

    void categoryTest1()
    {
        auto backend = LocalSqliteBackend(":memory:");

        auto root = backend.createCategory();
        auto root2 = backend.createCategory();
        auto child1 = backend.createCategory(root->id());
        auto child11 = backend.createCategory(child1->id());
        auto child2 = backend.createCategory(root2->id());

        // * root
        //   * child1
        //     * child11
        // * root2
        //   *child2

        QCOMPARE(backend.rootCategories().count(), 2);
        QCOMPARE(backend.categoryChildrenCount(), 2);

        QCOMPARE(root->children().count(), 1);
        QCOMPARE(root2->children().count(), 1);

        QCOMPARE(child1->parent(), root);
        QCOMPARE(child1->children().count(), 1);
        QCOMPARE(backend.categoryChildrenCount(child1->id()), 1);
        QCOMPARE(child11->children().count(), 0);

        // Remove.
        child1->remove();
        QCOMPARE(root->children().count(), 0);
        QCOMPARE(root2->children().count(), 1);

        QCOMPARE(backend.categoryWithId(child1->id()), nullptr);
        QCOMPARE(backend.categoryWithId(child11->id()), nullptr);
    }

    void categoryTestRemoveAndMove()
    {
        auto backend = LocalSqliteBackend(":memory:");

        auto root = backend.createCategory();
        auto child1 = backend.createCategory(root->id());
        auto child2 = backend.createCategory(root->id());
        auto child3 = backend.createCategory(root->id());

        QCOMPARE(child1->orderIndex(), 0);
        QCOMPARE(child2->orderIndex(), 1);
        QCOMPARE(child3->orderIndex(), 2);

        child2->remove();
        QCOMPARE(child2->id(), BackendEntity::InvalidId);
        QCOMPARE(child1->orderIndex(), 0);
        QCOMPARE(child3->orderIndex(), 1);


        auto child4 = backend.createCategory(root->id());
        auto child5 = backend.createCategory(root->id());
        QCOMPARE(child4->orderIndex(), 2);
        QCOMPARE(child5->orderIndex(), 3);

        child4->move(nullptr, 0);
        QCOMPARE(root->orderIndex(), 1);
        QCOMPARE(child4->orderIndex(), 0);
        QCOMPARE(child4->parent(), nullptr);
        QCOMPARE(child5->orderIndex(), 2);

        child5->move(root, 0);
        QCOMPARE(child5->orderIndex(), 0);
        QCOMPARE(child1->orderIndex(), 1);
        QCOMPARE(child3->orderIndex(), 2);

        child1->remove();
        QCOMPARE(child1->id(), BackendEntity::InvalidId);
        QCOMPARE(child5->orderIndex(), 0);
        QCOMPARE(child3->orderIndex(), 1);
    }

    void tagTest1()
    {
        auto backend = LocalSqliteBackend(":memory:");

        auto root = backend.createCategory();
        auto child1 = backend.createCategory(root->id());

        auto tag1 = backend.createTag("tag1");
        QCOMPARE(tag1->title(), "tag1");

        child1->assignTag(tag1);
        QCOMPARE(root->tags().count(), 0);
        QCOMPARE(child1->tags().count(), 1);
        QCOMPARE(child1->tags()[0], tag1);

        child1->removeTag(tag1);
        QCOMPARE(child1->tags().count(), 0);

        root->assignTag(tag1);
        QCOMPARE(child1->tags().count(), 0);
        QCOMPARE(root->tags().count(), 1);

        tag1->remove();
        QCOMPARE(child1->tags().count(), 0);
        QCOMPARE(root->tags().count(), 0);
    }

    void notesTest1()
    {
        auto backend = LocalSqliteBackend(":memory:");

        auto root = backend.createCategory();
        auto child1 = backend.createCategory(root->id());
        QCOMPARE(root->rootNotes().count(), 0);

        auto note1 = root->createNote();
        QCOMPARE(root->rootNotes().count(), 1);
        QCOMPARE(child1->rootNotes().count(), 0);

        auto note2 = child1->createNote();
        QCOMPARE(root->rootNotes().count(), 1);
        QCOMPARE(child1->rootNotes().count(), 1);
        QCOMPARE(note1->children().count(), 0);
        QCOMPARE(note2->children().count(), 0);

        note2->move(root, note1);
        QCOMPARE(root->rootNotes().count(), 1);
        QCOMPARE(child1->rootNotes().count(), 0);
        QCOMPARE(note1->children().count(), 1);
        QCOMPARE(note2->children().count(), 0);

        note1->remove();
        QCOMPARE(root->rootNotes().count(), 0);
        QCOMPARE(child1->rootNotes().count(), 0);
    }
};

#endif // LOCALSQLITEBACKENDTEST_HPP
