#-------------------------------------------------
#
# Project created by QtCreator 2019-01-04T12:14:10
#
#-------------------------------------------------

QT       -= gui
QT       += core concurrent

TARGET = deeper
TEMPLATE = lib

CONFIG += staticlib

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
    Entities/Database.cpp \
    Entities/Note.cpp \
    Entities/NoteState.cpp \
    Entities/Tag.cpp \
    Storage/AbstractStorage.cpp \
    Storage/StorageFactory.cpp \
    Storage/StorageException.cpp \
    Storage/StorageLocalJsonFile.cpp \
    Entities/TimeTrack.cpp \
    Entities/Achievement.cpp \
    Entities/Goal.cpp

HEADERS += \
        libdeeper.hpp \
    Entities/Category.hpp \
    Entities/Database.hpp \
    Entities/Note.hpp \
    Entities/NoteState.hpp \
    Entities/Serializable.hpp \
    Entities/Tag.hpp \
    Storage/AbstractStorage.hpp \
    libdeeper.hpp \
    Storage/StorageFactory.hpp \
    Storage/StorageException.hpp \
    Storage/StorageLocalJsonFile.hpp \
    Entities/TimeTrack.hpp \
    Entities/Achievement.hpp \
    Entities/Goal.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES +=

DISTFILES +=
