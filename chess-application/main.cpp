#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QLoggingCategory::setFilterRules(
        "chess.movetimes.debug=true\n"
        "chess.evaluations.debug=true\n"
        "chess.mouse.debug=false\n"
        "chess.themes.debug=false\n"
        );

    MainWindow w;
    w.show();
    return a.exec();
}
