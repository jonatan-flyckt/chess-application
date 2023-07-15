#ifndef ENGINETHREAD_H
#define ENGINETHREAD_H

#include "chessengine.h"
#include <chessgame.h>
#include <QThread>


//The class below is a thread for searching for the best move to ensure the UI doesn't freeze
class EngineThread : public QThread {
    Q_OBJECT

public:
    EngineThread(){
        qRegisterMetaType<Move>("Move");
    }

    void run() override {
        Move move = _engine->selectMoveFromState(_game->getCurrent_state(), _user_is_white ? Black : White);
        emit moveCalculationsFinished(move);
    }

    void setGame(ChessGame *game){_game = game;}
    void setEngine(ChessEngine *engine){_engine = engine;}
    void setUserIsWhite(bool userIsWhite){_user_is_white = userIsWhite;}

signals:
    void moveCalculationsFinished(Move move);

private:

    ChessGame *_game;
    ChessEngine *_engine;
    bool _user_is_white;
};

#endif // ENGINETHREAD_H
