#include "paipu.h"

#include <cassert>
#include <sstream>

static const std::regex re_angang_or_jiagang{ R"(^[mpsz]\d\d\d\d$|[\+\=\-]\d$)" }; // 暗槓もしくは加槓

PaipuReplay::PaipuReplay(const Game::Paipu& paipu) : _paipu{ paipu }, _game{ paipu.rule, false }, _round{ 0 }, _ply{ 0 }, _skip{ false } {
    _game.kaiju();
    qipai();
}

void PaipuReplay::qipai() {
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

void PaipuReplay::next() {
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
                if (_paipu.rounds[_round].moves[_ply].msg != Game::Message::HULE || _paipu.rounds[_round].moves[_ply].arg.empty()) {
                    // 槍槓以外
                    _game.next();
                }
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

std::ostream& operator<<(std::ostream& os, const Game::Model& model) {
    os << model.qijia << " "
        << model.zhuangfeng << " "
        << model.jushu << " "
        << model.changbang << " "
        << model.lizhibang << " "
        << model.defen << " "
        << model.shan << " "
        << model.shoupai << " "
        << model.he << " "
        << model.player_id << " "
        << model.lunban;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Game::Reply& reply) {
    os << (int)reply.msg << reply.arg;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Game::Paipu::Round& round) {
    os << round.model << " ";
    for (size_t i = 0; i < round.moves.size(); i++) {
        if (i > 0)
            os << ",";
        os << round.moves[i];
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Game::Paipu& paipu) {
    os << paipu.rule << " ";
    for (size_t i = 0; i < paipu.rounds.size(); i++) {
        if (i > 0)
            os << " ";
        os << paipu.rounds[i];
    }
    os << "\n";
    return os;
}
void load_model(std::istream& is, Game::Model& model, const Rule& rule) {
    is >> model.qijia
        >> model.zhuangfeng
        >> model.jushu
        >> model.changbang
        >> model.lizhibang
        >> model.defen;
    {
        std::string paistr;
        is >> paistr;
        model.shan.set(paistr, rule);
        if (is.get() != ' ')
            throw std::runtime_error("unexpected char");
    }
    for (size_t i = 0; i < 4; i++) {
        std::string paistr;
        std::getline(is, paistr, ' ');
        model.shoupai[i].set(paistr);
    }
    for (size_t i = 0; i < 4; i++) {
        std::string paistr;
        std::getline(is, paistr, ' ');
        model.he[i].set(paistr);
    }
    is >> model.player_id
        >> model.lunban;
}

void load_round(std::istream& is, Game::Paipu::Round& round, const Rule& rule) {
    load_model(is, round.model, rule);
    std::string movesstr;
    is >> movesstr;
    std::istringstream ss(movesstr);
    std::string move;
    while (std::getline(ss, move, ',')) {
        round.moves.emplace_back(Game::Reply{ (Game::Message)to_int(move[0]), move.substr(1) });
    }
}

std::istream& operator>>(std::istream& is, Game::Paipu& paipu) {
    std::string line;
    if (!std::getline(is, line))
        return is;

    std::istringstream ss{ line };
    ss >> paipu.rule;
    while (ss.peek(), !ss.eof()) {
        auto& round = paipu.rounds.emplace_back();
        load_round(ss, round, paipu.rule);
    }
    return is;
}
