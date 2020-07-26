#ifndef NEWGAMEPOPUP_H
#define NEWGAMEPOPUP_H

#include <QWidget>
#include "graphicsinfo.h"
#include "chessstructs.h"
#include <QDebug>

namespace Ui {
class NewGamePopup;
}

class NewGamePopup : public QWidget
{
    Q_OBJECT

public:
    explicit NewGamePopup(QWidget *parent = nullptr);
    void showEvent(QShowEvent *event);
    ~NewGamePopup();

private slots:
    void on_cancel_button_clicked();

    void on_white_button_clicked();

    void on_black_button_clicked();

    void on_name_line_edit_textChanged(const QString &arg1);

    void on_difficulty_combo_box_currentTextChanged(const QString &arg1);

    void on_start_game_button_clicked();

signals:

    void startNewGame(Colour colour, QString difficulty, QString name);

private:

    Ui::NewGamePopup *_ui;

    GraphicsInfo _graphics_info;

    Colour _selected_colour;

    QString _name;

    QString _difficulty;

};

#endif // NEWGAMEPOPUP_H
