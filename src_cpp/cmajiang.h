#pragma once

#include "shoupai.h"

// 手牌
class __Shoupai {
public:
    // 牌文字列検証
    static bool valid_pai(const std::string& p) { return Shoupai::valid_pai(p); }
    // 面子文字列検証
    static std::string valid_mianzi(const std::string& m) { return Shoupai::valid_mianzi(m); }

    __Shoupai() : _shoupai() {}
    __Shoupai(const std::vector<std::string>& qipai) : _shoupai(qipai) {}
    __Shoupai(const std::string& paistr) : _shoupai(paistr) {};

    std::string toString() const { return _shoupai.toString(); }

    // 自摸
    void zimo(const std::string& p, bool check = true) { _shoupai.zimo(p, check); }
    // 打牌
    void dapai(const std::string& p, bool check = true) { _shoupai.dapai(p, check); }
    // 副露
    void fulou(const std::string& m, bool check = true) { _shoupai.fulou(m, check); }
    // 杠(槓)
    void gang(const std::string& m, bool check = true) { _shoupai.gang(m, check); }
    // 门前(門前)
    bool menqian() const { return _shoupai.menqian(); }
    // 立直
    bool lizhi() const { return _shoupai.lizhi(); }
    // 打牌取得
    std::vector<std::string> get_dapai(bool check = true) const { return _shoupai.get_dapai(check); }
    // 吃(チー)面子取得
    std::vector<std::string> get_chi_mianzi(const std::string& p, bool check = true) const { return _shoupai.get_chi_mianzi(p, check); }
    // 碰(ポン)面子取得
    std::vector<std::string> get_peng_mianzi(const std::string& p) const { return _shoupai.get_peng_mianzi(p); }
    // 杠(槓)面子取得
    std::vector<std::string> get_gang_mianzi(const std::string& p) const { return _shoupai.get_gang_mianzi(p); }
    std::vector<std::string> get_gang_mianzi() const { return _shoupai.get_gang_mianzi(); }

    std::vector<int> bingpai(const std::string& s) const { return _shoupai.bingpai(s[0]); }

    std::vector<int> m() const { return _shoupai.m(); }
    std::vector<int> p() const { return _shoupai.p(); }
    std::vector<int> s() const { return _shoupai.s(); }
    std::vector<int> z() const { return _shoupai.z(); }
    int m(const int n) const { return _shoupai.m(n); }
    int p(const int n) const { return _shoupai.p(n); }
    int s(const int n) const { return _shoupai.s(n); }
    int z(const int n) const { return _shoupai.z(n); }
    std::vector<std::string> fulou() const { return _shoupai.fulou(); };
    std::string zimo() const { return _shoupai.zimo(); }

private:
    Shoupai _shoupai;
};