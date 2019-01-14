#ifndef TIMETRACKTEST_HPP
#define TIMETRACKTEST_HPP

#include "TestHelper.hpp"

class TimeTrackTest : public QObject
{
    Q_OBJECT

private slots:
    void timeTrack0()
    {
        const int h = 2;
        const int m = 21;
        const int s = 47;
        const int duration = s + m * 60 + h * 3600;

        TimeTrack t;
        t.setStart(QDateTime::currentDateTime().addSecs(-duration));
        t.setEndToNow();

        QCOMPARE(t.durationInSeconds(), duration);

        auto d = t.duration();
        QCOMPARE(d.hours, h);
        QCOMPARE(d.minutes, m);
        QCOMPARE(d.seconds, s);
    }

    void timeTrack1()
    {
        auto db = createDatabaseFromTemplate();
        auto root = db->categories()->rootObjects().first();
        auto note = db->createNote(root);

        QCOMPARE(db->activeTimeTrack(), nullptr);
        QCOMPARE(db->timeTracksForNote(note).count(), 0);

        auto tt1 = db->startTimeTrack(note);
        QCOMPARE(db->timeTracksForNote(note).count(), 1);
        QCOMPARE(db->activeTimeTrack(), tt1);

        int durationSeconds = rand() % 3600;
        tt1->setStart(QDateTime::currentDateTime().addSecs(-durationSeconds));
        tt1->setEndToNow();
        db->saveTimeTrack(tt1);
        QCOMPARE(db->timeTracksForNote(note).count(), 1);
        QCOMPARE(db->activeTimeTrack(), nullptr);

        QVERIFY(tt1->start().isValid());
        QVERIFY(tt1->end().isValid());
        QCOMPARE(tt1->durationInSeconds(), durationSeconds);
    }

    void timeTrack2()
    {
        auto db = createDatabaseFromTemplate();
        auto root = db->categories()->rootObjects().first();
        auto note = db->createNote(root);

        auto tt1 = db->startTimeTrack(note);
        tt1->setStart(QDateTime::currentDateTime().addSecs(-30));
        tt1->setEndToNow();
        db->saveTimeTrack(tt1);

        auto tt2 = db->startTimeTrack(note);
        tt2->setStart(QDateTime::currentDateTime().addSecs(-20));
        tt2->setEndToNow();
        db->saveTimeTrack(tt2);

        QCOMPARE(db->noteTotalDuration(note).seconds, 50);
    }
};

#endif // TIMETRACKTEST_HPP
