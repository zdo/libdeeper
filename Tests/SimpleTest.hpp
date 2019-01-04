#ifndef SIMPLETEST_HPP
#define SIMPLETEST_HPP

#include <QObject>
#include <QTest>

#include "Entities/Category.hpp"

namespace deeper {

class SimpleTest : public QObject
{
    Q_OBJECT

private slots:
    void categoryJson()
    {
        QJsonObject json;
        json["id"] = 10;
        json["title"] = u8"Привет";
        json["parentId"] = 2;
        json["tags"] = QJsonArray({ 3, 4 });

        Category category;
        category.deserializeFromJson(json);

        QCOMPARE(category.id(), 10);
        QCOMPARE(category.title(), u8"Привет");
        QCOMPARE(category.parentId(), 2);
    }
};

} // namespace deeper

#endif // SIMPLETEST_HPP
