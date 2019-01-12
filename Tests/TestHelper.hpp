#ifndef TESTHELPER_HPP
#define TESTHELPER_HPP

#include <QObject>
#include <QString>

#include <QTest>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

#include "Entities/Database.hpp"
#include "Storage/StorageLocalJsonFile.hpp"

using namespace deeper;

QString readFileToString(const QString &path);

QJsonObject jsonObjectFromFile(const QString &path);
QSharedPointer<Database> createDatabase(const QString &path = "");
QSharedPointer<Database> createPersistentDatabase(const QString &path = "");
QSharedPointer<Database> createDatabaseFromTemplate(const QString &path = "");
QSharedPointer<Database> createPersistentDatabaseFromTemplate(const QString &path = "");

#endif // TESTHELPER_HPP
