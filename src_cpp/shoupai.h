#pragma once

#include <vector>
#include <string>
#include <regex>
#include <functional>
#include <stdexcept>
#include <ostream>
#include <istream>

// 手牌
class Shoupai {
public:
    // 牌文字列検証
    static bool valid_pai(const std::string& p);
    // 面子文字列検証
    static std::string valid_mianzi(const std::string& m);

    Shoupai();
    Shoupai(const std::vector<std::string>& qipai);
    Shoupai(const std::string& paistr);
    Shoupai(const std::vector<std::string>& pai, const std::vector<std::string>& fulou);

    void set(const std::string& paistr) { *this = { paistr }; }

    std::string toString() const;
    operator std::string() const {
        return toString();
    }

    Shoupai clone() const {
        return Shoupai(*this);
    }

    void decrease(const char s, const int n);

    // 自摸
    Shoupai& zimo(const std::string& p, bool check = true);
    // 打牌
    Shoupai& dapai(const std::string& p, bool check = true);
    // 副露
    Shoupai& fulou(const std::string& m, bool check = true);
    // 杠(槓)
    Shoupai& gang(const std::string& m, bool check = true);
    // 门前(門前)
    bool menqian() const;
    // 立直
    bool lizhi() const { return _lizhi; }
    void set_lizhi(const bool lizhi) { _lizhi = lizhi; }
    // 打牌取得
    std::vector<std::string> get_dapai(bool check = true) const;
    // 吃(チー)面子取得
    std::vector<std::string> get_chi_mianzi(const std::string& p, bool check = true) const;
    // 碰(ポン)面子取得
    std::vector<std::string> get_peng_mianzi(const std::string& p) const;
    // 杠(槓)面子取得
    std::vector<std::string> get_gang_mianzi(const std::string& p = {}) const;

    const std::vector<int>& bingpai(const char s) const {
        return const_cast<Shoupai&>(*this)._bingpai(s);
    }

    const std::vector<int>& m() const { return _m; }
    const std::vector<int>& p() const { return _p; }
    const std::vector<int>& s() const { return _s; }
    const std::vector<int>& z() const { return _z; }
    int m_(const int n) const { return _m[n]; }
    int p_(const int n) const { return _p[n]; }
    int s_(const int n) const { return _s[n]; }
    int z_(const int n) const { return _z[n]; }
    const std::vector<std::string>& fulou_() const { return _fulou; };
    const std::string& zimo_() const { return _zimo; }

    friend int xiangting_yiban(const Shoupai& shoupai);
    friend std::vector<std::string> tingpai(const Shoupai& shoupai, const std::function<int(const Shoupai&)>& f_xiangting);
    friend std::vector<std::vector<std::string>> hule_mianzi_yiban(Shoupai& shoupai, const std::string& hulepai);

private:
    void _set(const std::vector<std::string>& qipai);

    static const std::regex _re_valid_pai;
    static const std::regex _re_qipai;
    static const std::regex _re_valid_mianzi1;
    static const std::regex _re_valid_mianzi3;
    static const std::regex _re_valid_mianzi4;
    static const std::regex _re_valid_mianzi5;
    static const std::regex _re_valid_mianzi6;
    static const std::regex _re_valid_mianzi7;
    static const std::regex _re_valid_mianzi8;
    static const std::regex _re_valid_mianzi9;
    static const std::regex _re_fulou1;
    static const std::regex _re_fulou2;
    static const std::regex _re_fulou3;
    static const std::regex _re_fulou4;
    static const std::regex _re_get_dapai1;
    static const std::regex _re_get_dapai2;
    static const std::regex _re_get_dapai3;
    static const std::regex _re_get_dapai4;

    // 兵牌(副露を含めない打牌可能な手牌)
    std::vector<int>& _bingpai(const char s) {
        switch (s) {
        case 'm':
            return _m;
        case 'p':
            return _p;
        case 's':
            return _s;
        case 'z':
            return _z;
        default:
            throw std::invalid_argument(std::string(1, s));
        }
    }

    int __;
    std::vector<int> _m;
    std::vector<int> _p;
    std::vector<int> _s;
    std::vector<int> _z;

    // 副露
    std::vector<std::string> _fulou;
    // 自摸
    std::string _zimo;
    // 立直
    bool _lizhi;

};

inline std::ostream& operator<<(std::ostream& os, const Shoupai& shoupai) {
    os << shoupai.toString();
    return os;
}

inline std::string to_string(const int n) {
    return std::string(1, n + '0');
}
inline std::string to_string(const int n1, const int n2) {
    const char str[] = { (char)(n1 + '0'), (char)(n2 + '0') };
    return std::string(str, 2);
}
inline std::string to_string(const char s, const int n) {
    const char str[] = { s, (char)(n + '0') };
    return std::string(str, 2);
}
inline std::string to_string(const char s, const int n1, const int n2) {
    const char str[] = { s, (char)(n1 + '0'), (char)(n2 + '0') };
    return std::string(str, 3);
}
inline std::string to_string(const char s, const int n1, const int n2, const char c1, const char c2) {
    const char str[] = { s, (char)(n1 + '0'), (char)(n2 + '0'), c1, c2 };
    return std::string(str, 5);
}
inline std::string to_string(const char s, const int n1, const char c1, const char c2, const int n2) {
    const char str[] = { s, (char)(n1 + '0'), c1, c2, (char)(n2 + '0') };
    return std::string(str, 5);
}
inline std::string to_string(const char s, const char c1, const char c2, const int n1, const int n2) {
    const char str[] = { s, c1, c2, (char)(n1 + '0'), (char)(n2 + '0') };
    return std::string(str, 5);
}
inline std::string to_string(const char s, const int n1, const int n2, const int n3) {
    const char str[] = { s, (char)(n1 + '0'), (char)(n2 + '0'), (char)(n3 + '0') };
    return std::string(str, 4);
}
inline std::string to_string(const char s, const int n1, const int n2, const int n3, const int n4) {
    const char str[] = { s, (char)(n1 + '0'), (char)(n2 + '0'), (char)(n3 + '0'), (char)(n4 + '0') };
    return std::string(str, 5);
}

inline int to_int(const char s) {
    return s - '0';
}

inline char to_char(const int n) {
    return (char)(n + '0');
}

extern const std::regex re_ling;
extern const std::regex re_wu;
extern const std::regex re_digit;
extern const std::regex re_menqian;
extern const std::regex re_menqian_end;

extern const std::regex re_angang;
extern const std::regex re_peng_gang;
extern const std::regex re_jiagang;
extern const std::regex re_chi1;
extern const std::regex re_chi2;
extern const std::regex re_gang;
