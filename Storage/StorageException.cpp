#include "StorageException.hpp"

namespace deeper {

StorageException::StorageException(const QString &reason)
{
    m_reason = reason;
}

const char *StorageException::what() const noexcept
{
    return m_reason.toUtf8().constData();
}

} // namespace deeper
