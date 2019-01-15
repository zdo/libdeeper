#include "BackendEntity.hpp"
#include <QUuid>

namespace deeper {

const int BackendEntity::InvalidId = 0;

void BackendEntity::setId(int id)
{
    m_id = id;
}

AbstractBackend *BackendEntity::getBackendOrError() const
{
    if (m_backend == nullptr) {
        throw std::runtime_error("Backend was already destroyed");
    }
    return m_backend;
}

void BackendEntity::__setBackend(AbstractBackend *backend)
{
    m_backend = backend;
}

}
