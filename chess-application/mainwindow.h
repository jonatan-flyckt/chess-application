#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define BOARD_GRID_COL 1
#define BOARD_GRID_ROW 1
#define SMALLEST_BOARD_SIZE 500

#define HOW_OFTEN_TO_CHECK_FOR_PLAYER_MOVE 1000
#define HOW_OFTEN_TO_CHECK_FOR_BOARD_GRAPHIC_UPDATE 20

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
#include <QScrollBar>
#include <QComboBox>
#include "notationwidget.h"
#include <QDateTime>
#include <QFileDialog>
#include <QFile>
#include <QException>
#include "newgamepopup.h"
#include "choosepromotionpopup.h"
#include "aboutpopup.h"
#include "linksdownladspopup.h"
#include "bugreportpopup.h"
#include "contactpopup.h"
#include "utils.h"
#include "chessengine.h"
#include <QTimer>
#include <QtConcurrent/QtConcurrentRun>
#include <QFuture>

#include <future>

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

    bool completeMove(Move attemptedMove);


    void highlightPreviousMove(State *state);
    void removeHighlightPreviousMove();
    void highlightCurrentMovingFromSquare(QString highlightSquare);
    void removeHighlightCurrentMovingFromSquare(QString highlightSquare);
    void highlightCheck(State *state);
    void doNotHightlightCheck();

    void promotedPawnSelection();
    void setLeftLayout();
    void setRightLayout();
    void setTopLayout();
    void addNotationWidgetForMove(State *resultingState);
    void clearAlgebraicNotationView();
    void loadStateGraphically(State *state);
    void removeAllSquareHighlights();
    void addPiecesToBoardFromState(State *state);

    void endGame();

    void performEngineMove(Move move);
    void getEngineMove();
signals:
    void on_set_white_button_clicked();

    void on_set_black_button_clicked();

    void signalToReloadStateGraphically();

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

    bool mouseIsInsideBoard();

    void copyFENToClipboard();

    QVector<SquareWidget*> sendSquareWidgets();

    void notationWidgetClicked(State *state);

    void clearAllPiecesFromBoard();

    void exploreFirstState();

    void explorePreviousState();

    void exploreNextState();

    void exploreLastState();

    void exportPGNFile();

    void showNewGamePopup();

    void restartGame(Colour colour, Difficulty difficulty, QString name);

    void resignGame();

    void promotionSelected(PieceType type);

    void aboutPopup();

    void linksDownloadsPopup();

    void bugReportPopup();

    void contactPopup();

    void checkIfPlayerMadeMove();

    void updateBoardGraphicsAfterMove();

private:
    Ui::MainWindow *_ui;
    QGridLayout *_board_grid_layout;
    QGridLayout *_main_grid_layout;
    BoardAspectRatioWidget *_board_aspect_ratio_widget;
    QVBoxLayout *_left_vertical_layout;
    QVBoxLayout *_right_vertical_layout;
    QHBoxLayout *_top_horizontal_layout;
    NewGamePopup *_new_game_popup;
    ChoosePromotionPopup *_choose_promotion_popup;

    AboutPopup *_about_popup;
    LinksDownladsPopup *_links_downloads_popup;
    BugReportPopup *_bug_report_popup;
    ContactPopup *_contact_popup;

    GraphicsInfo _graphics_info;
    bool _user_is_white;
    ChessGame *_game;
    bool _in_exploration_mode;
    State *_state_being_viewed;
    ChessEngine *_engine;

    QLabel *_info_label;
    QLabel *_fen_label;
    QPushButton *_copy_fen;
    QPushButton *_contact_button;
    QPushButton *_about_button;
    QPushButton *_new_game_button;
    QPushButton *_resign_game_button;
    QScrollArea *_algebraic_notation_scroll_area;
    QVBoxLayout *_algebraic_notation_vertical_layout;
    QVector<QHBoxLayout*> _algebraic_notation_horizontal_layouts;
    QVector<NotationWidget*> _notation_widgets;
    QComboBox *_theme_combo_box;
    QPushButton *_export_pgn_notation_button;
    QPushButton *_links_and_download_button;
    QPushButton *_bug_report_button;
    QHBoxLayout *_fen_horizontal_layout;
    QHBoxLayout *_exploration_navigation_horizontal_layout;
    QPushButton *_explore_next_button;
    QPushButton *_explore_previous_button;
    QPushButton *_explore_first_button;
    QPushButton *_explore_last_button;
    QHBoxLayout *_colour_to_move_horizontal_layout;
    QLabel *_colour_to_move_label;
    QHBoxLayout *_playing_as_horizontal_layout;
    QLabel *_playing_as_colour_label;
    QLabel *_difficulty_label;

    PieceType _piece_selected_for_promotion;

    QVector<SquareWidget*> _square_widgets;
    QVector<QLabel*> _board_header_labels;
    QVector<PieceWidget*> _piece_widgets;

    QString _currently_hovered_square;
    bool _clicking_move_in_progress;
    bool _dragging_move_in_progress;
    bool _dragging_move_ready_to_complete;
    bool _move_was_promotion;

    bool _player_moved_against_engine;
    QTimer *_check_for_player_move_timer;

    bool _time_to_update_board;
    QTimer *_check_for_update_board_timer;

    QString _move_in_progress_origin_square;
    QVector<Move> _legal_moves_for_current_state;
    QVector<QString> _legal_destination_squares_for_origin_square;
    PieceWidget *_piece_widget_currently_dragged;
    QPixmap _pixmap_of_dragged_piece;
    PieceWidget *_piece_widget_of_moved_from_square;
};
#endif // MAINWINDOW_H


