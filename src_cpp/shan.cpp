#include "shan.h"

#include <algorithm>

std::random_device seed_gen;
std::mt19937_64 engine(seed_gen());

void set_seed(const size_t seed) {
    engine.seed(seed);
}

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
    if (rule.libaopai)
        _libaopai.emplace_back(_pai[9]);
}

Shan::Shan(std::vector<std::string>& pai, const Rule& rule)
    : _pai(pai), _rule(rule), _weikaigang(false), _closed(false) {
    _baopai.emplace_back(_pai[4]);
    // 裏ドラあり
    if (rule.libaopai)
        _libaopai.emplace_back(_pai[9]);
}

void Shan::set(const std::string& shanstr, const Rule& rule) {
    _rule = rule;
    _weikaigang = false;
    _closed = false;
    std::stringstream ss(shanstr);
    {
        std::string paistr;
        std::getline(ss, paistr, ',');
        _pai.clear();
        for (size_t i = 0; i < paistr.size(); i += 2) {
            _pai.emplace_back(paistr.substr(i, 2));
        }
    }
    {
        std::string baopaistr;
        std::getline(ss, baopaistr, ',');
        for (size_t i = 0; i < baopaistr.size(); i += 2) {
            _baopai.emplace_back(baopaistr.substr(i, 2));
        }
    }
    {
        std::string libaopaistr;
        std::getline(ss, libaopaistr);
        for (size_t i = 0; i < libaopaistr.size(); i += 2) {
            _libaopai.emplace_back(libaopaistr.substr(i, 2));
        }
    }
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
    if (!_libaopai.empty() && _rule.ganglibaopai)
        _libaopai.emplace_back(_pai[9]);
    _weikaigang = false;
    return *this;
}
