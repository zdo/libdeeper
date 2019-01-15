#ifndef ABSTRACTBACKEND_HPP
#define ABSTRACTBACKEND_HPP

#include "../Entities/Category.hpp"
#include "../Entities/Tag.hpp"

#include <QUrl>

namespace deeper {

// Backend SHOULD NOT use QFuture<> for any operation.
// If the operation will be blocked - it's ok.
// UI layer is responsible for an async processing.
class AbstractBackend
{
public:
    virtual ~AbstractBackend() {}

    // Categories.
    virtual QList<QSharedPointer<Category>> rootCategories() = 0;
    virtual QList<QSharedPointer<Category>> categoryChildren(int parentId = BackendEntity::InvalidId) = 0;
    virtual int categoryChildrenCount(int parentId = BackendEntity::InvalidId) = 0;
    virtual QSharedPointer<Category> createCategory(int parentId = BackendEntity::InvalidId) = 0;
    virtual QSharedPointer<Category> categoryWithId(int id) = 0;
    virtual void saveCategory(int id) = 0;
    virtual void removeCategory(int id) = 0;
    virtual void moveCategory(int id, int newParentId, int index = -1) = 0;
};

} // namespace deeper

#endif // ABSTRACTBACKEND_HPP
