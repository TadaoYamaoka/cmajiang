#pragma once

#include "game.h"

class PaipuPalyer {
public:
    PaipuPalyer(const Game::Paipu& paipu);

    void next();

    const Game& game() const { return _game; }
    int round() const { return _round; }
    int ply() const { return _ply; }
    Game::Status status() const { return _game.status(); }

private:
    const Game::Paipu& _paipu;
    Game _game;
    int _round;
    int _ply;
    bool _skip;

    void qipai();
};
