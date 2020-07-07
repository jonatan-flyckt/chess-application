#ifndef SQUAREWIDGET_H
#define SQUAREWIDGET_H

#include <QLabel>
#include <QDebug>

class SquareWidget : public QLabel
{
Q_OBJECT
public:
    SquareWidget(QString id, QPixmap pixmap, QString denotation);

    void populateWithPixmap();

    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

signals:
    void signalInfoMessage(QString message);

    void signalCurrentHovered(QString id);

private:
    QString _id;
    QPixmap _square_pixmap;
    QString _denotation;
};

#endif // SQUAREWIDGET_H
