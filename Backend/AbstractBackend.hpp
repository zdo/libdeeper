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
    virtual ~AbstractBackend();

    // Categories.
    virtual QList<QSharedPointer<Category>> rootCategories() = 0;
    virtual QList<QSharedPointer<Category>> childrenCategories(const QSharedPointer<Category> &parent) = 0;
    virtual int childrenCategoriesCount(const QSharedPointer<Category> &parent) = 0;
    virtual QSharedPointer<Category> createCategory(const QSharedPointer<Category> &parent = nullptr) = 0;
    virtual QSharedPointer<Category> category(int id) = 0;
    virtual void saveCategory(const QSharedPointer<Category> &category) = 0;
    virtual void deleteCategory(const QSharedPointer<Category> &category) = 0;

    virtual QList<QSharedPointer<Tag>> categoryTags(const QSharedPointer<Category> &category) = 0;
    virtual void addCategoryTag(const QSharedPointer<Category> &category, const QSharedPointer<Tag> &tag) = 0;
    virtual void removeCategoryTag(const QSharedPointer<Category> &category, const QSharedPointer<Tag> &tag) = 0;
};

} // namespace deeper

#endif // ABSTRACTBACKEND_HPP
