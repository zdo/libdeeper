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
    std::sort(m_noteStates.begin(), m_noteStates.end(), [=](const QSharedPointer<NoteState> &n1, const QSharedPointer<NoteState> &n2) {
        if (n1->isFinished() == n2->isFinished()) {
            return n1->orderIndex() < n2->orderIndex();
        } else {
            return n1->isFinished() ? false : true;
        }
    });

    m_goals = fromArrayToSharedPointers<Goal>(baseInfo.goals);
}

void Database::clear()
{
    m_categories = QSharedPointer<HavingParentTree<Category>>::create();
    m_tags.clear();
    m_noteStates.clear();
    m_goals.clear();

    m_notesPerCategoryId.clear();
    m_notes.clear();
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

            m_notes[note->id()] = note;
        }

        tree = QSharedPointer<HavingParentTree<Note>>::create();
        tree->clearAndFillWith(notes);

        m_notesPerCategoryId[category->id()] = tree;
    }

    return tree;
}

QSharedPointer<Note> Database::note(const QString &id)
{
    QSharedPointer<Note> result = m_notes.value(id);
    if (result.isNull()) {
        try {
            QJsonObject noteJson = m_storage->note(id);
            result = QSharedPointer<Note>::create();
            result->deserializeFromJson(noteJson);
            m_notes[id] = result;
        } catch (const std::exception &exc) {
            qCritical() << "Can't obtain note with id" << id << ":" << exc.what();
        }
    }
    return result;
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
    m_notes.remove(note->id());

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

QSharedPointer<TimeTrack> Database::activeTimeTrack()
{
    auto ttJson = m_storage->activeTimeTrack();
    if (ttJson.isEmpty()) {
        return nullptr;
    }

    auto tt = this->obtainAndUpdateTimeTrack(ttJson);
    return tt;
}

QList<QSharedPointer<TimeTrack>> Database::timeTracksForNote(const QSharedPointer<Note> &note)
{
    QList<QSharedPointer<TimeTrack>> result;

    auto timeTracksJson = m_storage->timeTracksForNote(note->id());
    for (auto timeTrackRaw : timeTracksJson) {
        auto tt = this->obtainAndUpdateTimeTrack(timeTrackRaw);
        result.append(tt);
    }

    return result;
}

QSharedPointer<TimeTrack> Database::startTimeTrack(const QSharedPointer<Note> &note)
{
    auto activeTT = this->activeTimeTrack();
    if (!activeTT.isNull()) {
        activeTT->setEnd(QDateTime::currentDateTime());
        this->saveTimeTrack(activeTT);
    }

    auto newTT = QSharedPointer<TimeTrack>::create();
    newTT->generateRandomId();
    newTT->setNoteId(note->id());
    newTT->setStart(QDateTime::currentDateTime());
    this->saveTimeTrack(newTT);

    m_timeTracks[newTT->id()] = newTT;

    return newTT;
}

void Database::saveTimeTrack(const QSharedPointer<TimeTrack> &tt)
{
    m_storage->saveTimeTrack(tt->serializeToJson());
}

void Database::deleteTimeTrack(const QSharedPointer<TimeTrack> &tt)
{
    m_storage->deleteTimeTrack(tt->id());
    m_timeTracks.remove(tt->id());
}

const QList<QSharedPointer<NoteState>> & Database::noteStates() const
{
    return m_noteStates;
}

QSharedPointer<TimeTrack> Database::obtainAndUpdateTimeTrack(const QJsonValue &jsonRaw)
{
    QJsonObject json = jsonRaw.toObject();
    QSharedPointer<TimeTrack> tt = m_timeTracks.value(json["id"].toString());
    if (tt.isNull()) {
        tt = QSharedPointer<TimeTrack>::create();
        tt->deserializeFromJson(json);
        m_timeTracks[tt->id()] = tt;
    } else {
        tt->deserializeFromJson(json);
    }
    return tt;
}

} // namespace deeper
