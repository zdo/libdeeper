//#include "Tests/EntitiesTest.hpp"
//#include "Tests/BasicTest.hpp"
//#include "Tests/TimeTrackTest.hpp"
//#include "Tests/FilterTest.hpp"

#include "Tests/LocalSqliteBackendTest.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    QList<QObject *> tests = {
        new LocalSqliteBackendTest,
//        new EntitiesTest,
//        new BasicTest,
//        new TimeTrackTest,
//        new FilterTest
    };
    for (auto test : tests) {
        test->setParent(&app);
    }

    int status = 0;
    for (auto test : tests) {
        QTEST_SET_MAIN_SOURCE_PATH
        status = QTest::qExec(test, argc, argv);
        if (status != 0) {
            break;
        }
    }
    return status;
}
