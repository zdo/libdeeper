#include "Database.hpp"
#include "Storage/StorageFactory.hpp"

#include <QFutureWatcher>
#include <QtConcurrent>

namespace deeper {

template <typename T>
static QVector<QSharedPointer<T>> fromArrayToSharedPointers(const QJsonArray &src) {
    QVector<QSharedPointer<T>> result;
    for (const QJsonValue &v : src) {
        QSharedPointer<T> t = QSharedPointer<T>::create();
        t->deserializeFromJson(v.toObject());
        result.append(t);
    }
    return result;
}

Database::Database(const QUrl &url)
{
    m_storage = StorageFactory::createStorage(url);
    Q_ASSERT(m_storage != nullptr);
}

Database::Database(QSharedPointer<AbstractStorage> storage)
{
    m_storage = storage;
    Q_ASSERT(m_storage != nullptr);
}

void Database::switchAndOverwriteStorage(QSharedPointer<AbstractStorage> storage)
{
    m_storage = storage;

    m_storage->clearAllData();

    for (auto c : m_categoryPerId) {
        this->saveCategory(c);
    }
}

QVector<QSharedPointer<Category>> Database::rootCategories()
{
    return HavingParent::children(m_categoryPerId, QSharedPointer<Category>());
}

QSharedPointer<Category> Database::categoryWithId(const QString &id)
{
    return m_categoryPerId.value(id);
}

QSharedPointer<Category> Database::parentOfCategory(const QSharedPointer<Category> &category)
{
    if (category->hasParent()) {
        return this->categoryWithId(category->parentId());
    } else {
        return nullptr;
    }
}

QVector<QSharedPointer<Category> > Database::childrenOfCategory(const QSharedPointer<Category> &category)
{
    return HavingParent::children(m_categoryPerId, category);
}

void Database::deleteCategory(const QSharedPointer<Category> &category)
{
    HavingParent::remove(m_categoryPerId, category);
    m_categoryPerId.remove(category->id());
    m_storage->deleteCategory(category->id());
}

QVector<QSharedPointer<Tag>> Database::tags()
{
    QVector<QSharedPointer<Tag>> result;
    for (auto t : m_tags) {
        result.append(t);
    }
    return result;
}

QSharedPointer<Category> Database::createCategory(const QSharedPointer<Category> &parent)
{
    auto newCategory = QSharedPointer<Category>::create();
    newCategory->generateRandomId();
    newCategory->setTitle(QObject::tr("New category"));

    m_categoryPerId[newCategory->id()] = newCategory;
    HavingParent::insert(m_categoryPerId, newCategory, parent, -1);

    this->saveCategory(newCategory);

    return newCategory;
}

void Database::saveCategory(const QSharedPointer<Category> &category)
{
    m_storage->saveCategory(category->serializeToJson());
}

void Database::refresh(bool sync)
{
    auto fn = [=]() {
        auto baseInfo = m_storage->getBaseInfo();
        auto categories = fromArrayToSharedPointers<Category>(baseInfo.categories);

        m_categoryPerId.clear();
        for (auto c : categories) {
            m_categoryPerId[c->id()] = c;
        }

        m_tags = fromArrayToSharedPointers<Tag>(baseInfo.tags);
        m_noteStates = fromArrayToSharedPointers<NoteState>(baseInfo.noteStates);
        m_goals = fromArrayToSharedPointers<Goal>(baseInfo.goals);

        emit this->onRefresh();
    };

    if (sync) {
        fn();
    } else {
        QtConcurrent::run([=]() {
            fn();
        });
    }
}

QVector<QSharedPointer<Note> > Database::notesSync(const QSharedPointer<Category> &category, const QSharedPointer<Note> &parentNote)
{
    QVector<QSharedPointer<Note>> result;

    if (m_categoryNoteGot.value(category->id())) {
        for (auto n : m_notePerId) {
            if (n->categoryId() != category->id()) {
                continue;
            }
            if ((parentNote.isNull() && n->parentId() == Note::InvalidId)
                    || (n->parentId() == parentNote->id())) {
                result.append(n);
            }
        }
    } else {
        auto notesJson = m_storage->notes(category->id(), parentNote.isNull() ? Note::InvalidId : parentNote->id());
        for (auto noteJson : notesJson) {
            auto note = QSharedPointer<Note>::create();
            note->deserializeFromJson(noteJson.toObject());
            result.append(note);

            m_notePerId[note->id()] = note;
        }

        m_categoryNoteGot[category->id()] = true;
    }

    HavingParent::sortByOrderIndex(result);

    return result;
}

bool Database::setCategoryParent(const QSharedPointer<Category> &category,
                                 const QSharedPointer<Category> &parentCategory,
                                 int index)
{
    auto oldParent = this->parentOfCategory(category);
    bool v = HavingParent::insert(m_categoryPerId, category, parentCategory, index);

    this->saveCategory(category);
    for (auto oldSibling : this->childrenOfCategory(oldParent)) {
        this->saveCategory(oldSibling);
    }
    for (auto newSibling : this->childrenOfCategory(parentCategory)) {
        this->saveCategory(newSibling);
    }

    return v;
}

QFuture<QVector<QSharedPointer<Note>>> Database::notes(const QSharedPointer<Category> &category,
                                                       const QSharedPointer<Note> &parentNote)
{
    return QtConcurrent::run([=]() {
        return this->notesSync(category, parentNote);
    });
}

QSharedPointer<Note> Database::createNote(const QSharedPointer<Category> &category, const QSharedPointer<Note> &parentNote)
{
    auto newNote = QSharedPointer<Note>::create();
    newNote->generateRandomId();
    newNote->setTitle(QObject::tr("New Note"));
    newNote->setCategoryId(category->id());
    newNote->setCreationTime(QDateTime::currentDateTime());

    int notesCount = this->notes(category, parentNote).result().count();
    newNote->setOrderIndex(notesCount);

    if (!parentNote.isNull()) {
        newNote->setParentId(parentNote->id());
    }

    m_notePerId[newNote->id()] = newNote;

    return newNote;
}

void Database::saveNote(const QSharedPointer<Note> &note)
{
    m_storage->saveNote(note->serializeToJson());
}

void Database::deleteNote(const QSharedPointer<Note> &note)
{
    m_storage->deleteNote(note->id());
}

bool Database::setNoteParent(const QSharedPointer<Note> &note, const QSharedPointer<Category> &category,
                             const QSharedPointer<Note> &parentNote, int index)
{
    QMap<QString, QSharedPointer<Note>> notesToSave;

    notesToSave[note->id()] = note;
    if (!parentNote.isNull()) {
        notesToSave[parentNote->id()] = parentNote;
    }

    if (note->categoryId() != category->id()) {
        // Remove from previous category.
        QMap<QString, QSharedPointer<Note>> tmpNotes;
        for (auto n : m_notePerId) {
            if (n->categoryId() == note->categoryId()) {
                tmpNotes[n->id()] = n;
                notesToSave[n->id()] = n;
            }
        }
        HavingParent::remove(tmpNotes, note);
    }

    // Add to new category.
    note->setCategoryId(category->id());

    QMap<QString, QSharedPointer<Note>> tmpNotes;
    for (auto n : m_notePerId) {
        if (n->categoryId() == category->id()) {
            tmpNotes[n->id()] = n;
            notesToSave[n->id()] = n;
        }
    }
    bool v = HavingParent::insert(tmpNotes, note, parentNote, index);

    for (auto n : notesToSave) {
        m_storage->saveNote(n->serializeToJson());
    }

    return v;
}

} // namespace deeper
