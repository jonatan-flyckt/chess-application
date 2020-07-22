#ifndef BOARDASPECTRATIOWIDGET_H
#define BOARDASPECTRATIOWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QResizeEvent>
#include <QDebug>
#include <squarewidget.h>

#include <QGuiApplication>
#include <QScreen>

class BoardAspectRatioWidget: public QWidget{
Q_OBJECT
public:
    BoardAspectRatioWidget(QGridLayout *boardLayout, float width, float height, QWidget *parent = 0);
    void resizeEvent(QResizeEvent *event);

    void mouseReleaseEvent(QMouseEvent *event){
    }

signals:
    QVector<SquareWidget*> getSquareWidgets();

private:
    QBoxLayout *layout;
    float arWidth; // aspect ratio width
    float arHeight; // aspect ratio height
};

#endif // BOARDASPECTRATIOWIDGET_H
