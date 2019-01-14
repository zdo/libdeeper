#ifndef LOCALSQLITEBACKENDTEST_HPP
#define LOCALSQLITEBACKENDTEST_HPP

#include "./TestHelper.hpp"
#include "Backend/LocalSqliteBackend.hpp"

class LocalSqliteBackendTest : public QObject
{
    Q_OBJECT

private slots:
    void test1()
    {
        auto backend = LocalSqliteBackend(":memory:");

        auto root = backend.createCategory();
        root->setTitle("Root category");
        backend.saveCategory(root);

        QCOMPARE(root->id(), 1);
        QCOMPARE(root->title(), "Root category");
        QCOMPARE(backend.category(1), root);


        auto root2 = backend.createCategory();
        root2->setTitle("Root category 2");
        backend.saveCategory(root);

        QCOMPARE(root2->id(), 2);
        QCOMPARE(root2->orderIndex(), 1);
        QCOMPARE(root2->title(), "Root category 2");
        QCOMPARE(backend.category(2), root2);
    }
};

#endif // LOCALSQLITEBACKENDTEST_HPP
