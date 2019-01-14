#include "Book.hpp"
#include "../Backend/BackendFactory.hpp"
#include <sstream>

namespace deeper {

Book::Book(const QString &path)
{
    m_storage = BackendFactory::createStorage(path);
    Q_ASSERT(m_storage != nullptr);
}

} // namespace deeper
