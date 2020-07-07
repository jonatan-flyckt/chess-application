#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define BOARD_GRID_COL 1
#define BOARD_GRID_ROW 1

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QSizePolicy>
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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initiateUIComponents();
    void initiateBoardSquaresUI();

private:
    Ui::MainWindow *_ui;
    QGridLayout *_board_grid_layout;
    QGridLayout *_main_grid_layout;
    BoardAspectRatioWidget *_board_aspect_ratio_widget;


};
#endif // MAINWINDOW_H


