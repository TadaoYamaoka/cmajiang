#include "shan.h"

#include <algorithm>
#include <random>

std::random_device seed_gen;
std::mt19937_64 engine(seed_gen());

Shan::Shan() : Shan(Rule()) {}

Shan::Shan(const Rule& rule) : _rule(rule), _weikaigang(false), _closed(false) {
    for (const auto s : { 'm', 'p', 's', 'z' }) {
        for (int n = 1; n <= (s == 'z' ? 7 : 9); n++) {
            for (int i = 0; i < 4; i++) {
                if (n == 5 && s != 'z' && i < rule.hongpai_(s))
                    _pai.emplace_back(to_string(s, 0));
                else
                    _pai.emplace_back(to_string(s, n));
            }
        }
    }

    std::shuffle(_pai.begin(), _pai.end(), engine);

    _baopai.emplace_back(_pai[4]);
    // 裏ドラあり
    if (rule.fubaopai)
        _fubaopai.emplace_back(_pai[9]);
}

// 自摸
std::string Shan::zimo() {
    if (_closed)       throw std::runtime_error("closed");
    if (paishu() == 0) throw std::runtime_error("paishu");
    if (_weikaigang)   throw std::runtime_error("weikaigang");
    auto p = std::move(_pai.back());
    _pai.pop_back();
    return p;
}

// 杠自摸(槓自摸)
std::string Shan::gangzimo() {
    if (_closed)             throw std::runtime_error("closed");
    if (paishu() == 0)       throw std::runtime_error("paishu");
    if (_weikaigang)         throw std::runtime_error("weikaigang");
    if (_baopai.size() == 5) throw std::runtime_error("baopai");
    // カンドラあり
    _weikaigang = _rule.gangbaopai;
    if (!_weikaigang) _baopai.emplace_back();
    auto p = std::move(_pai.front());
    _pai.erase(_pai.begin());
    return p;
}

// 开杠(開槓)
Shan& Shan::kaigang() {
    if (_closed)             throw std::runtime_error("closed");
    if (!_weikaigang)        throw std::runtime_error("weikaigang");
    _baopai.emplace_back(_pai[4]);
    // カン裏あり
    if (!_fubaopai.empty() && _rule.ganglibaopai)
        _fubaopai.emplace_back(_pai[9]);
    _weikaigang = false;
    return *this;
}
