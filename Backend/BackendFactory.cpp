#include "BackendFactory.hpp"
#include <stdexcept>

#include "LocalSqliteBackend.hpp"

namespace deeper {

QSharedPointer<AbstractBackend> BackendFactory::createStorage(const QString &path)
{
    return QSharedPointer<LocalSqliteBackend>::create(path);
}

} // namespace deeper
