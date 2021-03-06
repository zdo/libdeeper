#ifndef NOTESTATE_HPP
#define NOTESTATE_HPP

#include "./Helpers/BackendEntity.hpp"
#include <QString>

namespace deeper {

class NoteState : public BackendEntity
{
public:
    QString title() const;
    void setTitle(const QString &title);

    QString color() const;
    void setColor(const QString &color);

    bool isFinished() const;
    void setIsFinished(bool isFinished);

    int orderIndex() const;
    void setOrderIndex(int orderIndex);

private:
    QString m_title;
    QString m_color;
    int m_orderIndex = 0;
    bool m_isFinished = false;
};

} // namespace deeper

#endif // NOTESTATE_HPP
