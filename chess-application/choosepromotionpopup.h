#ifndef CHOOSEPROMOTIONPOPUP_H
#define CHOOSEPROMOTIONPOPUP_H

#include <QWidget>
#include <QDialog>
#include <chessstructs.h>
#include <graphicsinfo.h>

namespace Ui {
class ChoosePromotionPopup;
}

class ChoosePromotionPopup : public QDialog//QWidget
{
    Q_OBJECT

public:
    explicit ChoosePromotionPopup(QWidget *parent = nullptr, Colour colour = White);
    ~ChoosePromotionPopup();

    void showEvent(QShowEvent *event);

signals:

    void promotionSelected(PieceType type);

private slots:
    void on_queen_button_clicked();

    void on_rook_button_clicked();

    void on_bishop_button_clicked();

    void on_knight_button_clicked();

private:
    Ui::ChoosePromotionPopup *_ui;

    GraphicsInfo _graphics_info;

    Colour _colour;
};

#endif // CHOOSEPROMOTIONPOPUP_H
