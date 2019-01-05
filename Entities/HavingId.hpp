#ifndef HAVINGID_HPP
#define HAVINGID_HPP

#include <QString>

namespace deeper {

class HavingId {
public:
    static const QString InvalidId;

    const QString & id() const { return m_id; }
    void setId(const QString &id) { m_id = id; }

    void generateRandomId();

protected:
    QString m_id = InvalidId;
};

}

#endif // HAVINGID_HPP
