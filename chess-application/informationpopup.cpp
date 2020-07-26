#include "informationpopup.h"
#include "ui_informationpopup.h"

InformationPopup::InformationPopup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InformationPopup)
{
    ui->setupUi(this);
}

InformationPopup::~InformationPopup()
{
    delete ui;
}
