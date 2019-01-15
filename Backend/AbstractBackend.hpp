#ifndef ABSTRACTBACKEND_HPP
#define ABSTRACTBACKEND_HPP

#include "../Entities/Category.hpp"
#include "../Entities/Tag.hpp"
#include "../Entities/Goal.hpp"
#include "../Entities/Milestone.hpp"
#include "../Entities/Note.hpp"
#include "../Entities/NoteState.hpp"
#include "../Entities/TimeTrack.hpp"

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

    // Tags.
    virtual QSharedPointer<Tag> createTag(const QString &title) = 0;
    virtual QList<QSharedPointer<Tag>> tags() = 0;
    virtual QSharedPointer<Tag> tagWithId(int id) = 0;
    virtual void saveTag(int id) = 0;
    virtual void removeTag(int id) = 0;

    // Categories + tags.
    virtual QList<QSharedPointer<Tag>> tagsForCategory(int categoryId) = 0;
    virtual void assignTagToCategory(int categoryId, int tagId) = 0;
    virtual void removeTagFromCategory(int categoryId, int tagId) = 0;

    // Notes.
    virtual QList<QSharedPointer<Note>> rootNotesForCategory(int categoryId) = 0;
    virtual QList<QSharedPointer<Note>> noteChildren(int categoryId, int parentNoteId) = 0;
    virtual int noteChildrenCount(int categoryId, int parentNoteId) = 0;
    virtual QSharedPointer<Note> createNote(int parentCategoryId, int parentNoteId = BackendEntity::InvalidId) = 0;
    virtual QSharedPointer<Note> noteWithId(int id) = 0;
    virtual void saveNote(int id) = 0;
    virtual void removeNote(int id) = 0;
    virtual void moveNote(int id, int newParentCategory, int newParentNoteId, int index = -1) = 0;

};

} // namespace deeper

#endif // ABSTRACTBACKEND_HPP
