#ifndef INFORMATIONPOPUP_H
#define INFORMATIONPOPUP_H

#include <QWidget>

namespace Ui {
class InformationPopup;
}

class InformationPopup : public QWidget
{
    Q_OBJECT

public:
    explicit InformationPopup(QWidget *parent = nullptr);
    ~InformationPopup();

private:
    Ui::InformationPopup *ui;
};

#endif // INFORMATIONPOPUP_H
