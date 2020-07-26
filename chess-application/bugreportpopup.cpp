#include "bugreportpopup.h"
#include "ui_bugreportpopup.h"

BugReportPopup::BugReportPopup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BugReportPopup)
{
    ui->setupUi(this);
}

BugReportPopup::~BugReportPopup()
{
    delete ui;
}
