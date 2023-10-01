#include "shoupai.h"

#include <string_view>
#include <algorithm>
#include <sstream>
#include <map>

const std::regex re_ling{ R"(0)" };
const std::regex re_wu{ R"(5)" };
const std::regex re_menqian{ R"([\+\=\-])" };
const std::regex re_menqian_end{ R"([\+\=\-]$)" };

const std::regex Shoupai::_re_valid_pai{ R"(^(?:[mps]\d|z[1-7])_?\*?[\+\=\-]?$)" };
const std::regex Shoupai::_re_qipai{ R"([mpsz]\d+)" };
const std::regex Shoupai::_re_valid_mianzi1{ R"(^z.*[089])" };
const std::regex Shoupai::_re_valid_mianzi3{ R"(^[mpsz](\d)\1\1[\+\=\-]\1?$)" };
const std::regex Shoupai::_re_valid_mianzi4{ R"(([mps])05)" };
const std::regex Shoupai::_re_valid_mianzi5{ R"(^[mpsz](\d)\1\1\1[\+\=\-]?$)" };
const std::regex Shoupai::_re_valid_mianzi6{ R"(\d(?![\+\=\-]))" };
const std::regex Shoupai::_re_valid_mianzi7{ R"(\d[\+\=\-]$)" };
const std::regex Shoupai::_re_valid_mianzi8{ R"(^[mps]\d+\-\d*$)" };
const std::regex Shoupai::_re_valid_mianzi9{ R"(\d)" };
const std::regex Shoupai::_re_valid_mianzi10{ R"(\d[\+\=\-]?)" };
const std::regex Shoupai::_re_fulou1{ R"(\d{4}$)" };
const std::regex Shoupai::_re_fulou2{ R"(\d{3}[\+\=\-]\d$)" };
const std::regex Shoupai::_re_fulou3{ R"(\d(?![\+\=\-]))" };
const std::regex Shoupai::_re_fulou4{ R"(\d{4})" };
const std::regex Shoupai::_re_get_dapai1{ R"(\d(?=[\+\=\-]))" };
const std::regex Shoupai::_re_get_dapai2{ R"(^[mpsz](\d)\1\1)" };
const std::regex Shoupai::_re_get_dapai3{ R"(^[mps]\d\-\d\d$)" };
const std::regex Shoupai::_re_get_dapai4{ R"(^[mps]\d\d\d\-$)" };

// 牌文字列検証
bool Shoupai::valid_pai(const std::string& p) {
    return std::regex_match(p, _re_valid_pai);
}

// 面子文字列検証
std::string Shoupai::valid_mianzi(const std::string& m) {
    if (std::regex_search(m, _re_valid_mianzi1)) return {};
    const auto h = std::regex_replace(m, re_ling, "5");
    if (std::regex_match(h, _re_valid_mianzi3)) {
        return std::regex_replace(m, _re_valid_mianzi4, "$0150");
    }
    else if (std::regex_match(h, _re_valid_mianzi5)) {
        std::vector<std::string> matches;
        for (std::sregex_iterator it(m.begin(), m.end(), _re_valid_mianzi6), end; it != end; ++it) {
            matches.emplace_back(it->str());
        }
        std::sort(matches.rbegin(), matches.rend());
        std::ostringstream os;
        os << m[0];
        std::copy(matches.begin(), matches.end(), std::ostream_iterator<std::string>(os));
        std::smatch match;
        if (std::regex_search(m, match, _re_valid_mianzi7)) {
            os << match.str();
        }
        return os.str();
    }
    else if (std::regex_match(h, _re_valid_mianzi8)) {
        std::vector<std::string> nn;
        for (std::sregex_iterator it(h.begin(), h.end(), _re_valid_mianzi9), end; it != end; ++it) {
            nn.emplace_back(it->str());
        }
        std::sort(nn.begin(), nn.end());
        if (nn.size() != 3) return {};
        if (nn[0][0] + 1 != nn[1][0] || nn[1][0] + 1 != nn[2][0]) return {};
        std::vector<std::string> matches;
        for (std::sregex_iterator it(h.begin(), h.end(), _re_valid_mianzi10), end; it != end; ++it) {
            matches.emplace_back(it->str());
        }
        std::sort(matches.begin(), matches.end());
        std::ostringstream os;
        os << h[0];
        std::copy(matches.begin(), matches.end(), std::ostream_iterator<std::string>(os));
        // 红牌(赤牌)
        const auto hongpai = std::regex_search(m, re_ling);
        if (hongpai) {
            return std::regex_replace(os.str(), re_wu, "0");
        }
        else {
            return os.str();
        }
    }
    return {};
}

void Shoupai::_set(const std::vector<std::string>& qipai) {
    for (const auto p : qipai) {
        if (p == "_") {
            __++;
            continue;
        }
        if (!valid_pai(p)) throw std::invalid_argument(p);
        const char s = p[0], n = to_int(p[1]);
        if (_bingpai(s)[n] == 4) throw std::invalid_argument(p);
        _bingpai(s)[n]++;
        if (s != 'z' && n == 0) _bingpai(s)[5]++;
    }
}

Shoupai::Shoupai() :
    __{}, _m(10), _p(10), _s(10), _z(8), _lizhi{ false }
{}

Shoupai::Shoupai(const std::vector<std::string>& qipai) :
    __{}, _m(10), _p(10), _s(10), _z(8), _lizhi{ false }
{
    _set(qipai);
}

Shoupai::Shoupai(const std::string& paistr) :
    __{}, _m(10), _p(10), _s(10), _z(8)
{
    std::vector<std::string> fulou;
    int fulou_count = 0;
    auto pos = paistr.find_first_of(',');
    std::string_view bingpai;
    if (pos != std::string::npos) {
        bingpai = std::string_view(&paistr[0], pos);
        while (true) {
            const auto offset = pos + 1;
            pos = paistr.find_first_of(',', offset);
            if (pos != std::string::npos) {
                fulou.emplace_back(paistr, offset, pos - offset);
                if (!fulou.back().empty()) fulou_count++;
            }
            else {
                fulou.emplace_back(paistr, offset, paistr.size() - offset);
                if (!fulou.back().empty()) fulou_count++;
                break;
            }
        }
    }
    else {
        bingpai = std::string_view(&paistr[0], paistr.size());
    }

    std::vector<std::string> qipai;
    for (const auto& p : bingpai) {
        if (p == '_') qipai.emplace_back("_");
    }
    for (std::regex_iterator<std::string_view::iterator> it = std::regex_iterator(bingpai.begin(), bingpai.end(), _re_qipai), end; it != end; ++it) {
        const auto suitstr = it->str();
        const auto s = suitstr[0];
        for (size_t i = 1; i < suitstr.size(); ++i) {
            if (std::isdigit(suitstr[i])) {
                const auto n = to_int(suitstr[i]);
                if (s == 'z' && (n < 1 || 7 < n)) continue;
                const char pai[] = { s, suitstr[i] };
                qipai.emplace_back(pai, 2);
            }
        }
    }
    const auto resize = 14 - fulou_count * 3;
    if (qipai.size() > resize) {
        qipai.erase(qipai.begin() + resize, qipai.end());
    }
    std::string zimo = (qipai.size() + 1) % 3 == 0 ? qipai.back() : std::string{};
    _set(qipai);

    for (auto& m : fulou) {
        if (m.empty() && _fulou.size() > 0) {
            _zimo = _fulou.back();
            break;
        }
        auto v = valid_mianzi(m);
        if (!v.empty()) {
            _fulou.emplace_back(std::move(v));
        }
    }

    if (_zimo.empty() && !zimo.empty()) {
        _zimo = std::move(zimo);
    }
    _lizhi = bingpai.size() > 0 && bingpai.back() == '*';
}

std::string Shoupai::toString() const {
    std::string paistr(__ + (_zimo == "_" ? -1 : 0), '_');

    for (const auto s : { 'm', 'p', 's', 'z' }) {
        std::string suitstr(1, s);
        const auto& bingpai = this->bingpai(s);
        auto n_hongpai = s == 'z' ? 0 : bingpai[0];
        for (int n = 1; n < bingpai.size(); n++) {
            auto n_pai = bingpai[n];
            if (!_zimo.empty()) {
                if (_zimo.size() == 2 && _zimo[0] == s && to_int(_zimo[1]) == n) {
                    n_pai--;
                }
                if (n == 5 && _zimo.size() == 2 && _zimo[0] == s && _zimo[1] == '0') {
                    n_pai--;
                    n_hongpai--;
                }
            }
            for (int i = 0; i < n_pai; i++) {
                if (n == 5 && n_hongpai > 0) {
                    suitstr += "0";
                    n_hongpai--;
                }
                else {
                    suitstr += to_char(n);
                }
            }
        }
        if (suitstr.size() > 1) paistr += suitstr;
    }
    if (!_zimo.empty() && _zimo.size() <= 2) paistr += _zimo;
    if (_lizhi) paistr += '*';

    for (const auto& m : _fulou) {
        paistr += ',';
        paistr += m;
    }
    if (!_zimo.empty() && _zimo.size() > 2) paistr += ',';

    return paistr;
}

void Shoupai::decrease(const char s, const int n) {
    auto& bingpai = _bingpai(s);
    if (bingpai[n] == 0 || n == 5 && bingpai[0] == bingpai[5]) {
        if (__ == 0) throw std::invalid_argument(to_string(s, n));
        __--;
    }
    else {
        bingpai[n]--;
        if (n == 0) bingpai[5]--;
    }
}

// 自摸
Shoupai& Shoupai::zimo(const std::string& p, bool check) {
    if (check && !_zimo.empty()) throw std::runtime_error("zimo must be empty");
    if (p == "_") {
        __++;
        _zimo = p;
    }
    else {
        if (!valid_pai(p)) throw std::invalid_argument(p);
        const auto s = p[0];
        const auto n = to_int(p[1]);
        auto& bingpai = _bingpai(s);
        if (bingpai[n] == 4) throw std::invalid_argument(p + " cannot be greater than 4");
        bingpai[n]++;
        if (n == 0) {
            if (bingpai[5] == 4) throw std::invalid_argument(s + "5 cannot be greater than 4");
            bingpai[5]++;
        }
        _zimo = p.substr(0, 2);
    }
    return *this;
}

// 打牌
Shoupai& Shoupai::dapai(const std::string& p, bool check) {
    if (check && _zimo.empty()) throw std::runtime_error("zimo must not be empty");
    if (!valid_pai(p)) throw std::invalid_argument(p);
    const auto s = p[0];
    const auto n = to_int(p[1]);
    decrease(s, n);
    _zimo.clear();
    if (p.back() == '*') _lizhi = true;
    return *this;
}

// 副露
Shoupai& Shoupai::fulou(const std::string& m, bool check) {
    if (check && !_zimo.empty()) throw std::runtime_error("zimo must be empty");
    if (m != valid_mianzi(m)) throw std::invalid_argument(m);
    if (std::regex_search(m, _re_fulou1)) throw std::invalid_argument(m);
    if (std::regex_search(m, _re_fulou2)) throw std::invalid_argument(m);
    const auto s = m[0];
    for (std::sregex_iterator it(m.begin(), m.end(), _re_fulou3), end; it != end; ++it) {
        decrease(s, to_int(it->str()[0]));
    }
    _fulou.emplace_back(m);
    if (!std::regex_search(m, _re_fulou4)) _zimo = m;
    return *this;
}

// 杠(槓)
Shoupai& Shoupai::gang(const std::string& m, bool check) {
    if (check && _zimo.empty()) throw std::runtime_error("zimo must not be empty");
    if (check && _zimo.size() > 2) throw std::runtime_error("unexpected zimo " + _zimo);
    if (m != valid_mianzi(m)) throw std::invalid_argument(m);
    const auto s = m[0];
    if (std::regex_search(m, _re_fulou1)) {
        for (std::sregex_iterator it(m.begin(), m.end(), _re_valid_mianzi9), end; it != end; ++it) {
            decrease(s, to_int(it->str()[0]));
        }
        _fulou.emplace_back(m);
    }
    else if (std::regex_search(m, _re_fulou2)) {
        const auto m1 = m.substr(0, 5);
        auto it = std::find(_fulou.begin(), _fulou.end(), m1);
        if (it == _fulou.end()) throw std::invalid_argument(m);
        _fulou[std::distance(_fulou.begin(), it)] = m;
        decrease(s, to_int(m.back()));
    }
    else throw std::invalid_argument(m);
    _zimo.clear();
    return *this;
}

// 门前(門前)
bool Shoupai::menqian() const {
    return !std::any_of(_fulou.begin(), _fulou.end(), [](const auto& m) { return std::regex_search(m, re_menqian); });
}

// 打牌取得
std::vector<std::string> Shoupai::get_dapai(bool check) const {
    if (_zimo.empty()) throw std::runtime_error("zimo must not be empty");

    std::map<std::string, bool> deny;
    if (check && _zimo.size() > 2) {
        auto m = _zimo;
        const auto s = m[0];
        std::smatch match;
        std::regex_search(m, match, _re_get_dapai1);
        auto n = to_int(match.str()[0]);
        if (n == 0) n = 5;
        deny[to_string(s, n)] = true;
        if (!std::regex_search(std::regex_replace(m, re_ling, "5"), _re_get_dapai2)) {
            if (n < 7 && std::regex_match(m, _re_get_dapai3)) deny[to_string(s, n + 3)] = true;
            if (3 < n && std::regex_match(m, _re_get_dapai4)) deny[to_string(s, n - 3)] = true;
        }
    }

    std::vector<std::string> dapai;
    if (!_lizhi) {
        for (const auto s : { 'm', 'p', 's', 'z' }) {
            const auto& bingpai = this->bingpai(s);
            for (int n = 1; n < bingpai.size(); n++) {
                if (bingpai[n] == 0)  continue;
                const char sn[] = { s, to_char(n) };
                std::string pai(sn, 2);
                if (check && deny[pai]) continue;
                if (pai == _zimo && bingpai[n] == 1) continue;
                if (s == 'z' || n != 5) dapai.emplace_back(std::move(pai));
                else {
                    const char s0[]{ s, '0' };
                    std::string pai0(s0, 2);
                    if (bingpai[0] > 0
                        && pai0 != _zimo || bingpai[0] > 1)
                        dapai.emplace_back(std::move(pai0));
                    if (bingpai[0] < bingpai[5]) dapai.emplace_back(pai);
                }
            }
        }
    }
    if (_zimo.size() == 2) dapai.emplace_back(_zimo + '_');
    return dapai;

}

// 吃(チー)面子取得
std::vector<std::string> Shoupai::get_chi_mianzi(const std::string& p, bool check) const {
    if (!_zimo.empty()) throw std::runtime_error("zimo must be empty");
    if (!valid_pai(p)) throw std::invalid_argument(p);

    const auto s = p[0];
    auto n = to_int(p[1]);
    if (n == 0) n = 5;
    std::smatch match;
    if (!std::regex_search(p, match, re_menqian_end)) throw std::invalid_argument(p);
    const auto d = match.str();
    if (s == 'z' || d != "-") return {};
    if (_lizhi) return {};

    std::vector<std::string> mianzi;
    const auto& bingpai = this->bingpai(s);
    if (3 <= n && bingpai[n - 2] > 0 && bingpai[n - 1] > 0) {
        if (!check
            || (3 < n ? bingpai[n - 3] : 0) + bingpai[n]
            < 14 - (_fulou.size() + 1) * 3)
        {
            if (n - 2 == 5 && bingpai[0] > 0) mianzi.emplace_back(std::string(1, s) + "067-");
            if (n - 1 == 5 && bingpai[0] > 0) mianzi.emplace_back(std::string(1, s) + "406-");
            if (n - 2 != 5 && n - 1 != 5 || bingpai[0] < bingpai[5])
                mianzi.emplace_back(to_string(s, n - 2, n - 1, p[1], d[0]));
        }
    }
    if (2 <= n && n <= 8 && bingpai[n - 1] > 0 && bingpai[n + 1] > 0) {
        if (!check || bingpai[n] < 14 - (_fulou.size() + 1) * 3) {
            if (n - 1 == 5 && bingpai[0] > 0) mianzi.emplace_back(std::string(1, s) + "06-7");
            if (n + 1 == 5 && bingpai[0] > 0) mianzi.emplace_back(std::string(1, s) + "34-0");
            if (n - 1 != 5 && n + 1 != 5 || bingpai[0] < bingpai[5])
                mianzi.emplace_back(to_string(s, n - 1, p[1], d[0], n + 1));
        }
    }
    if (n <= 7 && bingpai[n + 1] > 0 && bingpai[n + 2] > 0) {
        if (!check
            || bingpai[n] + (n < 7 ? bingpai[n + 3] : 0)
            < 14 - (_fulou.size() + 1) * 3)
        {
            if (n + 1 == 5 && bingpai[0] > 0) mianzi.emplace_back(std::string(1, s) + "4-06");
            if (n + 2 == 5 && bingpai[0] > 0) mianzi.emplace_back(std::string(1, s) + "3-40");
            if (n + 1 != 5 && n + 2 != 5 || bingpai[0] < bingpai[5])
                mianzi.emplace_back(to_string(s, p[1], d[0], n + 1, n + 2));
        }
    }
    return mianzi;
}

// 碰(ポン)面子取得
std::vector<std::string> Shoupai::get_peng_mianzi(const std::string& p) const {
    if (!_zimo.empty()) throw std::runtime_error("zimo must be empty");
    if (!valid_pai(p)) throw std::invalid_argument(p);

    const auto s = p[0];
    auto n = to_int(p[1]);
    if (n == 0) n = 5;
    std::smatch match;
    if (!std::regex_search(p, match, re_menqian_end)) throw std::invalid_argument(p);
    const auto d = match.str();
    if (_lizhi) return {};

    std::vector<std::string> mianzi;
    const auto& bingpai = this->bingpai(s);
    if (bingpai[n] >= 2) {
        if (n == 5 && bingpai[0] >= 2)  mianzi.emplace_back(std::string(1, s) + "00" + p[1] + d);
        if (n == 5 && bingpai[0] >= 1 && bingpai[5] - bingpai[0] >= 1)
            mianzi.emplace_back(std::string(1, s) + "50" + p[1] + d);
        if (n != 5 || bingpai[5] - bingpai[0] >= 2)
            mianzi.emplace_back(to_string(s, n, n, p[1], d[0]));
    }
    return mianzi;
}

// 杠(槓)面子取得
std::vector<std::string> Shoupai::get_gang_mianzi(const std::string& p) const {
    if (!p.empty()) {
        if (!_zimo.empty()) throw std::runtime_error("zimo must be empty");
        if (!valid_pai(p)) throw std::invalid_argument(p);

        const auto s = p[0];
        auto n = to_int(p[1]);
        if (n == 0) n = 5;
        std::smatch match;
        if (!std::regex_search(p, match, re_menqian_end)) throw std::invalid_argument(p);
        const auto d = match.str();
        if (_lizhi) return {};

        const auto& bingpai = this->bingpai(s);
        if (bingpai[n] == 3) {
            if (n == 5) return { s + std::string(3 - bingpai[0], '5')
                + std::string(bingpai[0], '0') + p[1] + d };
            else return { to_string(s, n, n, n, n) + d };
        }
        return {};
    }
    else {
        if (_zimo.empty()) throw std::runtime_error("zimo must not be empty");
        if (_zimo.size() > 2) throw std::runtime_error("unexpected zimo " + _zimo);
        const auto p = std::regex_replace(_zimo, re_ling, "5");

        std::vector<std::string> mianzi;
        for (const auto s : { 'm', 'p', 's', 'z' }) {
            const auto& bingpai = this->bingpai(s);
            for (int n = 1; n < bingpai.size(); n++) {
                if (bingpai[n] == 0) continue;
                if (bingpai[n] == 4) {
                    if (_lizhi && to_string(s, n) != p) continue;
                    if (n == 5) mianzi.emplace_back(s + std::string(4 - bingpai[0], '5')
                        + std::string(bingpai[0], '0'));
                    else mianzi.emplace_back(to_string(s, n, n, n, n));
                }
                else {
                    if (_lizhi) continue;
                    for (const auto& m : _fulou) {
                        if (std::regex_replace(m, re_ling, "5").substr(0, 4) == to_string(s, n, n, n)) {
                            if (n == 5 && bingpai[0] > 0) mianzi.emplace_back(m + '0');
                            else                          mianzi.emplace_back(m + to_char(n));
                        }
                    }
                }
            }
        }
        return mianzi;
    }
}
