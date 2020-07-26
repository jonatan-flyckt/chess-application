#ifndef CONTACTPOPUP_H
#define CONTACTPOPUP_H

#include <QWidget>

namespace Ui {
class ContactPopup;
}

class ContactPopup : public QWidget
{
    Q_OBJECT

public:
    explicit ContactPopup(QWidget *parent = nullptr);
    ~ContactPopup();

private:
    Ui::ContactPopup *ui;
};

#endif // CONTACTPOPUP_H
