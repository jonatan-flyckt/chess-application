#include "choosepromotionpopup.h"
#include "ui_choosepromotionpopup.h"

ChoosePromotionPopup::ChoosePromotionPopup(GraphicsInfo graphicsInfo, QWidget *parent, Colour colour) :
    QDialog(parent),
    _ui(new Ui::ChoosePromotionPopup)
{
    _graphics_info = graphicsInfo;
    _colour = colour;
    _ui->setupUi(this);
}

ChoosePromotionPopup::~ChoosePromotionPopup(){
    delete _ui;
}

void ChoosePromotionPopup::showEvent(QShowEvent *event){
    if (_colour == White){
        _ui->queen_button->setIcon(QIcon(_graphics_info._white_queen));
        _ui->queen_button->setIconSize(_ui->queen_button->size());
        _ui->rook_button->setIcon(QIcon(_graphics_info._white_rook));
        _ui->rook_button->setIconSize(_ui->rook_button->size());
        _ui->bishop_button->setIcon(QIcon(_graphics_info._white_bishop));
        _ui->bishop_button->setIconSize(_ui->bishop_button->size());
        _ui->knight_button->setIcon(QIcon(_graphics_info._white_knight));
        _ui->knight_button->setIconSize(_ui->knight_button->size());
    }
    else{
        _ui->queen_button->setIcon(QIcon(_graphics_info._black_queen));
        _ui->queen_button->setIconSize(_ui->queen_button->size());
        _ui->rook_button->setIcon(QIcon(_graphics_info._black_rook));
        _ui->rook_button->setIconSize(_ui->rook_button->size());
        _ui->bishop_button->setIcon(QIcon(_graphics_info._black_bishop));
        _ui->bishop_button->setIconSize(_ui->bishop_button->size());
        _ui->knight_button->setIcon(QIcon(_graphics_info._black_knight));
        _ui->knight_button->setIconSize(_ui->knight_button->size());
    }

    _ui->queen_button->setDown(false);
    _ui->rook_button->setDown(false);
    _ui->bishop_button->setDown(false);
    _ui->knight_button->setDown(false);
}

void ChoosePromotionPopup::on_queen_button_clicked(){
    emit promotionSelected(Queen);
    this->close();
}

void ChoosePromotionPopup::on_rook_button_clicked(){
    emit promotionSelected(Rook);
    this->close();
}

void ChoosePromotionPopup::on_bishop_button_clicked(){
    emit promotionSelected(Bishop);
    this->close();
}

void ChoosePromotionPopup::on_knight_button_clicked(){
    emit promotionSelected(Knight);
    this->close();
}
