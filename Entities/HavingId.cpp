#include "HavingId.hpp"
#include <QUuid>

namespace deeper {

const QString HavingId::InvalidId = QString("");

void HavingId::generateRandomId()
{
    m_id = QUuid::createUuid().toString(QUuid::Id128);
}

}
