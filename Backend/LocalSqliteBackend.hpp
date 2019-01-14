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
    const QString & path() const { return m_path; }

    // Categories.
    QList<QSharedPointer<Category>> rootCategories() override;
    QList<QSharedPointer<Category>> childrenCategories(const QSharedPointer<Category> &parent) override;
    int childrenCategoriesCount(const QSharedPointer<Category> &parent) override;
    QSharedPointer<Category> createCategory(const QSharedPointer<Category> &parent = nullptr) override;
    QSharedPointer<Category> category(int id) override;
    void saveCategory(const QSharedPointer<Category> &category) override;
    void deleteCategory(const QSharedPointer<Category> &category) override;

//    QList<QSharedPointer<Tag>> categoryTags(const QSharedPointer<Category> &category) override;
//    void addCategoryTag(const QSharedPointer<Category> &category, const QSharedPointer<Tag> &tag) override;
//    void removeCategoryTag(const QSharedPointer<Category> &category, const QSharedPointer<Tag> &tag) override;


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
};

} // namespace deeper

#endif // LOCALSQLITEBACKEND_HPP
