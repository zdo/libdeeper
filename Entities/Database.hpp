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

    // Storage.
    void switchAndOverwriteStorage(QSharedPointer<AbstractStorage> storage);

    // Tags.
    QVector<QSharedPointer<Tag>> tags();

    // Categories.
    QVector<QSharedPointer<Category>> rootCategories();
    QSharedPointer<Category> categoryWithId(const QString &id);
    QSharedPointer<Category> parentOfCategory(const QSharedPointer<Category> &category);
    QVector<QSharedPointer<Category>> childrenOfCategory(const QSharedPointer<Category> &category);

    QSharedPointer<Category> createCategory(const QSharedPointer<Category> &parent = nullptr);
    void saveCategory(const QSharedPointer<Category> &category);
    void deleteCategory(const QSharedPointer<Category> &category);
    bool setCategoryParent(const QSharedPointer<Category> &category, const QSharedPointer<Category> &parentCategory, int index = -1);

    // Notes.
    QFuture<QVector<QSharedPointer<Note>>> notes(const QSharedPointer<Category> &category,
                                                 const QSharedPointer<Note> &parentNote = nullptr);
    QVector<QSharedPointer<Note>> notesSync(const QSharedPointer<Category> &category,
                                            const QSharedPointer<Note> &parentNote = nullptr);
    QSharedPointer<Note> createNote(const QSharedPointer<Category> &category,
                                    const QSharedPointer<Note> &parentNote = nullptr);
    void saveNote(const QSharedPointer<Note> &note);
    void deleteNote(const QSharedPointer<Note> &note);
    bool setNoteParent(const QSharedPointer<Note> &note, const QSharedPointer<Category> &category,
                       const QSharedPointer<Note> &parentNote, int index = -1);

public slots:
    void refresh(bool sync=false);

signals:
    void onRefresh();

private:
    QSharedPointer<AbstractStorage> m_storage;

    QMap<QString, QSharedPointer<Category>> m_categoryPerId;
    QVector<QSharedPointer<Tag>> m_tags;
    QVector<QSharedPointer<NoteState>> m_noteStates;
    QVector<QSharedPointer<Goal>> m_goals;

    QMap<QString, bool> m_categoryNoteGot;
    QMap<QString, QSharedPointer<Note>> m_notePerId;
};

} // namespace deeper

#endif // DATABASE_HPP
