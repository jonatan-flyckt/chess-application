#include "newgamepopup.h"
#include "ui_newgamepopup.h"

NewGamePopup::NewGamePopup(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::NewGamePopup){
    _ui->setupUi(this);

    _ui->white_button->setIcon(QIcon(_graphics_info._white_king));
    _ui->white_button->setIconSize(_ui->white_button->size());
    _ui->black_button->setIcon(QIcon(_graphics_info._black_king));
    _ui->black_button->setIconSize(_ui->black_button->size());

    for ( int diffInt = Easy; diffInt != FakeLast; diffInt++ ){
        Difficulty difficulty = static_cast<Difficulty>(diffInt);
        _ui->difficulty_combo_box->addItem(QString::fromStdString(stringFromDifficulty(difficulty)));
        _ui->difficulty_combo_box->setCurrentText(QString::fromStdString(stringFromDifficulty(difficulty)));
    }

    _difficulty = Normal;
    _selected_colour = White;

}

void NewGamePopup::showEvent(QShowEvent *event){
    _ui->white_button->setDown(_selected_colour == White);
    _ui->black_button->setDown(_selected_colour == Black);
    _ui->name_line_edit->setText(_name);
}

NewGamePopup::~NewGamePopup(){
    delete _ui;
}

void NewGamePopup::on_cancel_button_clicked(){
    this->close();
}

void NewGamePopup::on_white_button_clicked(){
    _ui->white_button->setDown(true);
    _ui->black_button->setDown(false);
    _selected_colour = White;
}

void NewGamePopup::on_black_button_clicked(){
    _ui->white_button->setDown(false);
    _ui->black_button->setDown(true);
    _selected_colour = Black;
}

void NewGamePopup::on_name_line_edit_textChanged(const QString &arg1){
    _name = _ui->name_line_edit->text();
    _ui->white_button->setDown(_selected_colour == White);
    _ui->black_button->setDown(_selected_colour == Black);
}

void NewGamePopup::on_difficulty_combo_box_currentTextChanged(const QString &arg1){
    QString difficultyString = _ui->difficulty_combo_box->currentText();
    _difficulty = difficultyFromString(difficultyString.toStdString());

    _ui->white_button->setDown(_selected_colour == White);
    _ui->black_button->setDown(_selected_colour == Black);
}

void NewGamePopup::on_start_game_button_clicked(){
    emit startNewGame(_selected_colour, _difficulty, _name);
    this->close();
}

void NewGamePopup::on_name_line_edit_returnPressed(){
    emit startNewGame(_selected_colour, _difficulty, _name);
    this->close();
}
