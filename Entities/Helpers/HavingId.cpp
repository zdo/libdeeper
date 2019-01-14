#include "HavingId.hpp"
#include <QUuid>

namespace deeper {

void HavingId::setId(int id)
{
    m_id = id;
}

AbstractBackend *HavingId::getBackendOrThrowError() const
{
    if (m_backend == nullptr) {
        throw std::runtime_error("Backend was already destroyed");
    }
    return m_backend;
}

}
