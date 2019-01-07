#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "Category.hpp"
#include "NoteState.hpp"
#include "Tag.hpp"
#include "Note.hpp"
#include "Goal.hpp"
#include "HavingParentTree.hpp"

#include "../Storage/AbstractStorage.hpp"
#include <QSharedPointer>

namespace deeper {

class Database
{
public:
    Database(const QUrl &url);
    Database(QSharedPointer<AbstractStorage> storage);

    // Storage.
    void switchAndOverwriteStorage(QSharedPointer<AbstractStorage> storage);
    void refresh();

    // Tags.
    QList<QSharedPointer<Tag>> tags();

    // Categories.
    QSharedPointer<HavingParentTree<Category>> categories();

    QSharedPointer<Category> createCategory(const QSharedPointer<Category> &parent = nullptr);
    void saveCategory(const QSharedPointer<Category> &category);
    void deleteCategory(const QSharedPointer<Category> &category);
    bool setCategoryParent(const QSharedPointer<Category> &category, const QSharedPointer<Category> &parentCategory, int index = -1);

    // Notes.
    QSharedPointer<HavingParentTree<Note>> notes(const QSharedPointer<Category> &category);
    QSharedPointer<Note> createNote(const QSharedPointer<Category> &category,
                                    const QSharedPointer<Note> &parentNote = nullptr);
    void saveNote(const QSharedPointer<Note> &note);
    void deleteNote(const QSharedPointer<Note> &note);
    bool setNoteParent(const QSharedPointer<Note> &note, const QSharedPointer<Category> &category,
                       const QSharedPointer<Note> &parentNote, int index = -1);

    const QList<QSharedPointer<NoteState>> & noteStates() const;

private:
    QSharedPointer<AbstractStorage> m_storage;

    QSharedPointer<HavingParentTree<Category>> m_categories;
    QList<QSharedPointer<Tag>> m_tags;
    QList<QSharedPointer<NoteState>> m_noteStates;
    QList<QSharedPointer<Goal>> m_goals;

    QMap<QString, QSharedPointer<HavingParentTree<Note>>> m_notesPerCategoryId;

    void clear();
};

} // namespace deeper

#endif // DATABASE_HPP
