#pragma once

#include "game.h"

class PaipuReplay {
public:
    PaipuReplay(const Game::Paipu& paipu);

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

std::ostream& operator<<(std::ostream& os, const Game::Paipu& paipu);
std::istream& operator>>(std::istream& is, Game::Paipu& paipu);
