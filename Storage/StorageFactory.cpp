#include "StorageFactory.hpp"
#include "StorageException.hpp"

#include "StorageLocalJsonFile.hpp"

namespace deeper {

QSharedPointer<AbstractStorage> StorageFactory::createStorage(const QUrl &url)
{
    if (url.scheme() == "file") {
        auto lowerPath = url.path().toLower();
        if (lowerPath.endsWith("json")) {
            return QSharedPointer<StorageLocalJsonFile>::create(url.path());
        } else {
            throw StorageException(QObject::tr("Can't create storage due to unknown file extension"));
        }
    } else {
        throw StorageException(QObject::tr("Can't create storage due to unknown URL scheme"));
    }
}

} // namespace deeper
