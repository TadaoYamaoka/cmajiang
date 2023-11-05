#include "he.h"

static const std::regex re_fulou{ R"(\d(?=[\+\=\-]))" };

// 打牌
He& He::dapai(const std::string& p) {
    if (!Shoupai::valid_pai(p)) throw std::invalid_argument(p);
    _pai.emplace_back(std::regex_replace(p, re_menqian_end, ""));
    _find.emplace(p.substr(0, 1) + (p[1] == '0' ? '5' : p[1]));
    return *this;
}

// 副露
He& He::fulou(const std::string& m) {
    if (Shoupai::valid_mianzi(m).empty()) throw std::invalid_argument(m);
    std::smatch match;
    std::regex_search(m, match, re_fulou);
    const auto p = m[0] + match.str();
    if (!std::regex_search(m, match, re_menqian)) throw std::invalid_argument(m);
    if (_pai.back().substr(0, 2) != p)     throw std::invalid_argument(m);
    _pai.back() += match.str();
    return *this;
}

bool He::find(const std::string& p) {
    return _find.find(p.substr(0, 1) + (p[1] == '0' ? '5' : p[1])) != _find.end();
}

void He::set(const std::string& paistr) {
    static const std::regex re_pai{ R"([mpsz]\d_?\*?)" };
    _pai.clear();
    for (std::sregex_iterator it(paistr.begin(), paistr.end(), re_pai), end; it != end; ++it)
        dapai(it->str());
}
