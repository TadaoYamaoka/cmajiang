#include "game.h"
#include "xiangting.h"

#include <cmath>
#include <numeric>
#include <sstream>

static const std::regex re_dapai{ R"(^z[1234])" };
static const std::regex re_fulou{ R"(^[mpsz]\d{4})" };
static const std::regex re_gangzimo{ R"(^[mpsz]\d{4}$)" };
static const std::regex re_pingju1{ R"(^z)" };
static const std::regex re_pingju2{ R"(^[mps][19])" };
static const std::regex re_reply_zimo{ R"(\*$)" };
static const std::regex re_reply_dapai1{ R"(^[mpsz](\d)\1\1\1)" };
static const std::regex re_reply_dapai2{ R"(^[mpsz](\d)\1\1)" };
static const std::regex re_reply_gang{ R"(^[mpsz]\d{4}$)" };
static const std::regex re_get_dapai{ R"(\d(?=[\+\=\-]))" };

static const std::vector<int> zipai_n = { 1, 2, 3, 4, 5, 6, 7 };
static const std::vector<int> yaojiu_n = { 1, 9 };

inline std::string match(const std::string& m, const std::regex& regex) {
    std::smatch match;
    std::regex_search(m, match, regex);
    return match.str();
}

template <typename T, typename F>
T filter(const T& v, F f) {
    T ret;
    std::copy_if(v.begin(), v.end(), std::back_inserter(ret), f);
    return ret;
}


Game::Game(const Rule& rule, const bool paipu) : _rule{ rule }, _paipu{}, _status{ Status::NONE } {
    if (paipu) {
        _paipu = std::make_unique<Paipu>();
        _paipu->rule = rule;
    }
}

void Game::set(const std::array<Shoupai, 4>& shoupai, const std::array<He, 4>& he, const Shan& shan, const int lunban) {
    _model.shan = shan;
    for (int l = 0; l < 4; l++) {
        _model.shoupai[l] = shoupai[l];
        _model.he[l] = he[l];
        _model.player_id[l] = (_model.qijia + _model.jushu + l) % 4;
    }
    _model.lunban = he[0].pai().size() == 0 ? -1 : lunban;

    _diyizimo = he[0].pai().size() <= 1
        && he[1].pai().size() <= 1
        && he[2].pai().size() <= 1
        && he[3].pai().size() == 0;

    _dapai = _model.lunban >= 0 && he[_model.lunban].pai().size() > 0 ? he[_model.lunban].pai().back() : "";
    _fulou.clear();
    _gang.clear();

    _lizhi = { shoupai[0].lizhi(), shoupai[1].lizhi(), shoupai[2].lizhi(), shoupai[3].lizhi() };
    _yifa = {
        he[0].pai().size() > 0 && he[0].pai().back().back() == '*',
        he[1].pai().size() > 0 && he[1].pai().back().back() == '*',
        he[2].pai().size() > 0 && he[2].pai().back().back() == '*',
        he[3].pai().size() > 0 && he[3].pai().back().back() == '*',
    };
    _n_gang = {
        (int)std::count_if(shoupai[0].fulou_().begin(), shoupai[0].fulou_().end(), [](const auto& m) { return std::regex_match(m, re_gang); }),
        (int)std::count_if(shoupai[1].fulou_().begin(), shoupai[1].fulou_().end(), [](const auto& m) { return std::regex_match(m, re_gang); }),
        (int)std::count_if(shoupai[2].fulou_().begin(), shoupai[2].fulou_().end(), [](const auto& m) { return std::regex_match(m, re_gang); }),
        (int)std::count_if(shoupai[3].fulou_().begin(), shoupai[3].fulou_().end(), [](const auto& m) { return std::regex_match(m, re_gang); }),
    };
    _neng_rong = { true, true, true, true };

    _hule.clear();
    _hule_option = HuleOption::NONE;
    _no_game = false;
    _lianzhuang = false;
    _changbang = _model.changbang;
    _fenpei = { 0, 0, 0, 0 };

    _defen = {};
    _pingju = {};

    if (_paipu) {
        if (_status != Status::QIPAI)
            _paipu->rounds.emplace_back();
        auto& round = _paipu->rounds.back();
        round.model = _model;
    }

    if (_dapai.empty())
        _status = Status::QIPAI;
    else
        _status = Status::DAPAI;
}

void Game::set(const Paipu::Round& round) {
    set(round.model.shoupai, round.model.he, round.model.shan, round.model.lunban);
    _model.qijia = round.model.qijia;
    _model.zhuangfeng = round.model.zhuangfeng;
    _model.jushu = round.model.jushu;
    _model.changbang = round.model.changbang;
    _model.lizhibang = round.model.lizhibang;
    _model.defen = round.model.defen;
    _model.player_id = round.model.player_id;
}

void Game::call_players(const Status type) {
    _status = type;
    _reply = {};
}

void Game::reply(const int id, const Message msg, const std::string& arg) {
    _reply[id] = { msg, arg };
}

void Game::next() {
    switch (_status) {
    case Status::KAIJU:
        reply_kaiju();
        break;
    case Status::QIPAI:
        reply_qipai();
        break;
    case Status::ZIMO:
        if (get_reply(_model.lunban).msg == Message::NONE) return;
        reply_zimo();
        break;
    case Status::DAPAI:
        reply_dapai();
        break;
    case Status::FULOU:
        reply_fulou();
        break;
    case Status::GANG:
        reply_gang();
        break;
    case Status::GANGZIMO:
        reply_zimo();
        break;
    case Status::HULE:
        reply_hule();
        break;
    case Status::PINGJU:
        reply_pingju();
        break;
    default:
        throw std::runtime_error("unexpected status");
    }
}

// 开局
void Game::kaiju(const int qijia) {
    _model = {};
    _model.defen = { _rule.startingPoints, _rule.startingPoints, _rule.startingPoints, _rule.startingPoints };

    // 起家
    std::uniform_int_distribution<int> dist(0, 3);
    _model.qijia = (qijia < 0) ? dist(engine) : qijia;

    // 場数
    _max_jushu = _rule.roundsType == 0 ? 0 : _rule.roundsType * 4 - 1;

    if (_paipu)
        _paipu->rounds = {};

    call_players(Status::KAIJU);
}

// 起牌(配牌)
void Game::qipai_(const Shan& shan) {
    _model.shan = shan;
    for (int l = 0; l < 4; l++) {
        std::vector<std::string> qipai_;
        for (int i = 0; i < 13; i++) {
            qipai_.emplace_back(_model.shan.zimo());
        }
        _model.shoupai[l] = Shoupai(qipai_);
        _model.he[l] = He();
        _model.player_id[l] = (_model.qijia + _model.jushu + l) % 4;
    }
    _model.lunban = -1;

    _diyizimo = true;
    // 途中流局あり
    _fengpai = _rule.abortiveDraw;

    _dapai.clear();
    _fulou.clear();
    _gang.clear();

    _lizhi = { 0, 0, 0, 0 };
    _yifa = { false, false, false, false };
    _n_gang = { 0, 0, 0, 0 };
    _neng_rong = { true, true, true, true };

    _hule.clear();
    _hule_option = HuleOption::NONE;
    _no_game = false;
    _lianzhuang = false;
    _changbang = _model.changbang;
    _fenpei = { 0, 0, 0, 0};

    _defen = {};
    _rank = {};
    _point = {};
    _pingju = {};

    if (_paipu) {
        auto& round = _paipu->rounds.emplace_back();
        round.model = _model;
    }

    call_players(Status::QIPAI);
}

void Game::qipai() {
    qipai_(Shan(_rule));
}

// 自摸
void Game::zimo() {
    _model.lunban = (_model.lunban + 1) % 4;

    auto zimo = _model.shan.zimo();
    _model.shoupai[_model.lunban].zimo(zimo);

    call_players(Status::ZIMO);
}

// 打牌
void Game::dapai(const std::string& p) {
    _yifa[_model.lunban] = 0;

    if (!_model.shoupai[_model.lunban].lizhi())
        _neng_rong[_model.lunban] = true;

    _model.shoupai[_model.lunban].dapai(p);
    _model.he[_model.lunban].dapai(p);

    if (_diyizimo) {
        if (!std::regex_search(p, re_dapai)) _fengpai = false;
        if (!_dapai.empty() && _dapai.substr(0, 2) != p.substr(0, 2))
            _fengpai = false;
    }
    else _fengpai = false;

    if (p.back() == '*') {
        _lizhi[_model.lunban] = _diyizimo ? 2 : 1;
        _yifa[_model.lunban] = _rule.yifa;
    }

    if (xiangting(_model.shoupai[_model.lunban]) == 0) {
        if (find_if(tingpai(_model.shoupai[_model.lunban]), [this](const std::string& p) {
            return _model.he[_model.lunban].find(p);
        })) {
            _neng_rong[_model.lunban] = false;
        }
    }

    _dapai = p;

    if (!_gang.empty()) kaigang();

    if (_paipu)
        _paipu->rounds.back().moves.emplace_back(Reply{ Message::DAPAI, p });

    call_players(Status::DAPAI);
}

// 副露
void Game::fulou(const std::string& m) {
    _diyizimo = false;
    _yifa = { 0, 0, 0, 0 };

    _model.he[_model.lunban].fulou(m);

    const auto d = match(m, re_menqian);
    _model.lunban = (_model.lunban + (int)std::string("_-=+").find(d[0])) % 4;

    _model.shoupai[_model.lunban].fulou(m);

    if (std::regex_search(m, re_fulou)) {
        _gang = m;
        _n_gang[_model.lunban]++;
    }

    _fulou = m;

    if (_paipu)
        _paipu->rounds.back().moves.emplace_back(Reply{ Message::FULOU, m });

    call_players(Status::FULOU);
}

// 杠(槓)
void Game::gang(const std::string& m) {
    _model.shoupai[_model.lunban].gang(m);

    if (!_gang.empty()) kaigang();

    _gang = m;
    _n_gang[_model.lunban]++;

    if (_paipu)
        _paipu->rounds.back().moves.emplace_back(Reply{ Message::GANG, m });

    call_players(Status::GANG);
}

// 杠自摸(槓自摸)
void Game::gangzimo() {
    _diyizimo = false;
    _yifa = { 0, 0, 0, 0 };

    auto zimo = _model.shan.gangzimo();
    _model.shoupai[_model.lunban].zimo(zimo);

    if (!_rule.gangbaopaiPostAddition ||
        std::regex_search(_gang, re_gangzimo)) kaigang();

    call_players(Status::GANGZIMO);
}

// 开杠(開槓)
void Game::kaigang() {
    _gang.clear();

    // カンドラあり
    if (!_rule.gangbaopai) return;

    _model.shan.kaigang();
}

// 和了
void Game::hule() {
    if (_status != Status::HULE) {
        _model.shan.close();
        _hule_option = _status == Status::GANG ? HuleOption::QIANGGANG
            : _status == Status::GANGZIMO ? HuleOption::LINGSHANG
            : HuleOption::NONE;
    }

    auto menfeng = _hule.size() ? pop_front(_hule) : _model.lunban;
    auto rongpai = menfeng == _model.lunban ? std::string{}
        : (_hule_option == HuleOption::QIANGGANG
            ? _gang.substr(0, 1) + _gang.back()
            : _dapai.substr(0, 2)
            ) + "_+=-"[(4 + _model.lunban - menfeng) % 4];
    auto shoupai = _model.shoupai[menfeng];
    auto libaopai = shoupai.lizhi() ? _model.shan.libaopai() : std::vector<std::string>{};

    Param param = {
        _rule,
        _model.zhuangfeng,
        menfeng,
        Param::Hupai{
            _lizhi[menfeng],
            _yifa[menfeng],
            _hule_option == HuleOption::QIANGGANG,
            _hule_option == HuleOption::LINGSHANG,
            _model.shan.paishu() > 0 || _hule_option == HuleOption::LINGSHANG ? 0 : rongpai.empty() ? 1 : 2,
            !(_diyizimo && rongpai.empty()) ? 0 : menfeng == 0 ? 1 : 2
        },
        _model.shan.baopai(),
        libaopai,
        Param::Jicun{
            _model.changbang,
            _model.lizhibang
        }
    };
    _defen = ::hule(shoupai, rongpai, param);

    if (_rule.dealerContinuationType > 0 && menfeng == 0) _lianzhuang = true;
    if (_rule.roundsType == 0) _lianzhuang = false;
    _fenpei = _defen.fenpei;

    if (_paipu)
        _paipu->rounds.back().moves.emplace_back(Reply{ Game::Message::HULE, rongpai });

    call_players(Status::HULE);
}

// 流局
void Game::pingju(Pingju::Name name, std::array<std::string, 4> shoupai) {
    std::array<int, 4> fenpei = { 0, 0, 0, 0 };

    if (name == Pingju::NONE) {
        int n_tingpai = 0;
        for (int l = 0; l < 4; l++) {
            if (_rule.notenDeclaration/*ノーテン宣言あり*/ && shoupai[l].empty()
                && !_model.shoupai[l].lizhi()) continue;
            if (!_rule.notenPenalty/*ノーテン罰あり*/
                && (_rule.dealerContinuationType/*連荘方式*/ != 2 || l != 0)
                && !_model.shoupai[l].lizhi())
            {
                shoupai[l].clear();
            }
            else if (xiangting(_model.shoupai[l]) == 0
                && tingpai(_model.shoupai[l]).size() > 0)
            {
                n_tingpai++;
                shoupai[l] = _model.shoupai[l].toString();
                if (_rule.dealerContinuationType/*連荘方式*/ == 2 && l == 0)
                    _lianzhuang = true;
            }
            else {
                shoupai[l].clear();
            }
        }
        if (_rule.liujumanguan/*流し満貫あり*/) {
            for (int l = 0; l < 4; l++) {
                bool all_yaojiu = true;
                for (const auto& p : _model.he[l].pai()) {
                    if (std::regex_search(p, re_menqian_end)) { all_yaojiu = false; break; }
                    if (std::regex_search(p, re_pingju1)) continue;
                    if (std::regex_search(p, re_pingju2)) continue;
                    all_yaojiu = false; break;
                }
                if (all_yaojiu) {
                    name = Pingju::MANGUAN/*流し満貫*/;
                    for (int i = 0; i < 4; i++) {
                        fenpei[i] += l == 0 && i == l ? 12000
                            : l == 0 ? -4000
                            : l != 0 && i == l ? 8000
                            : l != 0 && i == 0 ? -4000
                            : -2000;
                    }
                }
            }
        }
        if (name == Pingju::NONE) {
            // 荒牌平局(通常の流局)
            name = Pingju::HUANGPAI;
            if (_rule.notenPenalty/*ノーテン罰あり*/
                && 0 < n_tingpai && n_tingpai < 4)
            {
                for (int l = 0; l < 4; l++) {
                    fenpei[l] = !shoupai[l].empty() ? 3000 / n_tingpai
                        : -3000 / (4 - n_tingpai);
                }
            }
        }
        if (_rule.dealerContinuationType/*連荘方式*/ == 3) _lianzhuang = true;
    }
    else {
        _no_game = true;
        _lianzhuang = true;
    }

    if (_rule.roundsType/*場数*/ == 0) _lianzhuang = true;

    _fenpei = fenpei;

    _pingju.name = name;
    _pingju.shoupai = shoupai;

    call_players(Status::PINGJU);
}

// 終了
void Game::last() {
    _model.lunban = -1;

    if (!_lianzhuang) {
        _model.jushu++;
        _model.zhuangfeng += (_model.jushu / 4) | 0;
        _model.jushu = _model.jushu % 4;
    }

    bool jieju_ = false;
    int guanjun = -1;
    const auto& defen = _model.defen;
    for (int i = 0; i < 4; i++) {
        int id = (_model.qijia + i) % 4;
        if (defen[id] < 0 && _rule.bankruptcyEndAll/*トビ終了あり*/) jieju_ = true;
        if (defen[id] >= 30000
            && (guanjun < 0 || defen[id] > defen[guanjun])) guanjun = id;
    }

    int sum_jushu = _model.zhuangfeng * 4 + _model.jushu;

    if (15 < sum_jushu)                                jieju_ = true;
    else if ((_rule.roundsType/*場数*/ + 1) * 4 - 1 < sum_jushu)  jieju_ = true;
    else if (_max_jushu < sum_jushu) {
        if (_rule.overtimeType/*延長戦方式*/ == 0)             jieju_ = true;
        else if (_rule.roundsType/*場数*/ == 0)                   jieju_ = true;
        else if (guanjun >= 0)                              jieju_ = true;
        else {
            _max_jushu += _rule.overtimeType/*延長戦方式*/ == 3 ? 4
                : _rule.overtimeType/*延長戦方式*/ == 2 ? 1
                : 0;
        }
    }
    else if (_max_jushu == sum_jushu) {
        if (_rule.lastRoundStop/*オーラス止めあり*/ && guanjun == _model.player_id[0]
            && _lianzhuang && !_no_game)         jieju_ = true;
    }

    if (jieju_) jieju();
    else       qipai();
}

// 结局(終局)
void Game::jieju() {
    std::vector<int> paiming;
    auto& defen = _model.defen;
    for (int i = 0; i < 4; i++) {
        int id = (_model.qijia + i) % 4;
        for (int j = 0; j < 4; j++) {
            if (j == paiming.size() || defen[id] > defen[paiming[j]]) {
                paiming.insert(paiming.begin() + j, id);
                break;
            }
        }
    }
    defen[paiming[0]] += _model.lizhibang * 1000;

    for (int i = 0; i < 4; i++) {
        _rank[paiming[i]] = i + 1;
    }

    for (int i = 1; i < 4; i++) {
        int id = paiming[i];
        _point[id] = (defen[id] - 30000) / 1000.0f
            + _rule.rankPoints/*順位点*/[i];
        if (_rule.roundRankPoints) _point[id] = std::round(_point[id]);
        _point[paiming[0]] -= _point[id];
    }

    call_players(Status::JIEJI);
}


// 応答取得
const Game::Reply& Game::get_reply(const int l) const {
    return _reply[_model.player_id[l]];
}

// 结局(終局)の応答
void Game::reply_kaiju() {
    qipai();
}

// 起牌(配牌)の応答
void Game::reply_qipai() {
    zimo();
}

// 自摸の応答
void Game::reply_zimo() {
    const auto& reply = get_reply(_model.lunban);
    if (reply.msg == Message::DAOPAI) {
        if (allow_pingju()) {
            std::array<std::string, 4> shoupai{};
            shoupai[_model.lunban] = _model.shoupai[_model.lunban].toString();
            return pingju(Pingju::YAO9/*九種九牌*/, shoupai);
        }
    }
    else if (reply.msg == Message::HULE) {
        if (allow_hule()) {
            return hule();
        }
    }
    else if (reply.msg == Message::GANG) {
        if (find(get_gang_mianzi(), reply.arg)) {
            return gang(reply.arg);
        }
    }
    else if (reply.msg == Message::DAPAI) {
        auto dapai_ = std::regex_replace(reply.arg, re_reply_zimo, "");
        if (find(get_dapai(), dapai_)) {
            if (reply.arg.back() == '*' && allow_lizhi(dapai_).first) {
                return dapai(reply.arg);
            }
            return dapai(dapai_);
        }
    }

    auto p = get_dapai().back();
    dapai(p);
}

// 打牌の応答
void Game::reply_dapai() {
    for (int i = 1; i < 4; i++) {
        int l = (_model.lunban + i) % 4;
        const auto& reply = get_reply(l);
        if (reply.msg == Message::HULE && allow_hule(l)) {
            if (_rule.maxSimultaneousWinners/*最大同時和了数*/ == 1 && _hule.size())
                continue;
            _hule.emplace_back(l);
        }
        else {
            auto shoupai = _model.shoupai[l];
            shoupai.zimo(_dapai);
            if (xiangting(shoupai) == -1)
                _neng_rong[l] = false;
        }
    }
    if (_hule.size() == 3 && _rule.maxSimultaneousWinners/*最大同時和了数*/ == 2) {
        std::array<std::string, 4> shoupai{};
        for (const auto l : _hule) {
            shoupai[l] = _model.shoupai[l].toString();
        }
        return pingju(Pingju::HULE3/*三家和了*/, shoupai);
    }
    else if (_hule.size()) {
        return hule();
    }

    if (_dapai.back() == '*') {
        _model.defen[_model.player_id[_model.lunban]] -= 1000;
        _model.lizhibang++;

        if (std::count_if(_lizhi.begin(), _lizhi.end(), [](const auto x) { return x > 0; }) == 4
            && _rule.abortiveDraw/*途中流局あり*/)
        {
            std::array<std::string, 4> shoupai;
            std::transform(_model.shoupai.begin(), _model.shoupai.end(), shoupai.begin(),
                [](const auto& shoupai_) { return shoupai_.toString(); });
            return pingju(Pingju::LIZHI4/*四家立直*/, shoupai);
        }
    }

    if (_diyizimo && _model.lunban == 3) {
        _diyizimo = false;
        if (_fengpai) {
            return pingju(Pingju::FENG4/*四風連打*/);
        }
    }

    if (std::accumulate(_n_gang.begin(), _n_gang.end(), 0) == 4) {
        if (_rule.abortiveDraw/*途中流局あり*/  && *std::max_element(_n_gang.begin(), _n_gang.end()) < 4) {
            return pingju(Pingju::GANG4/*四槓散了*/);
        }
    }

    if (!_model.shan.paishu()) {
        std::array<std::string, 4> shoupai{};
        for (int l = 0; l < 4; l++) {
            const auto& reply = get_reply(l);
            if (reply.msg == Message::DAOPAI) shoupai[l] = reply.arg;
        }
        return pingju(Pingju::NONE, shoupai);
    }

    for (int i = 1; i < 4; i++) {
        int l = (_model.lunban + i) % 4;
        const auto& reply = get_reply(l);
        if (reply.msg == Message::FULOU) {
            auto m = std::regex_replace(reply.arg, re_ling, "5");
            if (std::regex_search(m, re_reply_dapai1)) {
                if (find(get_gang_mianzi(l), reply.arg)) {
                    return fulou(reply.arg);
                }
            }
            else if (std::regex_search(m, re_reply_dapai2)) {
                if (find(get_peng_mianzi(l), reply.arg)) {
                    return fulou(reply.arg);
                }
            }
        }
    }
    int l = (_model.lunban + 1) % 4;
    const auto& reply = get_reply(l);
    if (reply.msg == Message::FULOU) {
        if (find(get_chi_mianzi(l), reply.arg)) {
            return fulou(reply.arg);
        }
    }

    zimo();
}

// 副露の応答
void Game::reply_fulou() {
    if (!_gang.empty()) {
        return gangzimo();
    }

    const auto& reply = get_reply(_model.lunban);
    if (reply.msg == Message::DAPAI) {
        if (find(get_dapai(), reply.arg)) {
            return dapai(reply.arg);
        }
    }

    const auto p = get_dapai().back();
    dapai(p);
}

// 杠(槓)の応答
void Game::reply_gang() {
    if (std::regex_match(_gang, re_reply_gang)) {
        return gangzimo();
    }

    for (int i = 1; i < 4; i++) {
        int l = (_model.lunban + i) % 4;
        const auto& reply = get_reply(l);
        if (reply.msg == Message::HULE && allow_hule(l)) {
            if (_rule.maxSimultaneousWinners/*最大同時和了数*/ == 1 && _hule.size())
                continue;
            _hule.emplace_back(l);
        }
        else {
            auto p = _gang.substr(0, 1) + _gang.back();
            auto shoupai = _model.shoupai[l];
            shoupai.zimo(p);
            if (xiangting(shoupai) == -1)
                _neng_rong[l] = false;
        }
    }
    if (_hule.size()) {
        return hule();
    }

    gangzimo();
}

// 和了の応答
void Game::reply_hule() {
    for (int l = 0; l < 4; l++) {
        _model.defen[_model.player_id[l]] += _fenpei[l];
    }
    _model.changbang = 0;
    _model.lizhibang = 0;

    if (_hule.size()) {
        return hule();
    }
    else {
        if (_lianzhuang) _model.changbang = _changbang + 1;
        return last();
    }
}

// 流局
void Game::reply_pingju() {
    for (int l = 0; l < 4; l++) {
        _model.defen[_model.player_id[l]] += _fenpei[l];
    }
    _model.changbang++;

    last();
}

// 打牌取得
std::vector<std::string> Game::get_dapai() const {
    return Game::_get_dapai(_rule, _model.shoupai[_model.lunban]);
}

// 吃面子取得
std::vector<std::string> Game::get_chi_mianzi(const int l) const {
    const auto d = "_+=-"[(4 + _model.lunban - l) % 4];
    return Game::_get_chi_mianzi(_rule, _model.shoupai[l],
        _dapai + d, _model.shan.paishu());
}

// 碰(ポン)面子取得
std::vector<std::string> Game::get_peng_mianzi(const int l) const {
    const auto d = "_+=-"[(4 + _model.lunban - l) % 4];
    return Game::_get_peng_mianzi(_rule, _model.shoupai[l],
        _dapai + d, _model.shan.paishu());
}

// 杠(槓)面子取得
std::vector<std::string> Game::get_gang_mianzi(const int l) const {
    if (l < 0) {
        return Game::_get_gang_mianzi(_rule, _model.shoupai[_model.lunban],
            {}, _model.shan.paishu(),
            std::accumulate(_n_gang.begin(), _n_gang.end(), 0));
    }
    else {
        const auto d = "_+=-"[(4 + _model.lunban - l) % 4];
        return Game::_get_gang_mianzi(_rule, _model.shoupai[l],
            _dapai + d, _model.shan.paishu(),
            std::accumulate(_n_gang.begin(), _n_gang.end(), 0));
    }
}

// 立直許可
std::pair<bool, std::vector<std::string>> Game::allow_lizhi(const std::string& p) const {
    return Game::_allow_lizhi(_rule, _model.shoupai[_model.lunban],
        p, _model.shan.paishu(),
        _model.defen[_model.player_id[_model.lunban]]);
}

// 和了可能
bool Game::allow_hule(const int l) const {
    if (l < 0) {
        const auto hupai = _model.shoupai[_model.lunban].lizhi()
            || _status == Status::GANGZIMO
            || _model.shan.paishu() == 0;
        return Game::_allow_hule(_rule,
            _model.shoupai[_model.lunban], {},
            _model.zhuangfeng, _model.lunban, hupai);
    }
    else {
        const auto p = (_status == Status::GANG
            ? _gang.substr(0, 1) + _gang.back()
            : _dapai
            ) + "_+=-"[(4 + _model.lunban - l) % 4];
        const auto hupai = _model.shoupai[l].lizhi()
            || _status == Status::GANG
            || _model.shan.paishu() == 0;
        return Game::_allow_hule(_rule,
            _model.shoupai[l], p,
            _model.zhuangfeng, l, hupai,
            _neng_rong[l]);
    }
}

// 流局可能
bool Game::allow_pingju() const {
    return Game::_allow_pingju(_rule, _model.shoupai[_model.lunban],
        _diyizimo);
}



// 打牌取得
std::vector<std::string> Game::_get_dapai(const Rule& rule, const Shoupai& shoupai) {
    if (rule.canChangePermissionLevel/*喰い替え許可レベル*/ == 0) return shoupai.get_dapai(true);
    if (rule.canChangePermissionLevel/*喰い替え許可レベル*/ == 1
        && !shoupai.zimo_().empty() && shoupai.zimo_().size() > 2)
    {
        const auto d = match(shoupai.zimo_(), re_get_dapai)[0];
        const auto deny = shoupai.zimo_().substr(0, 1)
            + (d == '0' ? '5' : d);
        return filter(shoupai.get_dapai(false),
            [&deny](const auto& p) { return std::regex_replace(p, re_ling, "5") != deny; });
    }
    return shoupai.get_dapai(false);
}

// 吃(チー)面子取得
std::vector<std::string> Game::_get_chi_mianzi(const Rule& rule, const Shoupai& shoupai, const std::string& p, const int paishu) {
    auto mianzi = shoupai.get_chi_mianzi(p, rule.canChangePermissionLevel/*喰い替え許可レベル*/ == 0);
    if (mianzi.empty()) return mianzi;
    if (rule.canChangePermissionLevel/*喰い替え許可レベル*/ == 1
        && shoupai.fulou_().size() == 3
        && shoupai.bingpai(p[0])[to_int(p[1])] == 2) mianzi.clear();
    return paishu == 0 ? std::vector<std::string>{} : mianzi;
}

// 碰(ポン)面子取得
std::vector<std::string> Game::_get_peng_mianzi(const Rule& rule, const Shoupai& shoupai, const std::string& p, const int paishu) {
    auto mianzi = shoupai.get_peng_mianzi(p);
    if (mianzi.empty()) return mianzi;
    return paishu == 0 ? std::vector<std::string>{} : mianzi;
}

// 杠(槓)面子取得
std::vector<std::string> Game::_get_gang_mianzi(const Rule& rule, const Shoupai& shoupai, const std::string& p, const int paishu, const int n_gang) {
    auto mianzi = p.empty() ? shoupai.get_gang_mianzi() : shoupai.get_gang_mianzi(p);
    if (mianzi.empty()) return mianzi;

    if (shoupai.lizhi()) {
        if (rule.lizhiPostClosedGangPermissionLevel/*リーチ後暗槓許可レベル*/ == 0) return {};
        else if (rule.lizhiPostClosedGangPermissionLevel/*リーチ後暗槓許可レベル*/ == 1) {
            int n_hule1 = 0, n_hule2 = 0;
            auto new_shoupai = shoupai;
            new_shoupai.dapai(shoupai.zimo_());
            for (const auto& p : tingpai(new_shoupai)) {
                n_hule1 += (int)hule_mianzi(new_shoupai, p).size();
            }
            new_shoupai = shoupai;
            new_shoupai.gang(mianzi[0]);
            for (const auto& p : tingpai(new_shoupai)) {
                n_hule2 += (int)hule_mianzi(new_shoupai, p).size();
            }
            if (n_hule1 > n_hule2) return {};
        }
        else {
            auto new_shoupai = shoupai;
            new_shoupai.dapai(shoupai.zimo_());
            int n_tingpai1 = (int)tingpai(new_shoupai).size();
            new_shoupai = shoupai;
            new_shoupai.gang(mianzi[0]);
            if (xiangting(new_shoupai) > 0) return {};
            int n_tingpai2 = (int)tingpai(new_shoupai).size();
            if (n_tingpai1 > n_tingpai2) return {};
        }
    }
    return paishu == 0 || n_gang == 4 ? std::vector<std::string>{} : mianzi;
}

// 立直可能
std::pair<bool, std::vector<std::string>> Game::_allow_lizhi(const Rule& rule, const Shoupai& shoupai, const std::string& p, const int paishu, const int defen) {
    if (shoupai.zimo_().empty()) return { false, {} };
    if (shoupai.lizhi())        return { false, {} };
    if (!shoupai.menqian())     return { false, {} };

    if (!rule.lizhiWithoutTsumoBonus/*ツモ番なしリーチあり*/&& paishu < 4) return { false, {} };
    if (rule.bankruptcyEndAll/*トビ終了あり*/ && defen < 1000)             return { false, {} };

    if (xiangting(shoupai) > 0) return { false, {} };

    if (!p.empty()) {
        auto new_shoupai = shoupai;
        new_shoupai.dapai(p);
        return { xiangting(new_shoupai) == 0 && tingpai(new_shoupai).size() > 0, {} };
    }
    else {
        std::vector<std::string> dapai;
        for (const auto& p : Game::_get_dapai(rule, shoupai)) {
            auto new_shoupai = shoupai;
            new_shoupai.dapai(p);
            if (xiangting(new_shoupai) == 0
                && tingpai(new_shoupai).size() > 0)
            {
                dapai.emplace_back(p);
            }
        }
        return { dapai.size(), dapai };
    }
}

// 和了可能
bool Game::_allow_hule(const Rule& rule, const Shoupai& shoupai, const std::string& p, const int zhuangfeng, const int menfeng, const bool hupai, const bool neng_rong) {
    if (!p.empty() && !neng_rong) return false;

    auto new_shoupai = shoupai;
    if (!p.empty()) new_shoupai.zimo(p);
    if (xiangting(new_shoupai) != -1) return false;

    if (hupai) return true;

    Param param = {
        rule,
        zhuangfeng,
        menfeng,
    };
    const auto& hule_ = ::hule(shoupai, p, param);

    return hule_.hupai.size();
}

// 流局可能
bool Game::_allow_pingju(const Rule& rule, const Shoupai& shoupai, const bool diyizimo) {
    if (!(diyizimo && !shoupai.zimo_().empty())) return false;
    if (!rule.abortiveDraw/*途中流局あり*/) return false;

    int n_yaojiu = 0;
    for (const auto s : { 'm', 'p', 's', 'z' }) {
        const auto& bingpai = shoupai.bingpai(s);
        const auto& nn = (s == 'z') ? zipai_n : yaojiu_n ;
        for (const auto n : nn) {
            if (bingpai[n] > 0) n_yaojiu++;
        }
    }
    return n_yaojiu >= 9;
}

// ノーテン宣言可能
bool Game::_allow_no_daopai(const Rule& rule, const Shoupai& shoupai, const int paishu) {
    if (paishu > 0 || !shoupai.zimo_().empty()) return false;
    if (!rule.notenDeclaration/*ノーテン宣言あり*/) return false;
    if (shoupai.lizhi()) return false;

    return xiangting(shoupai) == 0
        && tingpai(shoupai).size() > 0;
}
