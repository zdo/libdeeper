#ifndef LOCALSQLITEBACKEND_HPP
#define LOCALSQLITEBACKEND_HPP

#include "AbstractBackend.hpp"

#include <QSqlDatabase>

#include "../Entities/Book.hpp"

namespace deeper {

class LocalSqliteBackend : public AbstractBackend
{
public:
    LocalSqliteBackend(const QString &path);
    ~LocalSqliteBackend() override;
    const QString & path() const { return m_path; }

    // Categories.
    QList<QSharedPointer<Category>> rootCategories() override;
    QList<QSharedPointer<Category>> categoryChildren(int parentId = BackendEntity::InvalidId) override;
    int categoryChildrenCount(int parentId = BackendEntity::InvalidId) override;
    QSharedPointer<Category> createCategory(int parentId = BackendEntity::InvalidId) override;
    QSharedPointer<Category> categoryWithId(int id) override;
    void saveCategory(int id) override;
    void removeCategory(int id) override;
    void moveCategory(int id, int newParentId, int index = -1) override;

//    QList<QSharedPointer<Tag>> categoryTags(Category *category) override;
//    void addCategoryTag(Category *category, const QSharedPointer<Tag> &tag) override;
//    void removeCategoryTag(Category *category, const QSharedPointer<Tag> &tag) override;


private:
    QString m_path;
    QSqlDatabase m_db;

    QMap<int, QSharedPointer<Category>> m_categories;
    QMap<int, QSharedPointer<Tag>> m_tags;
    QMap<int, QSharedPointer<NoteState>> m_noteStates;
    QMap<int, QSharedPointer<Goal>> m_goals;
    QMap<int, QSharedPointer<Note>> m_notes;
    QMap<int, QSharedPointer<TimeTrack>> m_timeTracks;

    QSqlQuery prepare(const QString &s);
    void exec(QSqlQuery &q);
    QSqlQuery exec(const QString &q);
    void doInTransaction(std::function<void ()> fn);

    void bindIdOrNull(QSqlQuery &q, const QString &id, int value);
    void bindEntityIdOrNull(QSqlQuery &q, const QString &id, BackendEntity *ptr);

    QList<int> getAllDeepCategoryChildrenIds(int id);

    void applyMigrations();
    void execSqlFromFile(const QString &path);
};

} // namespace deeper

#endif // LOCALSQLITEBACKEND_HPP
