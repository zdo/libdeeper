#ifndef STORAGEFACTORY_HPP
#define STORAGEFACTORY_HPP

#include "AbstractStorage.hpp"
#include <QSharedPointer>
#include <QUrl>

namespace deeper {

class StorageFactory
{
public:
    static QSharedPointer<AbstractStorage> createStorage(const QUrl &url);

private:
    StorageFactory();
};

} // namespace deeper

#endif // STORAGEFACTORY_HPP
