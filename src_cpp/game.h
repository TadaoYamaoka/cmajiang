#pragma once

#include "shan.h"
#include "he.h"
#include "hule.h"

#include <array>

class Game {
public:
    struct Model {
        int qijia = 0;      // 起家
        int zhuangfeng = 0; // 庄风(場風)
        int jushu = 0;      // 局数
        int changbang = 0;  // 场棒(積み棒)
        int lizhibang = 0;  // 立直棒
        std::array<int, 4> defen = { 0, 0, 0, 0 }; // 得分(得点)
        Shan shan;          // 山
        std::array<Shoupai, 4> shoupai; // 手牌
        std::array<He, 4> he; // 河
        std::array<int, 4> player_id;
        int lunban; // 轮班(手番)
    };

    struct Pingju {
        enum Name {
            NONE,
            HUANGPAI, // 荒牌平局
            MANGUAN,  // 流し満貫
            YAO9,     // 九種九牌
            HULE3,    // 三家和了
            LIZHI4,   // 四家立直
            FENG4,    // 四風連打
            GANG4,    // 四槓散了
        } name;
        std::array<std::string, 4> shoupai;
    };

    enum class Message {
        NONE,
        DAOPAI,
        FULOU,
        HULE,
        GANG,
        DAPAI,
    };

    enum class Status {
        NONE,
        KAIJU,
        QIPAI,
        ZIMO,
        DAPAI,
        FULOU,
        GANG,
        GANGZIMO,
        HULE,
        PINGJU,
        JIEJI
    };

    enum class HuleOption {
        NONE,
        QIANGGANG,
        LINGSHANG
    };

    Game(const Rule& rule);
    Game() : Game(Rule{}) {};

    void call_players(const Status type);
    void reply(const int id, const Message msg, const std::string& arg = {});
    void next();

    // 手番のプレイヤーID
    int lunban_player_id() const { return _model.player_id[_model.lunban]; }

    // 开局(半荘の開始)
    void kaiju(const int qijia = -1);
    // 起牌(配牌)
    void qipai_(const Shan& shan);
    void qipai();
    // 自摸
    void zimo();
    // 打牌
    void dapai(const std::string& p);
    // 副露
    void fulou(const std::string& m);
    // 杠(槓)
    void gang(const std::string& m);
    // 杠自摸(槓自摸)
    void gangzimo();
    // 开杠(開槓)
    void kaigang();
    // 和了
    void hule();
    // 流局
    void pingju(Pingju::Name name = Pingju::NONE, std::array<std::string, 4> shoupai = {});
    // 終了
    void last();
    // 结局(終局)
    void jieju();

    // 応答取得
    struct Reply {
        Message msg;
        std::string arg;
    };
    Reply get_reply(const int l);

    // 结局(終局)の応答
    void reply_kaiju();
    // 起牌(配牌)の応答
    void reply_qipai();
    // 自摸の応答
    void reply_zimo();
    // 打牌の応答
    void reply_dapai();
    // 副露の応答
    void reply_fulou();
    // 杠(槓)の応答
    void reply_gang();
    // 和了の応答
    void reply_hule();
    // 流局
    void reply_pingju();

    // 打牌取得
    std::vector<std::string> get_dapai();
    // 吃(チー)面子取得
    std::vector<std::string> get_chi_mianzi(const int l);
    // 碰(ポン)面子取得
    std::vector<std::string> get_peng_mianzi(const int l);
    // 杠(槓)面子取得
    std::vector<std::string> get_gang_mianzi(const int l = -1);

    // 立直可能
    std::pair<bool, std::vector<std::string>> allow_lizhi(const std::string& p = {});
    // 和了可能
    bool allow_hule(const int l = -1);
    // 流局可能
    bool allow_pingju();

    // 打牌取得
    static std::vector<std::string> _get_dapai(const Rule& rule, const Shoupai& shoupai);
    // 吃(チー)面子取得
    static std::vector<std::string> _get_chi_mianzi(const Rule& rule, const Shoupai& shoupai, const std::string& p, const int paishu);
    // 碰(ポン)面子取得
    static std::vector<std::string> _get_peng_mianzi(const Rule& rule, const Shoupai& shoupai, const std::string& p, const int paishu);
    // 杠(槓)面子取得
    static std::vector<std::string> _get_gang_mianzi(const Rule& rule, const Shoupai& shoupai, const std::string& p, const int paishu, const int n_gang = -1);

    // 立直可能
    static std::pair<bool, std::vector<std::string>> _allow_lizhi(const Rule& rule, const Shoupai& shoupai, const std::string& p = {}, const int paishu = INT_MAX, const int defen = INT_MAX);
    // 和了可能
    static bool _allow_hule(const Rule& rule, const Shoupai& shoupai, const std::string& p, const int zhuangfeng, const int menfeng, const bool hupai, const bool neng_rong = true);
    // 流局可能
    static bool _allow_pingju(const Rule& rule, const Shoupai& shoupai, const bool diyizimo);
    // ノーテン宣言可能
    static bool _allow_no_daopai(const Rule& rule, const Shoupai& shoupai, const int paishu);

    int qijia() const { return _model.qijia; }
    const Shan& shan() const { return _model.shan; }

    const std::array<Shoupai, 4>& shoupai() const { return _model.shoupai; }
    const Shoupai& shoupai_(const int l) const { return _model.shoupai[l]; }

    const std::array<He, 4>& he() const { return _model.he; }
    const He& he_(const int l) const { return _model.he[l]; }

    int max_jushu() const { return _max_jushu; }

    bool diyizimo() const { return _diyizimo; }
    void set_diyizimo(const bool diyizimo_) { _diyizimo = diyizimo_; }

    const bool fengpai() const { return _fengpai; }

    const std::string& dapai_() const { return _dapai; }

    const std::array<int, 4>& lizhi() const { return _lizhi; }
    int lizhi_(const int l) const { return _lizhi[l]; }

    const std::array<bool, 4>& yifa() const { return _yifa; }
    bool yifa_(const int l) const { return _yifa[l]; }
    void set_yifa(const std::array<bool, 4>& yifa_) { _yifa = yifa_; }
    void set_yifa_(const int l, const bool yifa_) { _yifa[l] = yifa_; }

    void set_n_gang(const std::array<int, 4>& n_gang_) { _n_gang = n_gang_; }

    const std::array<bool, 4>& neng_rong() const { return _neng_rong; }
    bool neng_rong_(const int l) const { return _neng_rong[l]; }
    void set_neng_rong(const std::array<bool, 4>& neng_rong_) { _neng_rong = neng_rong_; }
    void set_neng_rong_(const int l, const bool neng_rong_) { _neng_rong[l] = neng_rong_; }

    const std::string& fulou_() const { return _fulou; }
    const std::string& gang_() const { return _gang; }
    
    const std::vector<int>& hule_() const { return _hule; }
    void set_hule(const std::vector<int>& hule_) { _hule = hule_; }
    const Defen& defen() const { return _defen; }
    const std::array<int, 4>& rank() const { return _rank; }
    const std::array<float, 4>& point() const { return _point; }
    const Pingju& pingju_() const { return _pingju; }
    const std::array<int, 4>& fenpei() const { return _fenpei; }

    bool lianzhuang() const {return _lianzhuang; }
    void set_lianzhuang(const bool lianzhuang_) { _lianzhuang = lianzhuang_; }

    bool no_game() const { return _no_game; }
    void set_no_game(const bool no_game_) { _no_game = no_game_; }

    Model& model() { return _model; }
    const Rule& rule() const { return _rule; }

    const Status status() const { return _status; }

private:
    Rule _rule;
    Model _model;

    Status _status;
    Defen _defen;
    std::array<int, 4> _rank;
    std::array<float, 4> _point;
    Pingju _pingju;
    std::array<Reply, 4> _reply;

    // 最大局数
    int _max_jushu;
    // 第一ツモ
    bool _diyizimo;
    // 四風連打
    bool _fengpai;
    // 打牌
    std::string _dapai;
    // 副露
    std::string _fulou;
    // 杠(槓)
    std::string _gang;
    // 立直
    std::array<int, 4> _lizhi;
    // 一発
    std::array<bool, 4> _yifa;
    // // 杠(槓)
    std::array<int, 4> _n_gang;
    // 能荣(フリテンでない状態)
    std::array<bool, 4> _neng_rong;
    // 和了
    std::vector<int> _hule;
    HuleOption _hule_option;
    // 流局
    bool _no_game;
    // 连庄(連荘)
    bool _lianzhuang;
    // 场棒(積み棒)
    int _changbang;
    // テンパイ
    std::array<int, 4> _fenpei;
};


template <typename T, typename F>
bool find_if(const T& v, F f) {
    return std::find_if(v.begin(), v.end(), f) != v.end();
}

template <typename T, typename V>
bool find(const T& v, const V& value) {
    return std::find(v.begin(), v.end(), value) != v.end();
}

template <typename T, typename F>
size_t count_if(const T& v, F f) {
    return std::count_if(v.begin(), v.end(), f);
}

template <typename T>
size_t max_index(const T& v) {
    return std::max_element(v.begin(), v.end()) - v.begin();
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::array<T, 4>& arr) {
    os << arr[0] << " " << arr[1] << " " << arr[2] << " " << arr[3];
    return os;
}

template <typename T>
std::array<T, 4> order_by_player_id(const T& arr, std::array<int, 4> player_id) {
    return { arr[player_id[0]], arr[player_id[1]], arr[player_id[2]], arr[player_id[3]] };
}

template <typename T>
T pop_front(std::vector<T>& v) {
    const T front = v.front();
    v.erase(v.begin());
    return front;
}