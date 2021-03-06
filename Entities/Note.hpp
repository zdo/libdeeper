#ifndef NOTE_HPP
#define NOTE_HPP

#include "Tag.hpp"
#include "NoteState.hpp"
#include "TimeTrack.hpp"
#include "Milestone.hpp"
#include "./Helpers/BackendFamilyEntity.hpp"

#include <QList>
#include <QDateTime>
#include <QSharedPointer>

namespace deeper {

class Category;

class Note : public BackendFamilyEntity
{
public:
    struct DateTimeWithDuration {
        QDateTime dateTime;
        int durationSeconds = 0;
    };

    QString title() const;
    void setTitle(const QString &title);

    QString text() const;
    void setText(const QString &text);

    int categoryId() const;
    void setCategoryId(const int &categoryId);

    QDateTime creationTime() const;
    void setCreationTime(const QDateTime &creationTime);

    QDateTime deadlineTime() const;
    void setDeadlineTime(const QDateTime &deadlineTime);

    DateTimeWithDuration scheduledTime() const;
    void setScheduledTime(const DateTimeWithDuration &scheduledTime);

    bool isArchived() const;
    void setIsArchived(bool isArchived);

    void save() override;
    void remove();
    void move(const QSharedPointer<Category> &newCategory, const QSharedPointer<Note> &newParentNote, int index = -1);

    QSharedPointer<Category> category() const;
    QSharedPointer<Note> parent() const;
    QList<QSharedPointer<Note>> children() const;

    QSharedPointer<Note> createChildNote();

private:
    QDateTime m_creationTime;
    bool m_isArchived = false;
    int m_categoryId = InvalidId;

    QString m_title, m_text;
    int m_stateId = InvalidId;

    DateTimeWithDuration m_scheduledTime;
    QDateTime m_deadlineTime;
};

} // namespace deeper

#endif // NOTE_HPP
