#ifndef STORAGEEXCEPTION_HPP
#define STORAGEEXCEPTION_HPP

#include <QException>

namespace deeper {

class StorageException : public QException
{
public:
    StorageException(const QString &reason);

    void raise() const override { throw *this; }
    StorageException *clone() const override { return new StorageException(*this); }

    const char * what() const noexcept override;

private:
    QString m_reason;
};

} // namespace deeper

#endif // STORAGEEXCEPTION_HPP
