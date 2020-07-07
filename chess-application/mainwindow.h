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
#include <squarewidget.h>
#include <QVector>
#include "math.h"
#include <QPushButton>

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

    void addColAndRowHeaders();
    void clearBoardUI();
    void addPieceGraphically();
    void initiatePiecesGraphically();

signals:
    void on_set_white_button_clicked();

    void on_set_black_button_clicked();

public slots:
    void setInfoMessage(QString message);

    void setCurrentHovered(QString id);

    void setPlayerWhite();

    void setPlayerBlack();

private:
    Ui::MainWindow *_ui;
    QGridLayout *_board_grid_layout;
    QGridLayout *_main_grid_layout;
    BoardAspectRatioWidget *_board_aspect_ratio_widget;
    GraphicsInfo _graphics_info;
    bool _user_is_white;

    QLabel *_info_label;
    QPushButton *_set_white_button;
    QPushButton *_set_black_button;
    QVector<SquareWidget*> _square_widgets;
    QVector<QLabel*> _board_header_labels;

    QString _currently_hovered_square;

    QVector<QString> colsFromIndex{"a", "b", "c", "d", "e", "f", "g", "h"};
};
#endif // MAINWINDOW_H


