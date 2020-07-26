#ifndef LINKSDOWNLADSPOPUP_H
#define LINKSDOWNLADSPOPUP_H

#include <QWidget>

namespace Ui {
class LinksDownladsPopup;
}

class LinksDownladsPopup : public QWidget
{
    Q_OBJECT

public:
    explicit LinksDownladsPopup(QWidget *parent = nullptr);
    ~LinksDownladsPopup();

private:
    Ui::LinksDownladsPopup *ui;
};

#endif // LINKSDOWNLADSPOPUP_H
