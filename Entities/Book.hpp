#ifndef DATABASE_HPP
#define DATABASE_HPP

#include "Category.hpp"
#include "NoteState.hpp"
#include "Tag.hpp"
#include "Note.hpp"
#include "Goal.hpp"

#include "../Overview/Filter.hpp"
#include "../Backend/AbstractBackend.hpp"
#include <QSharedPointer>

namespace deeper {

class Book
{
public:
    Book(const QString &path);

    // Tags.
    QList<QSharedPointer<Tag>> tags();

    // Categories.
    QList<QSharedPointer<Category>> rootCategories();
    QSharedPointer<Category> createCategory(const QSharedPointer<Category> &parent = nullptr);
    void saveCategory(const QSharedPointer<Category> &category);
    void deleteCategory(const QSharedPointer<Category> &category);
    bool setCategoryParent(const QSharedPointer<Category> &category,
                           const QSharedPointer<Category> &parentCategory,
                           int index = -1);

    // Notes.
    QList<QSharedPointer<Note>> rootNotes(const QSharedPointer<Category> &category,
                                          const QSharedPointer<Filter> &filter = nullptr);
    QSharedPointer<Note> note(const QString &id);
    QSharedPointer<Note> createNote(const QSharedPointer<Category> &category,
                                    const QSharedPointer<Note> &parentNote = nullptr);
    void saveNote(const QSharedPointer<Note> &note);
    void deleteNote(const QSharedPointer<Note> &note);
    bool setNoteParent(const QSharedPointer<Note> &note, const QSharedPointer<Category> &category,
                       const QSharedPointer<Note> &parentNote, int index = -1);

    // Track time.
    QSharedPointer<TimeTrack> activeTimeTrack();
    QList<QSharedPointer<TimeTrack>> timeTracksForNote(const QSharedPointer<Note> &note);
    QSharedPointer<TimeTrack> startTimeTrack(const QSharedPointer<Note> &note);
    void saveTimeTrack(const QSharedPointer<TimeTrack> &tt);
    void deleteTimeTrack(const QSharedPointer<TimeTrack> &tt);
    TimeTrack::Duration noteTotalDuration(const QSharedPointer<Note> &note);

    // Note states.
    const QList<QSharedPointer<NoteState>> & noteStates() const;

private:
    QSharedPointer<AbstractBackend> m_storage;
};

} // namespace deeper

#endif // DATABASE_HPP
