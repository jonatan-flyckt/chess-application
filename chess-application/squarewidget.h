#ifndef SQUAREWIDGET_H
#define SQUAREWIDGET_H

#include <QLabel>
#include <QDebug>

class SquareWidget : public QLabel
{
public:
    SquareWidget(QString id, QPixmap pixmap, QString denotation);

    void populateWithPixmap();

    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private:
    QString _id;
    QPixmap _square_pixmap;
    QString _denotation;
};

#endif // SQUAREWIDGET_H
