#ifndef HAVINGID_HPP
#define HAVINGID_HPP

namespace deeper {

class AbstractBackend;

class HavingId {
public:
    static const int InvalidId = 0;

    const int & id() const { return m_id; }
    void setId(int id);

protected:
    AbstractBackend * getBackendOrThrowError() const;

private:
    friend AbstractBackend;

    AbstractBackend *m_backend = nullptr;

    int m_id = InvalidId;
};

}

#endif // HAVINGID_HPP
