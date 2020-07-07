#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define BOARD_GRID_COL 1
#define BOARD_GRID_ROW 1

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QDebug>
#include <boardaspectratiowidget.h>
#include <graphicsinfo.h>
#include <QPixmap>
#include <QDesktopWidget>

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
    GraphicsInfo _graphics_info;


};
#endif // MAINWINDOW_H


