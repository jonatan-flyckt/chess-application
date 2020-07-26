#include "contactpopup.h"
#include "ui_contactpopup.h"

ContactPopup::ContactPopup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContactPopup)
{
    ui->setupUi(this);
}

ContactPopup::~ContactPopup()
{
    delete ui;
}
