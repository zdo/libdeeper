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

    // Tags.
    QSharedPointer<Tag> createTag(const QString &title) override;
    QList<QSharedPointer<Tag>> tags() override;
    QSharedPointer<Tag> tagWithId(int id) override;
    void saveTag(int id) override;
    void removeTag(int id) override;

    // Categories + tags.
    QList<QSharedPointer<Tag>> tagsForCategory(int categoryId) override;
    void assignTagToCategory(int categoryId, int tagId) override;
    void removeTagFromCategory(int categoryId, int tagId) override;

    // Notes.
    QList<QSharedPointer<Note>> rootNotesForCategory(int categoryId) override;
    QList<QSharedPointer<Note>> noteChildren(int categoryId, int parentNoteId) override;
    int noteChildrenCount(int categoryId, int parentNoteId) override;
    QSharedPointer<Note> createNote(int parentCategoryId, int parentNoteId = BackendEntity::InvalidId) override;
    QSharedPointer<Note> noteWithId(int id) override;
    void saveNote(int id) override;
    void removeNote(int id) override;
    void moveNote(int id, int newParentCategory, int newParentNoteId, int index = -1) override;

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

    void bindValueOrNull(QSqlQuery &q, const QString &id, int value);
    void bindValueOrNull(QSqlQuery &q, const QString &id, BackendEntity *ptr);
    void bindValueOrNull(QSqlQuery &q, const QString &id, const QString &value);
    void bindValueOrNull(QSqlQuery &q, const QString &id, const QDateTime &value);
    QDateTime dateTimeFromQuery(QSqlQuery &q, const QString &id);

    QList<int> getAllDeepCategoryChildrenIds(int id);
    QList<int> getAllDeepNoteChildrenIds(int id);

    void applyMigrations();
    void execSqlFromFile(const QString &path);

    QSharedPointer<Category> createCategoryFromSql(QSqlQuery &q);
    QSharedPointer<Tag> createTagFromSql(QSqlQuery &q);
    QSharedPointer<Note> createNoteFromSql(QSqlQuery &q);
};

} // namespace deeper

#endif // LOCALSQLITEBACKEND_HPP
