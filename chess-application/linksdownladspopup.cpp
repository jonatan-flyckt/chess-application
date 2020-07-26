#include "linksdownladspopup.h"
#include "ui_linksdownladspopup.h"

LinksDownladsPopup::LinksDownladsPopup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LinksDownladsPopup)
{
    ui->setupUi(this);
}

LinksDownladsPopup::~LinksDownladsPopup()
{
    delete ui;
}
