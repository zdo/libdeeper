#include "Database.hpp"
#include "Storage/StorageFactory.hpp"

namespace deeper {

template <typename T>
static QList<QSharedPointer<T>> fromArrayToSharedPointers(const QJsonArray &src) {
    QList<QSharedPointer<T>> result;
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

    for (auto c : m_categories->allObjects()) {
        this->saveCategory(c);
    }
}

void Database::deleteCategory(const QSharedPointer<Category> &category)
{
    QList<QSharedPointer<Category>> affected;
    QList<QSharedPointer<Category>> deepChildren = m_categories->allDeepChildren(category);
    m_categories->remove(category, &affected);

    m_storage->deleteCategory(category->id());
    for (auto c : affected) {
        m_storage->saveCategory(c->serializeToJson());
    }

    for (auto c : deepChildren) {
        m_storage->deleteCategory(c->id());
    }
}

QList<QSharedPointer<Tag>> Database::tags()
{
    QList<QSharedPointer<Tag>> result;
    for (auto t : m_tags) {
        result.append(t);
    }
    return result;
}

QSharedPointer<HavingParentTree<Category>> Database::categories()
{
    return m_categories;
}

QSharedPointer<Category> Database::createCategory(const QSharedPointer<Category> &parent)
{
    auto newCategory = QSharedPointer<Category>::create();
    newCategory->generateRandomId();
    newCategory->setTitle(QObject::tr("New category"));

    QList<QSharedPointer<Category>> affected;
    m_categories->move(newCategory, parent, -1, &affected);
    for (auto c : affected) {
        m_storage->saveCategory(c->serializeToJson());
    }

    return newCategory;
}

void Database::saveCategory(const QSharedPointer<Category> &category)
{
    m_storage->saveCategory(category->serializeToJson());
}

void Database::refresh()
{
    this->clear();

    auto baseInfo = m_storage->getBaseInfo();
    auto categories = fromArrayToSharedPointers<Category>(baseInfo.categories);

    m_categories->clearAndFillWith(categories);

    m_tags = fromArrayToSharedPointers<Tag>(baseInfo.tags);
    m_noteStates = fromArrayToSharedPointers<NoteState>(baseInfo.noteStates);
    m_goals = fromArrayToSharedPointers<Goal>(baseInfo.goals);
}

void Database::clear()
{
    m_categories = QSharedPointer<HavingParentTree<Category>>::create();
    m_tags.clear();
    m_noteStates.clear();
    m_goals.clear();
    m_notesPerCategoryId.clear();
}

QSharedPointer<HavingParentTree<Note>> Database::notes(const QSharedPointer<Category> &category)
{
    QSharedPointer<HavingParentTree<Note>> tree = m_notesPerCategoryId.value(category->id());

    if (tree.isNull()) {
        auto notesJson = m_storage->notes(category->id());
        QList<QSharedPointer<Note>> notes;
        for (auto noteJson : notesJson) {
            auto note = QSharedPointer<Note>::create();
            note->deserializeFromJson(noteJson.toObject());
            notes.append(note);
        }

        tree = QSharedPointer<HavingParentTree<Note>>::create();
        tree->clearAndFillWith(notes);

        m_notesPerCategoryId[category->id()] = tree;
    }

    return tree;
}

bool Database::setCategoryParent(const QSharedPointer<Category> &category,
                                 const QSharedPointer<Category> &parentCategory,
                                 int index)
{
    QList<QSharedPointer<Category>> affected;
    bool v = m_categories->move(category, parentCategory, index, &affected);
    for (auto o : affected) {
        this->saveCategory(o);
    }
    return v;
}

QSharedPointer<Note> Database::createNote(const QSharedPointer<Category> &category, const QSharedPointer<Note> &parentNote)
{
    auto newNote = QSharedPointer<Note>::create();
    newNote->generateRandomId();
    newNote->setTitle(QObject::tr("New Note"));
    newNote->setCategoryId(category->id());
    newNote->setCreationTime(QDateTime::currentDateTime());

    int notesCount = this->notes(category)->children(parentNote).count();
    newNote->setOrderIndex(notesCount);

    if (!parentNote.isNull()) {
        newNote->setParentId(parentNote->id());
    }

    m_notesPerCategoryId[category->id()]->move(newNote, parentNote);
    this->saveNote(newNote);

    return newNote;
}

void Database::saveNote(const QSharedPointer<Note> &note)
{
    m_storage->saveNote(note->serializeToJson());
}

void Database::deleteNote(const QSharedPointer<Note> &note)
{
    QList<QSharedPointer<Note>> affected;
    QList<QSharedPointer<Note>> deepChildren = m_notesPerCategoryId[note->categoryId()]->allDeepChildren(note);
    m_notesPerCategoryId[note->categoryId()]->remove(note, &affected);

    for (auto v : affected) {
        this->saveNote(v);
    }

    m_storage->deleteNote(note->id());
    for (auto v : deepChildren) {
        m_storage->deleteNote(v->id());
    }
}

bool Database::setNoteParent(const QSharedPointer<Note> &note, const QSharedPointer<Category> &category,
                             const QSharedPointer<Note> &parentNote, int index)
{
    QList<QSharedPointer<Note>> affected;
    bool v = m_notesPerCategoryId[category->id()]->move(note, parentNote, index, &affected);
    for (auto n : affected) {
        m_storage->saveNote(n->serializeToJson());
    }

    return v;
}

const QList<QSharedPointer<NoteState>> & Database::noteStates() const
{
    return m_noteStates;
}

} // namespace deeper
