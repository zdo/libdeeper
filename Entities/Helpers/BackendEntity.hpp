#ifndef HAVINGID_HPP
#define HAVINGID_HPP

namespace deeper {

class AbstractBackend;

class BackendEntity {
public:
    static const int InvalidId;

    virtual ~BackendEntity() {}

    const int & id() const { return m_id; }
    void setId(int id);

    virtual void save() = 0;
    void __setBackend(AbstractBackend *backend);

protected:
    friend class AbstractBackend;

    AbstractBackend * getBackendOrError() const;

private:
    AbstractBackend *m_backend = nullptr;

    int m_id = InvalidId;
};

}

#endif // HAVINGID_HPP
