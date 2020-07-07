#ifndef SQUAREWIDGET_H
#define SQUAREWIDGET_H

#include <QLabel>
#include <QDebug>
#include <QBoxLayout>

class SquareWidget : public QLabel
{
Q_OBJECT
public:
    SquareWidget(QString id, QPixmap pixmap, QString denotation);

    void populateWithPixmap();

    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

    QString id() const;
    void setId(const QString &id);

    QBoxLayout *inner_layout() const;
    void setInner_layout(QBoxLayout *inner_layout);

signals:
    void signalInfoMessage(QString message);

    void signalCurrentHovered(QString id);

private:
    QString _id;
    QPixmap _square_pixmap;
    QString _denotation;
    QBoxLayout *_inner_layout;
};

#endif // SQUAREWIDGET_H
