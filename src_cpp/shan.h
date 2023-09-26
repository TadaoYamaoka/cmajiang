#pragma once

#include "shoupai.h"
#include "rule.h"

#include <random>

extern std::mt19937_64 engine;

// 山
class Shan {
public:
    static std::string zhenbaopai(const std::string& p) {
        if (!Shoupai::valid_pai(p)) throw std::invalid_argument(p);
        const auto s = p[0];
        int n = to_int(p[1]);
        if (n == 0) n = 5;
        return s == 'z' ? (n < 5 ? to_string(s, (n % 4 + 1)) : to_string(s, ((n - 4) % 3 + 5)))
            : to_string(s, (n % 9 + 1));
    }

    Shan();
    Shan(const Rule& rule);

    const std::vector<std::string>& pai() const { return _pai; }
    std::vector<std::string>& pai() { return _pai; }
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
    // 副宝牌(裏ドラ)
    std::vector<std::string> fubaopai() const {
        if (!_closed) return {};
        if (!_fubaopai.empty()) return _fubaopai;
        return {};
    }

    // 自摸
    std::string zimo();
    // 杠自摸(槓自摸)
    std::string gangzimo();
    // 开杠(開槓)
    Shan& kaigang();

    Shan& close() { _closed = true; return *this; }

private:
    Rule _rule;
    std::vector<std::string> _pai;
    std::vector<std::string> _baopai;   // 宝牌(ドラ)
    std::vector<std::string> _fubaopai; // 副宝牌(裏ドラ)
    bool _weikaigang;                   // 未开杠(未開槓)
    bool _closed;
};
