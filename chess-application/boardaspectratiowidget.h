#ifndef BOARDASPECTRATIOWIDGET_H
#define BOARDASPECTRATIOWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QResizeEvent>

class BoardAspectRatioWidget: public QWidget{
public:
    BoardAspectRatioWidget(QGridLayout *boardLayout, float width, float height, QWidget *parent = 0);
    void resizeEvent(QResizeEvent *event);

private:
    QBoxLayout *layout;
    float arWidth; // aspect ratio width
    float arHeight; // aspect ratio height
};

#endif // BOARDASPECTRATIOWIDGET_H
