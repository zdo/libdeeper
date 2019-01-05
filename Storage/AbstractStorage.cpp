#include "AbstractStorage.hpp"
#include <QFile>

namespace deeper {

AbstractStorage::~AbstractStorage()
{
}

StorageBaseInfo AbstractStorage::defaultBaseInfo()
{
    StorageBaseInfo info;

    info.tags.append(QJsonObject{ { "id", "1" }, { "title", "deep" }});

    info.categoriesTree.append(QJsonObject{ { "id", "1" }, { "title", "Job" }});
    info.categoriesTree.append(QJsonObject{ { "id", "2" }, { "title", "Personal" }});

    info.noteStates.append(QJsonObject{ { "id", "1" }, { "title", "TODO" }, { "color", "#cc0000" }, { "isFinished", false } });
    info.noteStates.append(QJsonObject{ { "id", "2" }, { "title", "DONE" }, { "color", "#00cc00" }, { "isFinished", true } });

    return info;
}

} // namespace deeper
