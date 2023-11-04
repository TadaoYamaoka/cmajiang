#include "paipu.h"

#include <cassert>

static const std::regex re_angang_or_jiagang{ R"(^[mpsz]\d\d\d\d$|[\+\=\-]\d$)" }; // 暗槓もしくは加槓

PaipuPalyer::PaipuPalyer(const Game::Paipu& paipu) : _paipu{ paipu }, _game{ paipu.rule, false }, _round{ 0 }, _ply{ 0 }, _skip{ false } {
    qipai();
}

void PaipuPalyer::qipai() {
    _game.set(_paipu.rounds[_round]);
    if (_game.status() == Game::Status::QIPAI) {
        _game.next();
    }
    else if (
        _paipu.rounds[_round].moves[_ply].msg == Game::Message::DAPAI ||
        _paipu.rounds[_round].moves[_ply].msg == Game::Message::GANG && std::regex_search(_paipu.rounds[_round].moves[_ply].arg, re_angang_or_jiagang) ||
        _paipu.rounds[_round].moves[_ply].msg == Game::Message::HULE && _paipu.rounds[_round].moves[_ply].arg.empty()
        )
        _skip = true;
}

void PaipuPalyer::next() {
    if (_skip) {
        _skip = false;
        _game.next();
        return;
    }
    if (_game.status() == Game::Status::HULE || _game.status() == Game::Status::PINGJU) {
        _game.next();
        if (_game.status() == Game::Status::JIEJI)
            return;
    }
    if (_game.status() == Game::Status::QIPAI) {
        _round++;
        _ply = 0;
        qipai();
        return;
    }
    if (_game.status() != Game::Status::HULE) {
        const auto& move = _paipu.rounds[_round].moves[_ply++];
        switch (move.msg)
        {
        case Game::Message::DAPAI:
            assert(_game.status() == Game::Status::ZIMO || _game.status() == Game::Status::GANGZIMO || _game.status() == Game::Status::FULOU);
            _game.reply(_game.lunban_player_id(), move.msg, move.arg);
            _game.next();
            assert(_game.he_(_game.lunban()).pai().back() == move.arg);
            if (_ply < _paipu.rounds[_round].moves.size() && (
                _paipu.rounds[_round].moves[_ply].msg == Game::Message::DAPAI ||
                _paipu.rounds[_round].moves[_ply].msg == Game::Message::GANG && std::regex_search(_paipu.rounds[_round].moves[_ply].arg, re_angang_or_jiagang) ||
                _paipu.rounds[_round].moves[_ply].msg == Game::Message::HULE && _paipu.rounds[_round].moves[_ply].arg.empty()
                ))
                _skip = true;
            break;
        case Game::Message::FULOU:
        {
            assert(_game.status() == Game::Status::DAPAI);
            std::smatch match;
            std::regex_search(move.arg, match, re_menqian);
            const char d = match.str()[0];
            const int l = (_game.lunban() + (d == '+' ? 3 : d == '=' ? 2 : 1)) % 4;
            _game.reply(_game.player_id()[l], move.msg, move.arg);
            _game.next();
            assert(_game.shoupai_(l).fulou_().back() == move.arg);
            break;
        }
        case Game::Message::GANG:
            if (std::regex_search(move.arg, re_angang_or_jiagang)) {
                // 暗槓もしくは加槓
                assert(_game.status() == Game::Status::ZIMO || _game.status() == Game::Status::GANGZIMO);
                _game.reply(_game.lunban_player_id(), move.msg, move.arg);
                _game.next();
                assert(find(_game.shoupai_(_game.lunban()).fulou_(), move.arg));
                _game.next();
            }
            else {
                assert(_game.status() == Game::Status::DAPAI);
                std::smatch match;
                std::regex_search(move.arg, match, re_menqian);
                const char d = match.str()[0];
                const int l = (_game.lunban() + (d == '+' ? 3 : d == '=' ? 2 : 1)) % 4;
                _game.reply(_game.player_id()[l], move.msg, move.arg);
                _game.next();
                assert(_game.shoupai_(l).fulou_().back() == move.arg);
            }
            break;
        case Game::Message::HULE:
            if (move.arg.empty()) {
                // 自摸
                assert(_game.status() == Game::Status::ZIMO || _game.status() == Game::Status::GANGZIMO);
                _game.reply(_game.lunban_player_id(), move.msg);
            }
            else {
                auto move_ = move;
                while (true) {
                    // ロン
                    assert(_game.status() == Game::Status::DAPAI || _game.status() == Game::Status::GANG);
                    const char d = move_.arg.back();
                    const int l = (_game.lunban() + (d == '+' ? 3 : d == '=' ? 2 : 1)) % 4;
                    _game.reply(_game.player_id()[l], move_.msg);
                    if (_ply < _paipu.rounds[_round].moves.size() && _paipu.rounds[_round].moves[_ply].msg == Game::Message::HULE) {
                        // 同時和了
                        move_ = _paipu.rounds[_round].moves[_ply++];
                        continue;
                    }
                    break;
                }
            }
            _game.next();
            assert(_game.status() == Game::Status::HULE);
            break;
        }
        if (_ply == _paipu.rounds[_round].moves.size() && _game.status() != Game::Status::HULE) {
            // 流局
            _game.next();
        }
    }
}
