#-------------------------------------------------
#
# Project created by QtCreator 2019-01-04T12:14:10
#
#-------------------------------------------------

QT       -= gui
QT       += core sql
QTPLUGIN += QSQLITE

# Enable to test.
 CONFIG += TESTME

contains(CONFIG, TESTME) {
    message("Test")
    TARGET = libdeeper_test
    CONFIG += testcase
    QT += testlib

    SOURCES += libdeeper_test.cpp \
    Tests/TestHelper.cpp

    HEADERS += \
    Tests/TestHelper.hpp \
    Tests/LocalSqliteBackendTest.hpp
    # Tests/TimeTrackTest.hpp \
    # Tests/BasicTest.hpp \
    # Tests/EntitiesTest.hpp \
    # Tests/FilterTest.hpp
} else {
    TEMPLATE = lib
    TARGET = deeper
    CONFIG += staticlib

unix {
    target.path = /usr/local/lib
    INSTALLS += target

    headers.path    = /usr/local/include/libdeeper
    headers.files   += *.hpp
    INSTALLS       += headers

    headers1.path    = /usr/local/include/libdeeper/Entities
    headers1.files   += Entities/*.hpp
    INSTALLS       += headers1

    headers2.path    = /usr/local/include/libdeeper/Storage
    headers2.files   += Storage/*.hpp
    INSTALLS       += headers2
}

}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Entities/Category.cpp \
    Entities/Note.cpp \
    Entities/NoteState.cpp \
    Entities/Tag.cpp \
    Entities/TimeTrack.cpp \
    Entities/Goal.cpp \
    Entities/Milestone.cpp \
    Overview/TimeTable.cpp \
    Overview/Filter.cpp \
    Entities/Book.cpp \
    Backend/BackendFactory.cpp \
    Backend/LocalSqliteBackend.cpp \
    Entities/Helpers/BackendEntity.cpp \
    Entities/Helpers/BackendFamilyEntity.cpp

HEADERS += \
        libdeeper.hpp \
    Entities/Category.hpp \
    Entities/Note.hpp \
    Entities/NoteState.hpp \
    Entities/Tag.hpp \
    Entities/TimeTrack.hpp \
    Entities/Goal.hpp \
    Entities/Helpers/HavingParentTree.hpp \
    Entities/Milestone.hpp \
    Overview/TimeTable.hpp \
    Overview/Filter.hpp \
    Overview/CustomFilters.hpp \
    Entities/Book.hpp \
    Backend/AbstractBackend.hpp \
    Backend/BackendFactory.hpp \
    Backend/LocalSqliteBackend.hpp \
    Entities/Helpers/BackendEntity.hpp \
    Entities/Helpers/BackendFamilyEntity.hpp

RESOURCES += \
    Resources.qrc
