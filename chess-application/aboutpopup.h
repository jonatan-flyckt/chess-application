#ifndef ABOUTPOPUP_H
#define ABOUTPOPUP_H

#include <QWidget>

namespace Ui {
class AboutPopup;
}

class AboutPopup : public QWidget
{
    Q_OBJECT

public:
    explicit AboutPopup(QWidget *parent = nullptr);
    ~AboutPopup();

private slots:
    void on_close_button_clicked();

private:
    Ui::AboutPopup *ui;
};

#endif // ABOUTPOPUP_H
