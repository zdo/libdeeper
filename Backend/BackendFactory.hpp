#ifndef BACKENDFACTORY_HPP
#define BACKENDFACTORY_HPP

#include "AbstractBackend.hpp"
#include <QSharedPointer>
#include <QUrl>

namespace deeper {

class BackendFactory
{
public:
    static QSharedPointer<AbstractBackend> createStorage(const QString &path);

private:
    BackendFactory();
};

} // namespace deeper

#endif // BACKENDFACTORY_HPP
