#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>

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
    QGridLayout *_board_layout;


};
#endif // MAINWINDOW_H
