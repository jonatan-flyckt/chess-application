#ifndef SQUAREWIDGET_H
#define SQUAREWIDGET_H

#include <QLabel>
#include <QDebug>
#include <QBoxLayout>
#include <QCursor>

class SquareWidget : public QLabel
{
Q_OBJECT
public:
    SquareWidget(QString id, QPixmap pixmap, QString denotation);

    void populateWithPixmap();

    void enterEvent(QEnterEvent  *event) override;
    void leaveEvent(QEvent *event) override;

    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;

    void mouseReleaseEvent(QMouseEvent *ev) override;

    void changePixmap(QPixmap newPixmap);

    bool stayedInSameSquareDuringDraggingMove();

    QString id() const;
    void setId(const QString &id);

    QBoxLayout *inner_layout() const;
    void setInner_layout(QBoxLayout *inner_layout);

    QBoxLayout *getInner_layout() const;

    QString getDenotation() const;
    void setDenotation(const QString &denotation);

    QPixmap getSquare_pixmap() const;
    void setSquare_pixmap(const QPixmap &square_pixmap);

    void setSquareScaleFactor(int size);
    int getSquare_scale_factor() const;

signals:
    void signalInfoMessage(QString message);

    void signalCurrentHovered(QString id);

    void signalStartClickingMove(QString originSquare);

    void signalCompleteClickingMove(QString destinationSquare);

    void signalStartDraggingMove(QString originSquare);

    void signalDraggingMoveReadyToComplete();

    void signalCompleteDraggingMove();

    void signalMovePieceWidget(QPoint mousePos);

    bool getClickingMoveStatus();

    bool getDraggingMoveStatus();

    bool getDraggingMoveReadyToCompleteStatus();

    QString getHoveredSquare();

    QString getMoveOriginSquare();

    bool signalMouseIsInsideBoard();

private:
    QString _id;
    QPixmap _square_pixmap;
    QString _denotation;
    int _square_scale_factor;

    QBoxLayout *_inner_layout;
};

#endif // SQUAREWIDGET_H
