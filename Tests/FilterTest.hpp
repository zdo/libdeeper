#ifndef FILTERTEST_HPP
#define FILTERTEST_HPP

#include "TestHelper.hpp"

class FilterTest : public QObject
{
    Q_OBJECT

private slots:
    void testSerialize()
    {
        Filter filter;
        filter.trackDate.isEnabled = true;
        filter.trackDate.start = QDate(2009, 11, 9);
        filter.trackDate.end = QDate(2009, 11, 19);

        auto json = filter.serializeToJson();

        Filter filter2;
        filter2.deserializeFromJson(json);

        QCOMPARE(filter.trackDate.start, filter2.trackDate.start);
        QCOMPARE(filter.trackDate.end, filter2.trackDate.end);
        QCOMPARE(filter.trackDate.isEnabled, filter2.trackDate.isEnabled);
    }

    void testTrackDate()
    {
        auto db = createDatabaseFromTemplate();
        auto root = db->categories()->rootObjects().first();
        auto note = db->createNote(root);

        auto tt1 = db->startTimeTrack(note);
        tt1->setStart(QDateTime(QDate(2018, 5, 12), QTime(11, 15, 0)));
        tt1->setEnd(QDateTime(QDate(2018, 5, 12), QTime(11, 45, 0)));
        db->saveTimeTrack(tt1);

        auto tt2 = db->startTimeTrack(note);
        tt2->setStart(QDateTime(QDate(2018, 5, 12), QTime(11, 00, 0)));
        tt2->setEnd(QDateTime(QDate(2018, 5, 12), QTime(11, 30, 0)));
        db->saveTimeTrack(tt2);


    }

    void testFlipOverNight()
    {

    }
};

#endif // FILTERTEST_HPP
