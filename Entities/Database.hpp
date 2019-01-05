#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "Category.hpp"
#include "NoteState.hpp"
#include "Tag.hpp"
#include "Note.hpp"
#include "Goal.hpp"

#include "../Storage/AbstractStorage.hpp"
#include <QSharedPointer>

namespace deeper {

class Database : public QObject
{
    Q_OBJECT

public:
    Database(const QUrl &url);
    Database(QSharedPointer<AbstractStorage> storage);

    void switchAndSaveToStorage(QSharedPointer<AbstractStorage> storage);

    QVector<QSharedPointer<Category>> categoriesTree();
    QVector<QSharedPointer<Category>> childrenCategoriesOf(int parentId);
    QVector<QSharedPointer<Tag>> tags();

    QSharedPointer<Category> createCategory(const QString &parentId = Category::InvalidId);
    QSharedPointer<Category> categoryWithId(const QString &id);
    void saveCategoryTree();
    QSharedPointer<Category> parentOfCategory(const QSharedPointer<Category> &category);
    void deleteCategory(const QSharedPointer<Category> &category);
    bool setCategoryParent(const QSharedPointer<Category> &category, const QSharedPointer<Category> &parentCategory, int index = -1);

public slots:
    void refresh(bool sync=false);

signals:
    void onRefresh();

private:
    QSharedPointer<AbstractStorage> m_storage;

    QVector<QSharedPointer<Category>> m_categoriesTree;
    QMap<QString, QSharedPointer<Category>> m_categoryPerId;

    QVector<QSharedPointer<Tag>> m_tags;
    QVector<QSharedPointer<NoteState>> m_noteStates;
    QVector<QSharedPointer<Goal>> m_goals;
};

} // namespace deeper

#endif // DATABASE_HPP
