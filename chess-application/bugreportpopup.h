#ifndef BUGREPORTPOPUP_H
#define BUGREPORTPOPUP_H

#include <QWidget>

namespace Ui {
class BugReportPopup;
}

class BugReportPopup : public QWidget
{
    Q_OBJECT

public:
    explicit BugReportPopup(QWidget *parent = nullptr);
    ~BugReportPopup();

private:
    Ui::BugReportPopup *ui;
};

#endif // BUGREPORTPOPUP_H
