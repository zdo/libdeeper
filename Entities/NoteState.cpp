#include "NoteState.hpp"

namespace deeper {

QString NoteState::title() const
{
    return m_title;
}

void NoteState::setTitle(const QString &title)
{
    m_title = title;
}

QString NoteState::color() const
{
    return m_color;
}

void NoteState::setColor(const QString &color)
{
    m_color = color;
}

bool NoteState::isFinished() const
{
    return m_isFinished;
}

void NoteState::setIsFinished(bool isFinished)
{
    m_isFinished = isFinished;
}

int NoteState::orderIndex() const
{
    return m_orderIndex;
}

void NoteState::setOrderIndex(int orderIndex)
{
    m_orderIndex = orderIndex;
}

} // namespace deeper
