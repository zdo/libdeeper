#ifndef ENTITIESTEST_HPP
#define ENTITIESTEST_HPP

#include "TestHelper.hpp"

class EntitiesTest : public QObject
{
    Q_OBJECT

private:
private slots:
    void categoryDeserialize()
    {
        QJsonObject json = jsonObjectFromFile(":/test/category.json");

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
        QJsonObject json = jsonObjectFromFile(":/test/category.json");

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
};

#endif // ENTITIESTEST_HPP
