#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define BOARD_GRID_COL 1
#define BOARD_GRID_ROW 1
#define SMALLEST_BOARD_SIZE 500

#define LEFT_LAYOUT_COL 0
#define RIGHT_LAYOUT_COL 2
#define TOP_LAYOUT_ROW 0

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QDebug>
#include <boardaspectratiowidget.h>
#include <graphicsinfo.h>
#include <QPixmap>
#include <QDesktopWidget>
#include <squarewidget.h>
#include <QVector>
#include "math.h"
#include <QPushButton>
#include "piecewidget.h"
#include <QMouseEvent>
#include <QPair>
#include <chessgame.h>
#include <QClipboard>
#include <QMessageBox>
#include <QSizePolicy>
#include <QGroupBox>
#include <QScrollArea>
#include <QComboBox>
#include "notationwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initiateUIComponents();
    void initiateBoardSquaresUI();
    void connectSquareToSignals(SquareWidget *square);

    void addCoordinateWidgets();
    void clearBoardUI();
    void addPieceGraphically(QPixmap pieceGraphic, QString squareID, QString denotation);
    void initiatePiecesGraphically();
    void removePieceGraphically(PieceWidget *piece);

    void highlightLegalSquares(QString originSquare);
    void removeLegalSquaresHighlight();

    QPixmap setPixmapFromType(QString type);

    void updateFontSizes();

    void resizeEvent(QResizeEvent* event);

    bool completeMove(QString destinationSquare);
    void restartGame(Colour colour);

    void moveRookForCastlingGraphically(Move move);
    void performPawnPromotionGraphically(Move move);
    void removeCapturedPieceGraphically(Move move);
    void removeEnPassantCapturedPieceGraphically(Move move);
    void highlightPreviousMove();
    void removeHighlightPreviousMove();
    void highlightCurrentMovingFromSquare(QString highlightSquare);
    void removeHighlightCurrentMovingFromSquare(QString highlightSquare);
    void highlightCheck();
    void doNotHightlightCheck();

    void promotedPawnSelection();
    void setLeftLayout();
    void setRightLayout();
    void setTopLayout();
    void addNotationWidgetForMove(State *resultingState);
signals:
    void on_set_white_button_clicked();

    void on_set_black_button_clicked();

public slots:
    void setInfoMessage(QString message);

    void setCurrentHovered(QString id);

    void startClickingMove(QString originSquare);

    void completeClickingMove(QString destinationSquare);

    void startDraggingMove(QString originSquare);

    void setDraggingMoveReadyToComplete();

    void completeDraggingMove();

    bool sendClickingMoveStatus();

    bool sendDraggingMoveStatus();

    bool sendDraggingMoveReadyToCompleteStatus();

    QString sendHoveredSquare();

    QString sendMoveOriginSquare();

    void movePieceWidget(QPoint mousePos);

    void setPlayerWhite();

    void setPlayerBlack();

    bool mouseIsInsideBoard();

    void copyFENToClipboard();

    QVector<SquareWidget*> sendSquareWidgets();

    void notationWidgetClicked(State *state);

private:
    Ui::MainWindow *_ui;
    QGridLayout *_board_grid_layout;
    QGridLayout *_main_grid_layout;
    BoardAspectRatioWidget *_board_aspect_ratio_widget;
    QVBoxLayout *_left_vertical_layout;
    QVBoxLayout *_right_vertical_layout;
    QHBoxLayout *_top_horizontal_layout;

    GraphicsInfo _graphics_info;
    bool _user_is_white;
    ChessGame *_game;
    bool _in_exploration_mode;

    QLabel *_info_label;
    QLabel *_fen_label;
    QPushButton *_set_white_button;
    QPushButton *_set_black_button;
    QPushButton *_copy_fen;
    QPushButton *_contact_button;
    QPushButton *_about_button;
    QHBoxLayout *_play_as_horizontal_layout;
    QPushButton *_new_game_button;
    QPushButton *_resign_game_button;
    QScrollArea *_algebraic_notation_scroll_area;
    QVBoxLayout *_algebraic_notation_vertical_layout;
    QVector<QHBoxLayout*> _algebraic_notation_horizontal_layouts;
    QVector<NotationWidget*> _notation_widgets;
    QComboBox *_difficulty_combo_box;
    QComboBox *_theme_combo_box;
    QPushButton *_export_pgn_notation_button;
    QPushButton *_links_and_download_button;
    QHBoxLayout *_fen_horizontal_layout;

    QVector<SquareWidget*> _square_widgets;
    QVector<QLabel*> _board_header_labels;
    QVector<PieceWidget*> _piece_widgets;

    QString _currently_hovered_square;
    bool _clicking_move_in_progress;
    bool _dragging_move_in_progress;
    bool _dragging_move_ready_to_complete;
    bool _move_was_promotion;
    Move _promotion_move;
    QString _move_in_progress_origin_square;
    QVector<Move> _legal_moves_for_current_state;
    QVector<QString> _legal_destination_squares_for_origin_square;
    PieceWidget *_piece_widget_currently_dragged;
    QPixmap _pixmap_of_dragged_piece;
    PieceWidget *_piece_widget_of_moved_from_square;

    QVector<QString> colsFromIndex{"a", "b", "c", "d", "e", "f", "g", "h"};
};
#endif // MAINWINDOW_H


