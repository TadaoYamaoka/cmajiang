#pragma once

#include "shoupai.h"
#include "rule.h"

#include <random>

extern std::mt19937_64 engine;
void set_seed(const size_t seed);

// 山
class Shan {
public:
    // 真宝牌(表示牌ではない現物のドラ)
    static std::string zhenbaopai(const std::string& p) {
        if (!Shoupai::valid_pai(p)) throw std::invalid_argument(p);
        const auto s = p[0];
        int n = to_int(p[1]);
        if (n == 0) n = 5;
        return s == 'z' ? (n < 5 ? to_string(s, (n % 4 + 1)) : to_string(s, ((n - 4) % 3 + 5)))
            : to_string(s, (n % 9 + 1));
    }

    Shan() {}
    Shan(const Rule& rule);
    Shan(std::vector<std::string>& pai, const Rule& rule = {});

    void set(const std::string& shanstr, const Rule& rule);

    std::vector<std::string>& pai() { return _pai; }
    const std::vector<std::string>& pai() const { return _pai; }
    // 牌数
    int paishu() const { return (int)_pai.size() - 14; }
    // 宝牌(ドラ)
    std::vector<std::string> baopai() const {
        std::vector<std::string> baopai_;
        for (const auto& p : _baopai) {
            if (!p.empty())
                baopai_.emplace_back(p);
        }
        return baopai_;
    }
    void set_baopai(const std::vector<std::string>& baopai) { _baopai = baopai; }
    // 里宝牌(裏ドラ)
    std::vector<std::string> libaopai() const {
        if (!_closed) return {};
        if (!_libaopai.empty()) return _libaopai;
        return {};
    }
    const std::vector<std::string>& libaopai_() const {
        return _libaopai;
    }

    // 自摸
    std::string zimo();
    // 杠自摸(槓自摸)
    std::string gangzimo();
    // 开杠(開槓)
    Shan& kaigang();

    Shan& close() { _closed = true; return *this; }
    const Rule& rule() const { return _rule; }

    friend std::ostream& operator<<(std::ostream& os, const Shan& shan);

private:
    Rule _rule;
    std::vector<std::string> _pai;
    std::vector<std::string> _baopai;   // 宝牌(ドラ)
    std::vector<std::string> _libaopai; // 里宝牌(裏ドラ)
    bool _weikaigang;                   // 未开杠(未開槓)
    bool _closed;
};

inline std::ostream& operator<<(std::ostream& os, const Shan& shan) {
    for (const auto& p : shan._pai)
        os << p;
    os << ",";
    for (const auto& p : shan._baopai)
        os << p;
    os << ",";
    for (const auto& p : shan._libaopai)
        os << p;
    return os;
}
