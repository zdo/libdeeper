#ifndef TIMETRACKTEST_HPP
#define TIMETRACKTEST_HPP

#include "TestHelper.hpp"

class TimeTrackTest : public QObject
{
    Q_OBJECT

private slots:
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
        tt1->setEnd(QDateTime::currentDateTime());
        db->saveTimeTrack(tt1);
        QCOMPARE(db->timeTracksForNote(note).count(), 1);
        QCOMPARE(db->activeTimeTrack(), nullptr);

        QVERIFY(tt1->start().isValid());
        QVERIFY(tt1->end().isValid());
        QCOMPARE(tt1->durationInSeconds(), durationSeconds);
    }
};

#endif // TIMETRACKTEST_HPP
