#ifndef NOTATIONWIDGET_H
#define NOTATIONWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <chessstructs.h>
#include <QDebug>

class NotationWidget : public QPushButton
{
    Q_OBJECT
public:
    NotationWidget(State *state);

    void setToolTipText();

    void enterEvent(QEvent *e);

    void leaveEvent(QEvent *e);

    void mousePressEvent(QMouseEvent *e);

    QString _tooltip_text;
    State *state() const;

signals:
    void clickedSendState(State *state);

private:

    QString _move_notation;
    Move _move;
    State *_state;
};

#endif // NOTATIONWIDGET_H
