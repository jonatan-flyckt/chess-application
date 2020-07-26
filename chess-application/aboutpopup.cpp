#include "aboutpopup.h"
#include "ui_aboutpopup.h"

AboutPopup::AboutPopup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutPopup){
    ui->setupUi(this);
}

AboutPopup::~AboutPopup(){
    delete ui;
}

void AboutPopup::on_close_button_clicked(){
    this->close();
}
