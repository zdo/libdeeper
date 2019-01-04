#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "Category.hpp"
#include "NoteState.hpp"
#include "Tag.hpp"
#include "Note.hpp"
#include "Goal.hpp"

#include "Storage/AbstractStorage.hpp"
#include <QSharedPointer>

namespace deeper {

class Database : public QObject
{
    Q_OBJECT

public:
    Database(const QUrl &url);

    QVector<QSharedPointer<Category>> rootCategories();
    QVector<QSharedPointer<Tag>> tags();

    QSharedPointer<Category> categoryWithId(int id);
    void saveCategory(const QSharedPointer<Category> &category);
    void deleteCategory(int id);

public slots:
    void refresh();

signals:
    void onRefresh();

private:
    QSharedPointer<AbstractStorage> m_storage;

    QMap<int, QSharedPointer<Category>> m_categories;
    QMap<int, QSharedPointer<Tag>> m_tags;
    QMap<int, QSharedPointer<NoteState>> m_noteStates;
    QMap<int, QSharedPointer<Goal>> m_goals;
};

} // namespace deeper

#endif // DATABASE_HPP
