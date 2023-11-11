#include "random.h"
#include "random.h"
#include "xiangting.h"

#include <set>
#include <cassert>

constexpr char suit[] = { 'm', 'p', 's', 'z'};
constexpr char tajia[] = { '+', '=', '-' };

// 牌山から順子を取り出す
template <bool DUANYAOJIU = false, bool YAOJIU = false, bool LVYISE = false>
bool pick_shunzi(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> dist4(0, 3);
    constexpr int N = DUANYAOJIU ? 5 : 7;
    constexpr int M = N * 3;
    std::uniform_int_distribution<int> shunzi_dist(0, M - 1);
    const int start = shunzi_dist(mt);
    for (int i = 0; i < M; i++) {
        const int j = (start + i) % M;
        const int s_i = j / N;
        const char s = suit[s_i];
        const int n = j % N + (DUANYAOJIU ? 1 : 0);
        std::string p[] = { to_string(s, n + 1), to_string(s, n + 2), to_string(s, n + 3) };
        if (rule.hongpai[s_i] && n >= 2 && n <= 4 && dist4(mt) == 0) {
            const auto hongpai = to_string(s, 0);
            if (rest[hongpai] > 0)
                p[4 - n] = hongpai;
        }
        if (rest[p[0]] > 0 && rest[p[1]] > 0 && rest[p[2]] > 0) {
            pai.emplace_back(p[0]);
            pai.emplace_back(p[1]);
            pai.emplace_back(p[2]);
            rest[p[0]]--;
            rest[p[1]]--;
            rest[p[2]]--;
            return true;
        }
    }
    return false;
}

// 幺九牌
template <>
bool pick_shunzi<false, true>(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    constexpr int N = 2;
    constexpr int M = N * 3;
    std::uniform_int_distribution<int> shunzi_dist(0, M - 1);
    const int start = shunzi_dist(mt);
    for (int i = 0; i < M; i++) {
        const int j = (start + i) % M;
        const char s = suit[j / N];
        int n = j % N;
        if (n == 1) n = 6;
        const auto p1 = to_string(s, n + 1);
        const auto p2 = to_string(s, n + 2);
        const auto p3 = to_string(s, n + 3);
        if (rest[p1] > 0 && rest[p2] > 0 && rest[p3] > 0) {
            pai.emplace_back(p1);
            pai.emplace_back(p2);
            pai.emplace_back(p3);
            rest[p1]--;
            rest[p2]--;
            rest[p3]--;
            return true;
        }
    }
    return false;
}

// 緑牌
template <>
bool pick_shunzi<false, false, true>(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    const auto p1 = "s2";
    const auto p2 = "s3";
    const auto p3 = "s4";
    if (rest[p1] && rest[p2] && rest[p3]) {
        pai.emplace_back(p1);
        pai.emplace_back(p2);
        pai.emplace_back(p3);
        rest[p1]--;
        rest[p2]--;
        rest[p3]--;
        return true;
    }
    return false;
}

// 数牌の種類指定
bool pick_shunzi(const int s_i, std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> dist4(0, 3);
    constexpr int N = 7;
    std::uniform_int_distribution<int> shunzi_dist(0, N - 1);
    const int start = shunzi_dist(mt);
    for (int i = 0; i < N; i++) {
        const int n = (start + i) % N;
        const char s = suit[s_i];
        std::string p[] = { to_string(s, n + 1), to_string(s, n + 2), to_string(s, n + 3) };
        if (rule.hongpai[s_i] && n >= 2 && n <= 4 && dist4(mt) == 0) {
            const auto hongpai = to_string(s, 0);
            if (rest[hongpai] > 0)
                p[4 - n] = hongpai;
        }
        if (rest[p[0]] > 0 && rest[p[1]] > 0 && rest[p[2]] > 0) {
            pai.emplace_back(p[0]);
            pai.emplace_back(p[1]);
            pai.emplace_back(p[2]);
            rest[p[0]]--;
            rest[p[1]]--;
            rest[p[2]]--;
            return true;
        }
    }
    return false;
}

// 牌山から刻子を取り出す
template <bool DUANYAOJIU = false, bool YAOJIU = false, bool ZIPAI = true, bool SANYUAN = false, bool LVYISE = false>
bool pick_kezi(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> dist4(0, 3);
    constexpr int N = DUANYAOJIU ? 7 : 9;
    constexpr int M = N * 3 + (DUANYAOJIU ? 0 : 7);
    std::uniform_int_distribution<int> kezi_dist(0, M - 1);
    const int start = kezi_dist(mt);
    for (int i = 0; i < M; i++) {
        const int j = (start + i) % M;
        const int s_i = j / N;
        const char s = suit[s_i];
        const int n = j % N + (DUANYAOJIU ? 1 : 0);
        const auto hongpai = to_string(s, 0);
        const auto p = to_string(s, n + 1);
        if (s != 'z' && rule.hongpai[s_i] && n == 4 && rest[hongpai] > 0 && dist4(mt) == 0 && rest[p] >= 2) {
            pai.emplace_back(p);
            pai.emplace_back(p);
            pai.emplace_back(hongpai);
            rest[p] -= 2;
            rest[hongpai]--;
            return true;
        }
        else if (rest[p] >= 3) {
            pai.emplace_back(p);
            pai.emplace_back(p);
            pai.emplace_back(p);
            rest[p] -= 3;
            return true;
        }
    }
    return false;
}

// 幺九牌
template <>
bool pick_kezi<false, true>(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    constexpr int N = 2;
    constexpr int M = N * 3 + 7;
    std::uniform_int_distribution<int> kezi_dist(0, M - 1);
    const int start = kezi_dist(mt);
    for (int i = 0; i < M; i++) {
        const int j = (start + i) % M;
        const char s = j >= N * 3 ? 'z' : suit[j / N];
        const int n = s == 'z' ? j - N * 3 : j % N == 0 ? 0 : 8;
        const auto p = to_string(s, n + 1);
        if (rest[p] >= 3) {
            pai.emplace_back(p);
            pai.emplace_back(p);
            pai.emplace_back(p);
            rest[p] -= 3;
            return true;
        }
    }
    return false;
}

// 幺九牌(字牌なし)
template <>
bool pick_kezi<false, true, false>(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    constexpr int N = 2;
    constexpr int M = N * 3;
    std::uniform_int_distribution<int> kezi_dist(0, M - 1);
    const int start = kezi_dist(mt);
    for (int i = 0; i < M; i++) {
        const int j = (start + i) % M;
        const char s = suit[j / N];
        const int n = j % N == 0 ? 1 : 9;
        const auto p = to_string(s, n);
        if (rest[p] >= 3) {
            pai.emplace_back(p);
            pai.emplace_back(p);
            pai.emplace_back(p);
            rest[p] -= 3;
            return true;
        }
    }
    return false;
}

// 字牌
template <>
bool pick_kezi<false, false, true, true>(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    constexpr int N = 7;
    std::uniform_int_distribution<int> kezi_dist(0, N - 1);
    const int start = kezi_dist(mt);
    for (int i = 0; i < N; i++) {
        const int j = (start + i) % N;
        const int n = 1 + j;
        const auto p = to_string('z', n);
        if (rest[p] >= 3) {
            pai.emplace_back(p);
            pai.emplace_back(p);
            pai.emplace_back(p);
            rest[p] -= 3;
            return true;
        }
    }
    return false;
}

// 三元牌
template <>
bool pick_kezi<false, false, false, true>(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    constexpr int N = 3;
    std::uniform_int_distribution<int> kezi_dist(0, N - 1);
    const int start = kezi_dist(mt);
    for (int i = 0; i < N; i++) {
        const int j = (start + i) % N;
        const int n = 5 + j;
        const auto p = to_string('z', n);
        if (rest[p] >= 3) {
            pai.emplace_back(p);
            pai.emplace_back(p);
            pai.emplace_back(p);
            rest[p] -= 3;
            return true;
        }
    }
    return false;
}

// 緑牌
template <>
bool pick_kezi<false, false, true, false, true>(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    static const std::string lvy[] = { "s2", "s3", "s4", "s6", "s8", "z6" };
    constexpr int N = 6;
    std::uniform_int_distribution<int> kezi_dist(0, N - 1);
    const int start = kezi_dist(mt);
    for (int i = 0; i < N; i++) {
        const int n = (start + i) % N;
        const auto p = lvy[n];
        if (rest[p] >= 3) {
            pai.emplace_back(p);
            pai.emplace_back(p);
            pai.emplace_back(p);
            rest[p] -= 3;
            return true;
        }
    }
    return false;
}

// 数牌の種類指定
template <bool ZIPAI = true>
bool pick_kezi(const int s_i, std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> dist4(0, 3);
    constexpr int N = 9 + (ZIPAI ? 7 : 0);
    std::uniform_int_distribution<int> kezi_dist(0, N - 1);
    const int start = kezi_dist(mt);
    for (int i = 0; i < N; i++) {
        const int j = (start + i) % N;
        const char s = ZIPAI && j >= 9 ? 'z' : suit[s_i];
        const int n = ZIPAI && s == 'z' ? j - 9 : j;
        const auto hongpai = to_string(s, 0);
        const auto p = to_string(s, n + 1);
        if ((!ZIPAI || s != 'z') && rule.hongpai[s_i] && n == 4 && rest[hongpai] > 0 && dist4(mt) == 0 && rest[p] >= 2) {
            pai.emplace_back(p);
            pai.emplace_back(p);
            pai.emplace_back(hongpai);
            rest[p] -= 2;
            rest[hongpai]--;
            return true;
        }
        else if (rest[p] >= 3) {
            pai.emplace_back(p);
            pai.emplace_back(p);
            pai.emplace_back(p);
            rest[p] -= 3;
            return true;
        }
    }
    return false;
}

// 牌山から雀頭を取り出す
template <bool DUANYAOJIU = false, bool YAOJIU = false, bool ZIPAI = true, bool SANYUAN = false, bool LVYISE = false>
bool pick_jiangpai(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> dist4(0, 3);
    constexpr int N = DUANYAOJIU ? 7 : 9;
    constexpr int M = N * 3 + (DUANYAOJIU ? 0 : 7);
    std::uniform_int_distribution<int> jiangpai_dist(0, M - 1);
    const int start = jiangpai_dist(mt);
    for (int i = 0; i < M; i++) {
        const int j = (start + i) % M;
        const int s_i = j / N;
        const char s = suit[s_i];
        const int n = j % N + (DUANYAOJIU ? 1 : 0);
        const auto hongpai = to_string(s, 0);
        const auto p = to_string(s, n + 1);
        if (s != 'z' && rule.hongpai[s_i] && n == 4 && rest[hongpai] > 0 && dist4(mt) == 0 && rest[p] >= 1) {
            pai.emplace_back(p);
            pai.emplace_back(hongpai);
            rest[p]--;
            rest[hongpai]--;
            return true;
        }
        else if (rest[p] >= 2) {
            pai.emplace_back(p);
            pai.emplace_back(p);
            rest[p] -= 2;
            return true;
        }
    }
    return false;
}

// 幺九牌
template <>
bool pick_jiangpai<false, true, true>(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    constexpr int N = 2;
    constexpr int M = N * 3 + 7;
    std::uniform_int_distribution<int> jiangpai_dist(0, M - 1);
    const int start = jiangpai_dist(mt);
    for (int i = 0; i < M; i++) {
        const int j = (start + i) % M;
        const char s = j >= N * 3 ? 'z' : suit[j / N];
        const int n = s == 'z' ? j - N * 3 : j % N == 0 ? 0 : 8;
        const auto p = to_string(s, n + 1);
        if (rest[p] >= 2) {
            pai.emplace_back(p);
            pai.emplace_back(p);
            rest[p] -= 2;
            return true;
        }
    }
    return false;
}

// 幺九牌(字牌なし)
template <>
bool pick_jiangpai<false, true, false>(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    constexpr int N = 2;
    constexpr int M = N * 3;
    std::uniform_int_distribution<int> jiangpai_dist(0, M - 1);
    const int start = jiangpai_dist(mt);
    for (int i = 0; i < M; i++) {
        const int j = (start + i) % M;
        const char s = suit[j / N];
        const int n = j % N == 0 ? 1 : 9;
        const auto p = to_string(s, n);
        if (rest[p] >= 2) {
            pai.emplace_back(p);
            pai.emplace_back(p);
            rest[p] -= 2;
            return true;
        }
    }
    return false;
}

// 字牌
template <>
bool pick_jiangpai<false, false, true, true>(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    constexpr int N = 7;
    std::uniform_int_distribution<int> jiangpai_dist(0, N - 1);
    const int start = jiangpai_dist(mt);
    for (int i = 0; i < N; i++) {
        const int j = (start + i) % N;
        const int n = 1 + j;
        const auto p = to_string('z', n);
        if (rest[p] >= 2) {
            pai.emplace_back(p);
            pai.emplace_back(p);
            rest[p] -= 2;
            return true;
        }
    }
    return false;
}

// 三元牌
template <>
bool pick_jiangpai<false, false, false, true>(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    constexpr int N = 3;
    std::uniform_int_distribution<int> jiangpai_dist(0, N - 1);
    const int start = jiangpai_dist(mt);
    for (int i = 0; i < N; i++) {
        const int j = (start + i) % N;
        const int n = 5 + j;
        const auto p = to_string('z', n);
        if (rest[p] >= 2) {
            pai.emplace_back(p);
            pai.emplace_back(p);
            rest[p] -= 2;
            return true;
        }
    }
    return false;
}

// 緑牌
template <>
bool pick_jiangpai<false, false, true, false, true>(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    static const std::string lvy[] = { "s2", "s3", "s4", "s6", "s8", "z6" };
    constexpr int N = 6;
    std::uniform_int_distribution<int> jiangpai_dist(0, N - 1);
    const int start = jiangpai_dist(mt);
    for (int i = 0; i < N; i++) {
        const int n = (start + i) % N;
        const auto p = lvy[n];
        if (rest[p] >= 2) {
            pai.emplace_back(p);
            pai.emplace_back(p);
            rest[p] -= 2;
            return true;
        }
    }
    return false;
}

// 数牌の種類指定
template <bool ZIPAI = true>
bool pick_jiangpai(const int s_i, std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> dist4(0, 3);
    constexpr int N = 9 + (ZIPAI ? 7 : 0);
    std::uniform_int_distribution<int> jiangpai_dist(0, N - 1);
    const int start = jiangpai_dist(mt);
    for (int i = 0; i < N; i++) {
        const int j = (start + i) % N;
        const char s = ZIPAI && j >= 9 ? 'z' : suit[s_i];
        const int n = ZIPAI && s == 'z' ? j - 9 : j;
        const auto hongpai = to_string(s, 0);
        const auto p = to_string(s, n + 1);
        if ((!ZIPAI || s != 'z') && rule.hongpai[s_i] && n == 4 && rest[hongpai] > 0 && dist4(mt) == 0 && rest[p] >= 1) {
            pai.emplace_back(p);
            pai.emplace_back(hongpai);
            rest[p]--;
            rest[hongpai]--;
            return true;
        }
        else if (rest[p] >= 2) {
            pai.emplace_back(p);
            pai.emplace_back(p);
            rest[p] -= 2;
            return true;
        }
    }
    return false;
}


template <bool DUANYAOJIU = false, bool YAOJIU = false, bool ZIPAI = true, bool LVYISE = false, bool JIANGPAI = true>
bool setup_hule(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> bool_dist(0, 1);
    while (pai.size() < 12) {
        if (bool_dist(mt) == 0) {
            // 順子
            if (pick_shunzi<DUANYAOJIU, YAOJIU, LVYISE>(pai, rest, rule, mt)) continue;
        }
        // 刻子
        if (!pick_kezi<DUANYAOJIU, YAOJIU, ZIPAI, false, LVYISE>(pai, rest, rule, mt)) return false;
    }
    if (JIANGPAI) {
        // 雀頭
        if (!pick_jiangpai<DUANYAOJIU, YAOJIU, ZIPAI, false, LVYISE>(pai, rest, rule, mt)) return false;
    }
    return true;
}

// チー
std::string make_chi(const std::string& p1, const std::string& p2, const std::string& p3, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> i_dist(0, 2);
    const int i = i_dist(mt);
    switch (i_dist(mt)) {
    case 0:
        return std::string(1, p1[0]) + p1[1] + '-' + p2[1] + p3[1];
    case 1:
        return std::string(1, p1[0]) + p1[1] + p2[1] + '-' + p3[1];
    default:
        return std::string(1, p1[0]) + p1[1] + p2[1] + p3[1] + '-';
    }
}

// 槓
template <bool ANGANG = false>
std::string make_gang(const std::string& p1, const std::string& p2, const std::string& p3, const std::string& p, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> tajia_dist(0, 2);
    std::uniform_int_distribution<int> angang_dist(0, 7);
    std::uniform_int_distribution<int> jiagang_dist(0, 7);

    const int tajia_i = tajia_dist(mt);
    if (ANGANG && angang_dist(mt) == 0) // 暗槓
        return p1 + p2[1] + p3[1] + p[1];
    else if (jiagang_dist(mt) == 0) // 加槓
        return p1 + p2[1] + p3[1] + tajia[tajia_i] + p[1];
    else // 大明槓
        return p1 + p2[1] + p3[1] + p[1] + tajia[tajia_i];
}

// ポン、槓
template <bool GANG = true>
std::string make_peng_gang(const std::string& p1, const std::string& p2, const std::string& p3, std::map<std::string, int>& rest, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> dist4(0, 3);
    std::uniform_int_distribution<int> tajia_dist(0, 2);
    std::uniform_int_distribution<int> gang_dist(0, 15);

    if (GANG && gang_dist(mt) == 0) {
        auto p = p1[1] == '0' ? to_string(p1[0], 5) : p1;
        // 赤牌
        if (p1[1] == '5' && p2[1] == '5' && p3[1] == '5' && dist4(mt) == 0 && rest[to_string(p1[0], 0)] > 0) {
            p = to_string(p1[0], 0);
        }
        if (rest[p] > 0) {
            rest[p]--;
            return make_gang(p1, p2, p3, p, mt);
        }
    }
    const int tajia_i = tajia_dist(mt);
    return p1 + p2[1] + p3[1] + tajia[tajia_i];
}


template <int MIN = 0>
void make_fulou(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> n_fulou_dist(MIN, 4);
    const int n_fulou = n_fulou_dist(mt);
    std::array<int, 4> menqian_i{ 0, 1, 2, 3 };
    std::shuffle(menqian_i.begin(), menqian_i.end(), mt);
    std::stable_sort(menqian_i.rend() - n_fulou, menqian_i.rend());
    for (int i = 0; i < n_fulou; i++) {
        const int j = menqian_i[i] * 3;
        if (pai[j] != pai[j + 1])
            fulou.emplace_back(make_chi(pai[j], pai[j + 1], pai[j + 2], mt));
        else
            fulou.emplace_back(make_peng_gang(pai[j], pai[j + 1], pai[j + 2], rest, mt));
        pai.erase(pai.begin() + j + 2);
        pai.erase(pai.begin() + j + 1);
        pai.erase(pai.begin() + j);
    }
}

template <>
void make_fulou<0>(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> n_fulou_dist(0, 4);
    const int n_fulou = n_fulou_dist(mt);
    if (n_fulou > 0) {
        std::array<int, 4> menqian_i{ 0, 1, 2, 3 };
        std::shuffle(menqian_i.begin(), menqian_i.end(), mt);
        std::stable_sort(menqian_i.rend() - n_fulou, menqian_i.rend());
        for (int i = 0; i < n_fulou; i++) {
            const int j = menqian_i[i] * 3;
            if (pai[j] != pai[j + 1])
                fulou.emplace_back(make_chi(pai[j], pai[j + 1], pai[j + 2], mt));
            else
                fulou.emplace_back(make_peng_gang(pai[j], pai[j + 1], pai[j + 2], rest, mt));
            pai.erase(pai.begin() + j, pai.begin() + j + 3);
        }
    }
}

// 場風
bool setup_zhuangfeng(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const int zhuangfeng, const Rule& rule, std::mt19937_64& mt) {
    auto p = to_string('z', zhuangfeng + 1);
    if (rest[p] >= 3) {
        rest[p] -= 3;
        pai = { p, p, p };
        if (!setup_hule(pai, rest, rule, mt)) return false;
        make_fulou(pai, rest, fulou, mt);
        return true;
    }
    return false;
}

// 自風
bool setup_menfeng(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const int l, const Rule& rule, std::mt19937_64& mt) {
    auto p = to_string('z', l + 1);
    if (rest[p] >= 3) {
        rest[p] -= 3;
        pai = { p, p, p };
        if (!setup_hule(pai, rest, rule, mt)) return false;
        make_fulou(pai, rest, fulou, mt);
        return true;
    }
    return false;
}

// 翻牌
bool setup_fanpai(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> fanpai_dist(0, 2);
    int fanpai = fanpai_dist(mt);
    auto p = to_string('z', 5 + fanpai);
    if (rest[p] >= 3) {
        rest[p] -= 3;
        pai = { p, p, p };
        if (!setup_hule(pai, rest, rule, mt)) return false;
        make_fulou(pai, rest, fulou, mt);
        return true;
    }
    return false;
}

// 平和
bool setup_pinghe(std::vector<std::string>& pai, std::map<std::string, int>& rest, const int zhuangfeng, const int l, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> dist4(0, 3);
    if (!pick_shunzi(pai, rest, rule, mt)) return false;
    if (!pick_shunzi(pai, rest, rule, mt)) return false;
    if (!pick_shunzi(pai, rest, rule, mt)) return false;
    if (!pick_shunzi(pai, rest, rule, mt)) return false;
    // 雀頭はオタ牌(客風牌)
    std::vector<int> kefeng;
    for (int i = 0; i < 4; i++) {
        if (i != zhuangfeng && i != l)
            kefeng.emplace_back(i);
    }
    constexpr int N = 9;
    const int M = N * 3 + (int)kefeng.size();
    std::uniform_int_distribution<int> jiangpai_dist(0, M - 1);
    int start = jiangpai_dist(mt);
    for (int i = 0; i < M; i++) {
        const int j = (start + i) % M;
        const int s_i = j / N;
        const char s = suit[s_i];
        const int n = s == 'z' ? kefeng[j - N * 3] : j % N;
        const auto hongpai = to_string(s, 0);
        const auto p = to_string(s, n + 1);
        if (s != 'z' && rule.hongpai[s_i] && n == 4 && rest[hongpai] > 0 && dist4(mt) == 0 && rest[p] >= 1) {
            pai.emplace_back(p);
            pai.emplace_back(hongpai);
            rest[p]--;
            rest[hongpai]--;
            return true;
        }
        else if (rest[p] >= 2) {
            pai.emplace_back(p);
            pai.emplace_back(p);
            rest[p] -= 2;
            return true;
        }
    }
    return false;
}

// 断幺九
bool setup_duanyaojiu(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    if (!pick_shunzi<true>(pai, rest, rule, mt)) return false;
    if (!pick_shunzi<true>(pai, rest, rule, mt)) return false;
    return setup_hule<true>(pai, rest, rule, mt);
}

// 盃口
bool setup_beikou(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> dist4(0, 3);
    std::uniform_int_distribution<int> shunzi_dist(0, 7 * 3 - 1);
    int start = shunzi_dist(mt);
    for (int i = 0; i < 7 * 3; i++) {
        int j = (start + i) % (7 * 3);
        const int s_i = j / 7;
        char s = suit[s_i];
        int n = j % 7;
        const auto p1 = to_string(s, n + 1);
        const auto p2 = to_string(s, n + 2);
        const auto p3 = to_string(s, n + 3);
        const auto hongpai = to_string(s, 0);
        if (rule.hongpai[s_i] && n >= 2 && n <= 4 && rest[hongpai] > 0 && dist4(mt) == 0) {
            std::array<int, 3> n_pai{ 2, 2, 2 };
            n_pai[4 - n]--;
            if (rest[p1] < n_pai[0] || rest[p2] < n_pai[1] || rest[p3] < n_pai[2])
                continue;
            pai.emplace_back(p1);
            if (n != 4) {
                pai.emplace_back(p1);
                rest[p1] -= 2;
            }
            else {
                rest[p1]--;
                pai.emplace_back(hongpai);
                rest[hongpai]--;
            }
            pai.emplace_back(p2);
            if (n + 1 != 4) {
                pai.emplace_back(p2);
                rest[p2] -= 2;
            }
            else {
                rest[p2]--;
                pai.emplace_back(hongpai);
                rest[hongpai]--;
            }
            pai.emplace_back(p3);
            if (n + 2 != 4) {
                pai.emplace_back(p3);
                rest[p3] -= 2;
            }
            else {
                rest[p3]--;
                pai.emplace_back(hongpai);
                rest[hongpai]--;
            }
            return true;
        }
        else if (rest[p1] >= 2 && rest[p2] >= 2 && rest[p3] >= 2) {
            pai.emplace_back(p1); pai.emplace_back(p1);
            pai.emplace_back(p2); pai.emplace_back(p2);
            pai.emplace_back(p3); pai.emplace_back(p3);
            rest[p1] -= 2;
            rest[p2] -= 2;
            rest[p3] -= 2;
            return true;
        }
    }
    return false;
}

// 一盃口
bool setup_yibeikou(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    if (setup_beikou(pai, rest, rule, mt)) {
        if (!setup_hule(pai, rest, rule, mt)) return false;
        return true;
    }
    return false;
}

// 三色同順
bool setup_sansetongshun(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> dist4(0, 3);
    std::uniform_int_distribution<int> shunzi_dist(0, 7 - 1);
    std::uniform_int_distribution<int> suit_dist(0, 2);
    int start = shunzi_dist(mt);
    for (int i = 0; i < 7; i++) {
        int n = (start + i) % 7;
        std::string p[][3] = {
            { to_string('m', n + 1), to_string('m', n + 2), to_string('m', n + 3) },
            { to_string('p', n + 1), to_string('p', n + 2), to_string('p', n + 3) },
            { to_string('s', n + 1), to_string('s', n + 2), to_string('s', n + 3) },
        };
        if (rule.hongpai[0] && rule.hongpai[1] && rule.hongpai[2] && n >= 2 && n <= 4 && dist4(mt) == 0) {
            const int hongpai_s = suit_dist(mt);
            const auto hongpai = to_string(suit[hongpai_s], 0);
            if (rest[hongpai] > 0)
                p[hongpai_s][4 - n] = hongpai;
        }
        if (rest[p[0][0]] > 0 && rest[p[0][1]] > 0 && rest[p[0][2]] > 0 && rest[p[1][0]] > 0 && rest[p[1][1]] > 0 && rest[p[1][2]] > 0 && rest[p[2][0]] > 0 && rest[p[2][1]] > 0 && rest[p[2][2]] > 0) {
            pai.emplace_back(p[0][0]); pai.emplace_back(p[0][1]); pai.emplace_back(p[0][2]);
            pai.emplace_back(p[1][0]); pai.emplace_back(p[1][1]); pai.emplace_back(p[1][2]);
            pai.emplace_back(p[2][0]); pai.emplace_back(p[2][1]); pai.emplace_back(p[2][2]);
            rest[p[0][0]]--; rest[p[0][1]]--; rest[p[0][2]]--;
            rest[p[1][0]]--; rest[p[1][1]]--; rest[p[1][2]]--;
            rest[p[2][0]]--; rest[p[2][1]]--; rest[p[2][2]]--;
            break;
        }
    }
    if (pai.size() > 0) {
        if (!setup_hule(pai, rest, rule, mt)) return false;
        make_fulou(pai, rest, fulou, mt);
        return true;
    }
    return false;
}

// 一気通貫
bool setup_yiqitongguan(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> dist4(0, 3);
    std::uniform_int_distribution<int> suit_dist(0, 2);
    int start = suit_dist(mt);
    for (int i = 0; i < 3; i++) {
        const int j = (start + i) % 3;
        const char s = suit[j];
        const auto hongpai = to_string(s, 0);
        const bool is_hongpai = rule.hongpai[j] && rest[hongpai] > 0 && dist4(mt) == 0;
        const auto p1 = to_string(s, 1);
        const auto p2 = to_string(s, 2);
        const auto p3 = to_string(s, 3);
        const auto p4 = to_string(s, 4);
        const auto p5 = is_hongpai ? hongpai : to_string(s, 5);
        const auto p6 = to_string(s, 6);
        const auto p7 = to_string(s, 7);
        const auto p8 = to_string(s, 8);
        const auto p9 = to_string(s, 9);
        if (rest[p1] > 0 && rest[p2] > 0 && rest[p3] > 0 && rest[p4] > 0 && rest[p5] > 0 && rest[p6] > 0 && rest[p7] > 0 && rest[p8] > 0 && rest[p9] > 0) {
            pai.emplace_back(p1); pai.emplace_back(p2); pai.emplace_back(p3); pai.emplace_back(p4); pai.emplace_back(p5); pai.emplace_back(p6); pai.emplace_back(p7); pai.emplace_back(p8); pai.emplace_back(p9);
            rest[p1]--; rest[p2]--; rest[p3]--; rest[p4]--; rest[p5]--; rest[p6]--; rest[p7]--; rest[p8]--; rest[p9]--;
            break;
        }
    }
    if (pai.size() > 0) {
        if (!setup_hule(pai, rest, rule, mt)) return false;
        make_fulou(pai, rest, fulou, mt);
        return true;
    }
    return false;
}

// 混全帯幺九
bool setup_hunquandaiyaojiu(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> bool_dist(0, 1);
    if (!pick_shunzi<false, true>(pai, rest, rule, mt)) return false;
    if (!pick_shunzi<false, true>(pai, rest, rule, mt)) return false;
    if (bool_dist(mt) == 0) {
        if (!pick_kezi<false, false, true, true>(pai, rest, rule, mt)) return false;
        if (!setup_hule<false, true>(pai, rest, rule, mt)) return false;
    }
    else {
        if (!setup_hule<false, true, true, false, false>(pai, rest, rule, mt)) return false;
        if (!pick_jiangpai<false, false, true, true>(pai, rest, rule, mt)) return false;
    }
    make_fulou(pai, rest, fulou, mt);
    return true;
}

// 七対子
bool setup_qiduizi(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    // 同じ牌4枚は七対子にならないため、一時的に牌山から除く
    if (!pick_jiangpai(pai, rest, rule, mt)) return false;
    rest[pai[0]]--; rest[pai[1]]--;
    if (!pick_jiangpai(pai, rest, rule, mt)) return false;
    rest[pai[2]]--; rest[pai[3]]--;
    if (!pick_jiangpai(pai, rest, rule, mt)) return false;
    rest[pai[4]]--; rest[pai[5]]--;
    if (!pick_jiangpai(pai, rest, rule, mt)) return false;
    rest[pai[6]]--; rest[pai[7]]--;
    if (!pick_jiangpai(pai, rest, rule, mt)) return false;
    rest[pai[8]]--; rest[pai[9]]--;
    if (!pick_jiangpai(pai, rest, rule, mt)) return false;
    rest[pai[10]]--; rest[pai[11]]--;
    if (!pick_jiangpai(pai, rest, rule, mt)) return false;
    rest[pai[0]]++; rest[pai[1]]++;
    rest[pai[2]]++; rest[pai[3]]++;
    rest[pai[4]]++; rest[pai[5]]++;
    rest[pai[6]]++; rest[pai[7]]++;
    rest[pai[8]]++; rest[pai[9]]++;
    rest[pai[10]]++; rest[pai[11]]++;
    return true;
}

// 対々和
bool setup_duiduihu(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    if (!pick_kezi(pai, rest, rule, mt)) return false;
    if (!pick_kezi(pai, rest, rule, mt)) return false;
    if (!pick_kezi(pai, rest, rule, mt)) return false;
    if (!pick_kezi(pai, rest, rule, mt)) return false;
    if (!pick_jiangpai(pai, rest, rule, mt)) return false;
    make_fulou<1>(pai, rest, fulou, mt);
    return true;
}

// 三暗刻
bool setup_sananke(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> shunzi_dist(0, 2);
    if (!pick_kezi(pai, rest, rule, mt)) return false;
    if (!pick_kezi(pai, rest, rule, mt)) return false;
    if (!pick_kezi(pai, rest, rule, mt)) return false;
    const int shunzi = shunzi_dist(mt);
    if (shunzi <= 1) {
        if (!pick_shunzi(pai, rest, rule, mt)) return false;
        if (shunzi == 1)
            fulou.emplace_back(make_chi(pai[9], pai[10], pai[11], mt));
    }
    else {
        if (!pick_kezi(pai, rest, rule, mt)) return false;
        fulou.emplace_back(make_peng_gang(pai[9], pai[10], pai[11], rest, mt));
    }
    if (!pick_jiangpai(pai, rest, rule, mt)) return false;
    if (fulou.size() > 0) {
        pai.erase(pai.begin() + 9, pai.begin() + 12);
    }
    return true;
}

// 三槓子
bool setup_sangangzi(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> shunzi_dist(0, 2);
    if (!pick_kezi(pai, rest, rule, mt)) return false;
    if (!pick_kezi(pai, rest, rule, mt)) return false;
    if (!pick_kezi(pai, rest, rule, mt)) return false;
    auto p1 = pai[0][1] == '0' ? to_string(pai[0][0], 5) : pai[0];
    auto p2 = pai[3][1] == '0' ? to_string(pai[3][0], 5) : pai[3];
    auto p3 = pai[6][1] == '0' ? to_string(pai[6][0], 5) : pai[6];
    if (pai[0][1] == '5' && pai[1][1] == '5' && pai[2][1] == '5' && rest[to_string(p1[0], 0)] > 0) p1 = to_string(p1[0], 0);
    if (pai[3][1] == '5' && pai[4][1] == '5' && pai[5][1] == '5' && rest[to_string(p2[0], 0)] > 0) p2 = to_string(p2[0], 0);
    if (pai[6][1] == '5' && pai[7][1] == '5' && pai[8][1] == '5' && rest[to_string(p3[0], 0)] > 0) p3 = to_string(p3[0], 0);
    if (rest[p1] > 0 && rest[p2] > 0 && rest[p3] > 0) {
        rest[p1]--;
        rest[p2]--;
        rest[p3]--;
        const int shunzi = shunzi_dist(mt);
        if (shunzi <= 1) {
            if (!pick_shunzi(pai, rest, rule, mt)) return false;
            if (shunzi == 1)
                fulou.emplace_back(make_chi(pai[9], pai[10], pai[11], mt));
        }
        else {
            if (!pick_kezi(pai, rest, rule, mt)) return false;
            fulou.emplace_back(make_peng_gang<false>(pai[9], pai[10], pai[11], rest, mt));
        }
        if (!pick_jiangpai(pai, rest, rule, mt)) return false;
        if (fulou.size() > 0) {
            pai.erase(pai.begin() + 9, pai.begin() + 12);
        }
        fulou.emplace_back(make_gang<true>(pai[0], pai[1], pai[2], p1, mt));
        fulou.emplace_back(make_gang<true>(pai[3], pai[4], pai[5], p2, mt));
        fulou.emplace_back(make_gang<true>(pai[6], pai[7], pai[8], p3, mt));
        pai.erase(pai.begin(), pai.begin() + 9);
        return true;
    }
    return false;
}

// 三色同刻
bool setup_sansetongke(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> dist4(0, 3);
    constexpr int N = 9;
    std::uniform_int_distribution<int> kezi_dist(0, N - 1);
    std::uniform_int_distribution<int> suit_dist(0, 2);
    int start = kezi_dist(mt);
    for (int i = 0; i < N; i++) {
        int n = (start + i) % N;
        const auto p1 = to_string('m', n + 1);
        const auto p2 = to_string('p', n + 1);
        const auto p3 = to_string('s', n + 1);
        if (n == 4 && rule.hongpai[0] && rule.hongpai[1] && rule.hongpai[2]) {
            int n_pai[] = { 3, 3, 3 };
            const int hongpai_s = suit_dist(mt);
            const auto hongpai = to_string(suit[hongpai_s], 0);
            const bool is_hongpai = rest[hongpai] > 0 && dist4(mt) == 0;
            if (is_hongpai)
                n_pai[hongpai_s]--;
            if (rest[p1] >= n_pai[0] && rest[p2] >= n_pai[1] && rest[p3] >= n_pai[2]) {
                for (int j = 0; j < n_pai[0]; j++) pai.emplace_back(p1);
                if (is_hongpai && hongpai_s == 0) {
                    pai.emplace_back(hongpai);
                    rest[hongpai]--;
                }
                for (int j = 0; j < n_pai[1]; j++) pai.emplace_back(p2);
                if (is_hongpai && hongpai_s == 1) {
                    pai.emplace_back(hongpai);
                    rest[hongpai]--;
                }
                for (int j = 0; j < n_pai[2]; j++) pai.emplace_back(p3);
                if (is_hongpai && hongpai_s == 2) {
                    pai.emplace_back(hongpai);
                    rest[hongpai]--;
                }
                rest[p1] -= n_pai[0];
                rest[p2] -= n_pai[1];
                rest[p3] -= n_pai[2];
                break;
            }
        }
        else if (rest[p1] >= 3 && rest[p2] >= 3 && rest[p3] >= 3) {
            pai.emplace_back(p1); pai.emplace_back(p1); pai.emplace_back(p1);
            pai.emplace_back(p2); pai.emplace_back(p2); pai.emplace_back(p2);
            pai.emplace_back(p3); pai.emplace_back(p3); pai.emplace_back(p3);
            rest[p1] -= 3;
            rest[p2] -= 3;
            rest[p3] -= 3;
            break;
        }
    }
    if (pai.size() > 0) {
        if (!pick_shunzi(pai, rest, rule, mt)) return false;
        if (!setup_hule(pai, rest, rule, mt)) return false;
        make_fulou(pai, rest, fulou, mt);
        return true;
    }
    return false;
}

// 混老頭
bool setup_hunlaotou(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> bool_dist(0, 1);
    if (!pick_kezi<false, true, false>(pai, rest, rule, mt)) return false;
    if (!pick_kezi<false, true>(pai, rest, rule, mt)) return false;
    if (!pick_kezi<false, true>(pai, rest, rule, mt)) return false;
    if (bool_dist(mt) == 0) {
        if (!pick_kezi<false, false, true, true>(pai, rest, rule, mt)) return false;
        if (!pick_jiangpai<false, true>(pai, rest, rule, mt)) return false;
    }
    else {
        if (!pick_kezi<false, true>(pai, rest, rule, mt)) return false;
        if (!pick_jiangpai<false, false, true, true>(pai, rest, rule, mt)) return false;
    }
    make_fulou<1>(pai, rest, fulou, mt);
    return true;
}

// 小三元
bool setup_xiaosanyuan(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> bool_dist(0, 1);
    if (!pick_kezi<false, false, false, true>(pai, rest, rule, mt)) return false;
    if (!pick_kezi<false, false, false, true>(pai, rest, rule, mt)) return false;
    rest["z5"] -= 4; rest["z6"] -= 4; rest["z7"] -= 4;
    if (bool_dist(mt) == 0) {
        if (!pick_shunzi(pai, rest, rule, mt)) return false;
        if (!setup_hule<false, false, true, false, false>(pai, rest, rule, mt)) return false;
        rest["z5"] += 4; rest["z6"] += 4; rest["z7"] += 4;
        if (!pick_jiangpai<false, false, false, true>(pai, rest, rule, mt)) return false;
        make_fulou(pai, rest, fulou, mt);
    }
    else {
        if (!setup_hule<false, false, true, false, false>(pai, rest, rule, mt)) return false;
        rest["z5"] += 4; rest["z6"] += 4; rest["z7"] += 4;
        if (!pick_jiangpai<false, false, false, true>(pai, rest, rule, mt)) return false;
        make_fulou<1>(pai, rest, fulou, mt);
    }
    return true;
}

// 混一色
bool setup_hunyise(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> bool_dist(0, 1);
    std::uniform_int_distribution<int> suit_dist(0, 2);
    const int s_i = suit_dist(mt);
    const auto is_kezi_zipai = bool_dist(mt) == 0;
    int n_kezi = 0;
    if (is_kezi_zipai) {
        if (!pick_kezi<false, false, true, true>(pai, rest, rule, mt)) return false;
        n_kezi++;
    }
    if (bool_dist(mt) == 0) {
        // 順子
        if (!pick_shunzi(s_i, pai, rest, rule, mt)) return false;
    }
    else {
        // 刻子
        if (!pick_kezi<false>(s_i, pai, rest, rule, mt)) return false;
        n_kezi++;
    }
    while (pai.size() < 12) {
        if (bool_dist(mt) == 0) {
            // 順子
            if (!pick_shunzi(s_i, pai, rest, rule, mt)) return false;
        }
        else {
            // 刻子
            if (!pick_kezi(s_i, pai, rest, rule, mt)) return false;
            n_kezi++;
        }
    }
    // 雀頭
    if (is_kezi_zipai) {
        if (!pick_jiangpai(s_i, pai, rest, rule, mt)) return false;
    }
    else {
        if (!pick_jiangpai<false, false, true, true>(pai, rest, rule, mt)) return false;
    }
    if (n_kezi >= 3)
        make_fulou<1>(pai, rest, fulou, mt);
    else
        make_fulou(pai, rest, fulou, mt);
    return true;
}

// 純全帯幺九
bool setup_chunquandaiyaojiu(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    if (!pick_shunzi<false, true>(pai, rest, rule, mt)) return false;
    if (!setup_hule<false, true, false>(pai, rest, rule, mt)) return false;
    make_fulou(pai, rest, fulou, mt);
    return true;
}

// 二盃口
bool setup_erbeikou(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    if (!setup_beikou(pai, rest, rule, mt)) return false;
    if (!setup_beikou(pai, rest, rule, mt)) return false;
    if (!pick_jiangpai(pai, rest, rule, mt)) return false;
    return true;
}

// 清一色
bool setup_qingyise(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> bool_dist(0, 1);
    std::uniform_int_distribution<int> suit_dist(0, 2);
    const int s_i = suit_dist(mt);
    int n_kezi = 0;
    while (pai.size() < 12) {
        if (bool_dist(mt) == 0) {
            // 順子
            if (!pick_shunzi(s_i, pai, rest, rule, mt)) return false;
        }
        else {
            // 刻子
            if (!pick_kezi<false>(s_i, pai, rest, rule, mt)) return false;
            n_kezi++;
        }
    }
    // 雀頭
    if (!pick_jiangpai<false>(s_i, pai, rest, rule, mt)) return false;
    if (n_kezi == 4)
        make_fulou<1>(pai, rest, fulou, mt);
    else
        make_fulou(pai, rest, fulou, mt);
    return true;
}

// 国士無双
bool setup_guoshiwushuang(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> guoshi_dist(0, 11);
    static const std::string guoshi[] = { "m1", "m9", "p1", "p9", "s1", "s9", "z1", "z2", "z3", "z4", "z5", "z6", "z7" };
    for (const auto& p : guoshi) {
        if (rest[p] < 1) return false;
        pai.emplace_back(p);
        rest[p]--;
    }
    const auto& p = guoshi[guoshi_dist(mt)];
    if (rest[p] < 1) return false;
    pai.emplace_back(p);
    rest[p]--;
    return true;
}

// 四暗刻
bool setup_sianke(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    if (!pick_kezi(pai, rest, rule, mt)) return false;
    if (!pick_kezi(pai, rest, rule, mt)) return false;
    if (!pick_kezi(pai, rest, rule, mt)) return false;
    if (!pick_kezi(pai, rest, rule, mt)) return false;
    if (!pick_jiangpai(pai, rest, rule, mt)) return false;
    return true;
}

// 大三元
bool setup_dasanyuan(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    static const std::string yuan[] = { "z5", "z6", "z7" };
    for (const auto& p : yuan) {
        if (rest[p] < 3) return false;
        pai.emplace_back(p);
        pai.emplace_back(p);
        pai.emplace_back(p);
        rest[p] -= 3;
    }
    if (!setup_hule(pai, rest, rule, mt)) return false;
    make_fulou(pai, rest, fulou, mt);
    return true;
}

// 小四喜
bool setup_xiaosixi(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> sixi_dist(0, 3);
    static const std::string sixi[] = { "z1", "z2", "z3", "z4" };
    const auto sixi_i = sixi_dist(mt);
    for (int i = 0; i < 4; i++) {
        if (i == sixi_i) continue;
        const auto& p = sixi[i];
        if (rest[p] < 3) return false;
        pai.emplace_back(p);
        pai.emplace_back(p);
        pai.emplace_back(p);
        rest[p] -= 3;
    }
    const auto& p = sixi[sixi_i];
    if (rest[p] < 2) return false;
    rest[p] -= 2;
    if (!setup_hule<false, false, true, false, false>(pai, rest, rule, mt)) return false;
    pai.emplace_back(p);
    pai.emplace_back(p);
    make_fulou(pai, rest, fulou, mt);
    return true;
}

// 大四喜
bool setup_dasixi(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> sixi_dist(0, 3);
    static const std::string sixi[] = { "z1", "z2", "z3", "z4" };
    for (const auto& p : sixi) {
        if (rest[p] < 3) return false;
        pai.emplace_back(p);
        pai.emplace_back(p);
        pai.emplace_back(p);
        rest[p] -= 3;
    }
    if (!setup_hule(pai, rest, rule, mt)) return false;
    make_fulou(pai, rest, fulou, mt);
    return true;
}

// 字一色
bool setup_ziyise(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    if (!pick_kezi<false, false, true, true>(pai, rest, rule, mt)) return false;
    if (!pick_kezi<false, false, true, true>(pai, rest, rule, mt)) return false;
    if (!pick_kezi<false, false, true, true>(pai, rest, rule, mt)) return false;
    if (!pick_kezi<false, false, true, true>(pai, rest, rule, mt)) return false;
    if (!pick_jiangpai<false, false, true, true>(pai, rest, rule, mt)) return false;
    make_fulou(pai, rest, fulou, mt);
    return true;
}

// 緑一色
bool setup_lvyise(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    if (!setup_hule<false, false, true, true>(pai, rest, rule, mt)) return false;
    make_fulou(pai, rest, fulou, mt);
    return true;
}

// 清老頭
bool setup_qinglaotou(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    if (!pick_kezi<false, true, false>(pai, rest, rule, mt)) return false;
    if (!pick_kezi<false, true, false>(pai, rest, rule, mt)) return false;
    if (!pick_kezi<false, true, false>(pai, rest, rule, mt)) return false;
    if (!pick_kezi<false, true, false>(pai, rest, rule, mt)) return false;
    if (!pick_jiangpai<false, true, false>(pai, rest, rule, mt)) return false;
    make_fulou(pai, rest, fulou, mt);
    return true;
}

// 四槓子
bool setup_sigangzi(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
    if (!setup_sianke(pai, rest, rule, mt)) return false;
    auto p1 = pai[0][1] == '0' ? to_string(pai[0][0], 5) : pai[0];
    auto p2 = pai[3][1] == '0' ? to_string(pai[3][0], 5) : pai[3];
    auto p3 = pai[6][1] == '0' ? to_string(pai[6][0], 5) : pai[6];
    auto p4 = pai[9][1] == '0' ? to_string(pai[9][0], 5) : pai[9];
    if (pai[0][1] == '5' && pai[1][1] == '5' && pai[2][1] == '5' && rest[to_string(p1[0], 0)] > 0) p1 = to_string(p1[0], 0);
    if (pai[3][1] == '5' && pai[4][1] == '5' && pai[5][1] == '5' && rest[to_string(p2[0], 0)] > 0) p2 = to_string(p2[0], 0);
    if (pai[6][1] == '5' && pai[7][1] == '5' && pai[8][1] == '5' && rest[to_string(p3[0], 0)] > 0) p3 = to_string(p3[0], 0);
    if (pai[9][1] == '5' && pai[10][1] == '5' && pai[11][1] == '5' && rest[to_string(p4[0], 0)] > 0) p4 = to_string(p4[0], 0);
    if (rest[p1] > 0 && rest[p2] > 0 && rest[p3] > 0 && rest[p4] > 0) {
        fulou.emplace_back(make_gang<true>(pai[0], pai[1], pai[2], p1, mt));
        fulou.emplace_back(make_gang<true>(pai[3], pai[4], pai[5], p2, mt));
        fulou.emplace_back(make_gang<true>(pai[6], pai[7], pai[8], p3, mt));
        fulou.emplace_back(make_gang<true>(pai[9], pai[10], pai[11], p4, mt));
        rest[p1]--;
        rest[p2]--;
        rest[p3]--;
        rest[p4]--;
        pai.erase(pai.begin(), pai.begin() + 12);
        return true;
    }
    return false;
}

// 九蓮宝燈
bool setup_jiulianbaodeng(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> suit_dist(0, 2);
    std::uniform_int_distribution<int> n_dist(1, 9);
    const int s_i = suit_dist(mt);
    const char s = suit[s_i];
    for (const int i : { 1, 9 }) {
        const auto p = to_string(s, i);
        if (rest[p] < 3) return false;
        pai.emplace_back(p);
        pai.emplace_back(p);
        pai.emplace_back(p);
        rest[p] -= 3;
    }
    for (int i = 2; i < 9; i++) {
        const auto p = to_string(s, i);
        if (rest[p] < 1) return false;
        pai.emplace_back(p);
        rest[p]--;
    }
    const auto p = to_string(s, n_dist(mt));
    if (rest[p] < 1) return false;
    pai.emplace_back(p);
    rest[p]--;
    return true;
}

// ランダムな役の手牌を作る
bool make_hule(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const int zhuangfeng, const int l, const Rule& rule, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> hupai_dist(0, 29);

    switch (hupai_dist(mt)) {
    case 0:
        // 場風
        return setup_zhuangfeng(pai, rest, fulou, zhuangfeng, rule, mt);
    case 1:
        // 自風
        return setup_menfeng(pai, rest, fulou, l, rule, mt);
    case 2:
        // 翻牌
        return setup_fanpai(pai, rest, fulou, rule, mt);
        break;
    case 3:
        // 平和
        return setup_pinghe(pai, rest, zhuangfeng, l, rule, mt);
    case 4:
        // 断幺九
        return setup_duanyaojiu(pai, rest, rule, mt);
    case 5:
        // 一盃口
        return setup_yibeikou(pai, rest, rule, mt);
    case 6:
        // 三色同順
        return setup_sansetongshun(pai, rest, fulou, rule, mt);
    case 7:
        // 一気通貫
        return setup_yiqitongguan(pai, rest, fulou, rule, mt);
    case 8:
        // 混全帯幺九
        return setup_hunquandaiyaojiu(pai, rest, fulou, rule, mt);
    case 9:
        // 七対子
        return setup_qiduizi(pai, rest, rule, mt);
    case 10:
        // 対々和
        return setup_duiduihu(pai, rest, fulou, rule, mt);
    case 11:
        // 三暗刻
        return setup_sananke(pai, rest, fulou, rule, mt);
    case 12:
        // 三槓子
        return setup_sangangzi(pai, rest, fulou, rule, mt);
    case 13:
        // 三色同刻
        return setup_sansetongke(pai, rest, fulou, rule, mt);
    case 14:
        // 混老頭
        return setup_hunlaotou(pai, rest, fulou, rule, mt);
    case 15:
        // 小三元
        return setup_xiaosanyuan(pai, rest, fulou, rule, mt);
    case 16:
        // 混一色
        return setup_hunyise(pai, rest, fulou, rule, mt);
    case 17:
        // 純全帯幺九
        return setup_chunquandaiyaojiu(pai, rest, fulou, rule, mt);
    case 18:
        // 二盃口
        return setup_erbeikou(pai, rest, fulou, rule, mt);
    case 19:
        // 清一色
        return setup_qingyise(pai, rest, fulou, rule, mt);
    case 20:
        // 国士無双
        return setup_guoshiwushuang(pai, rest, rule, mt);
    case 21:
        // 四暗刻
        return setup_sianke(pai, rest, rule, mt);
    case 22:
        // 大三元
        return setup_dasanyuan(pai, rest, fulou, rule, mt);
    case 23:
        // 小四喜
        return setup_xiaosixi(pai, rest, fulou, rule, mt);
    case 24:
        // 大四喜
        return setup_dasixi(pai, rest, fulou, rule, mt);
    case 25:
        // 字一色
        return setup_ziyise(pai, rest, fulou, rule, mt);
    case 26:
        // 緑一色
        return setup_lvyise(pai, rest, fulou, rule, mt);
    case 27:
        // 清老頭
        return setup_qinglaotou(pai, rest, fulou, rule, mt);
    case 28:
        // 四槓子
        return setup_sigangzi(pai, rest, fulou, rule, mt);
    case 29:
        // 九蓮宝燈
        return setup_jiulianbaodeng(pai, rest, rule, mt);
    }
    return false;
}

// 1枚捨てて聴牌にする
void discard_one(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> dist14(0, 13);
    const int discard_i = dist14(mt);
    if (discard_i < pai.size()) {
        const auto& p = pai[discard_i];
        rest[p]++;
        pai.erase(pai.begin() + discard_i);
    }
    else {
        // 副露も対象にする
        const int fulou_i = (discard_i - (int)pai.size()) / 3;
        const int fulou_j = (discard_i - (int)pai.size()) % 3;
        auto& m = fulou[fulou_i];
        int j = 0;
        for (std::sregex_iterator it(m.begin() + 1, m.end(), re_digit), end; it != end; ++it, j++) {
            const auto p = m[0] + it->str();
            if (j == fulou_j)
                rest[p]++;
            else if (j == 3) // 槓
                rest[p]++;
            else
                pai.emplace_back(p);
        }
        fulou.erase(fulou.begin() + fulou_i);
    }
}

// N向聴にする
void make_n_xiangting(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, BetaDistribution& xiangting_dist, std::mt19937_64& mt) {
    // ベータ分布からNをサンプリングし、牌山と入れ替える

    // 牌山シャッフル
    std::vector<std::string> rest_pai;
    for (auto itr = rest.begin(); itr != rest.end(); ++itr) {
        for (int i = 0; i < itr->second; i++) {
            rest_pai.emplace_back(itr->first);
        }
    }
    std::shuffle(rest_pai.begin(), rest_pai.end(), mt);

    // N枚を牌山と入れ替え 副露も対象にする
    const int N = std::min(xiangting_dist(mt), (int)(pai.size() - 1));
    const int pai_size = (int)pai.size();
    std::vector<int> indexes{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    std::shuffle(indexes.begin(), indexes.end(), mt);
    std::set<int> erace_fulou_i;
    std::vector<std::vector<std::string>> fulou_pai;
    for (const auto& m : fulou) {
        auto& pai_ = fulou_pai.emplace_back();
        for (std::sregex_iterator it(m.begin() + 1, m.end(), re_digit), end; it != end; ++it) {
            pai_.emplace_back(m[0] + it->str());
        }
    }
    for (int n = 0; n < N; n++) {
        const int exchange_i = indexes[n];
        if (exchange_i < pai_size) {
            const auto& p = pai[exchange_i];
            rest[p]++;
            pai[exchange_i] = rest_pai[n];
            rest[pai[exchange_i]]--;
        }
        else {
            const int fulou_i = (exchange_i - pai_size) / 3;
            const int fulou_j = (exchange_i - pai_size) % 3;
            auto& pai_ = fulou_pai[fulou_i];

            const auto& p = pai_[fulou_j];
            rest[p]++;
            pai_[fulou_j] = rest_pai[n];
            rest[pai_[fulou_j]]--;

            erace_fulou_i.emplace(fulou_i);
        }
    }
    for (auto itr = erace_fulou_i.rbegin(); itr != erace_fulou_i.rend(); ++itr) {
        const int fulou_i = *itr;
        auto& pai_ = fulou_pai[fulou_i];
        for (int j = 0; j < pai_.size(); j++) {
            const auto& p = pai_[j];
            if (j == 3) // 槓
                rest[p]++;
            else
                pai.emplace_back(p);
        }
        fulou.erase(fulou.begin() + fulou_i);
    }
}

// ランダムな役のN向聴の状態から開始するゲームを生成する
void random_game_state(Game& game, const int n_xiangting, std::mt19937_64& mt) {
    std::uniform_int_distribution<int> dist4(0, 3);
    std::uniform_int_distribution<int> bool_dist(0, 1);
    std::map<std::string, int> rest{
        { "m1", 4 }, { "m2", 4 }, { "m3", 4 }, { "m4", 4 }, { "m5", 4 }, { "m6", 4 }, { "m7", 4 }, { "m8", 4 }, { "m9", 4 },
        { "p1", 4 }, { "p2", 4 }, { "p3", 4 }, { "p4", 4 }, { "p5", 4 }, { "p6", 4 }, { "p7", 4 }, { "p8", 4 }, { "p9", 4 },
        { "s1", 4 }, { "s2", 4 }, { "s3", 4 }, { "s4", 4 }, { "s5", 4 }, { "s6", 4 }, { "s7", 4 }, { "s8", 4 }, { "s9", 4 },
        { "z1", 4 }, { "z2", 4 }, { "z3", 4 }, { "z4", 4 }, { "z5", 4 }, { "z6", 4 }, { "z7", 4 }
    };
    for (const auto s : { 'm', 'p', 's' }) {
        for (int i = 0; i < game.rule().hongpai_(s); i++) {
            rest[to_string(s, 5)]--;
            rest[to_string(s, 0)]++;
        }
    }

    BetaDistribution xiangting_dist{ n_xiangting, 12 };

    const int lunban = dist4(mt);

    struct PlayerState {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::vector<std::string> he;
        std::vector<std::string> tingpai;
        std::vector<std::string> tingpai_fulou;
    };

    // ランダムな役の手牌を生成
    std::array<PlayerState, 4> player_state;
    int start = dist4(mt);
    int n_gang = 0;
    for (int i = 0; i < 4; i++) {
        int l = (start + i) % 4;
        auto& pai = player_state[l].pai;
        auto& fulou = player_state[l].fulou;
        while (true) {
            auto rest_tmp = rest;
            if (make_hule(pai, rest, fulou, game.zhuangfeng(), l, game.rule(), mt)) {
                // 槓は3まで
                auto n = (int)count_if(fulou, [](const auto& m) { return std::regex_match(m, re_gang); });
                if (n == 4) {
                    // 四槓子は副露を1つ戻す
                    const auto& m = fulou.back();
                    const auto p = std::string(1, m[0]) + m[1];
                    rest[p]++;
                    for (std::sregex_iterator it(m.begin() + 2, m.end(), re_digit), end; it != end; ++it) {
                        const auto p = m[0] + it->str();
                        pai.emplace_back(p);
                    }
                    fulou.pop_back();
                    n = 3;
                }
                if (n_gang + n < 4) {
                    n_gang += n;
                    break;
                }
            }
            pai.clear();
            fulou.clear();
            rest = rest_tmp;
        }

        // 1枚捨てて聴牌にする
        discard_one(pai, rest, fulou, mt);
        player_state[l].tingpai = pai;
        player_state[l].tingpai_fulou = fulou;

        // N向聴にする
        make_n_xiangting(pai, rest, fulou, xiangting_dist, mt);
    }

    // 捨て牌を生成
    // 平均9巡で聴牌すると仮定し、向聴数から捨て牌の基本枚数を決める
    BetaDistribution he_dist{ std::max(9 - n_xiangting * 9 / 12, 1), 12 };
    const int base_he_num = he_dist(mt) + 1;
    // 副露と矛盾しないようにする
    std::array<int, 4> he_num{};
    static const std::regex re_fulou{ R"(\d[\+\=\-])" };
    int max_he_num = 0;
    for (int i = 0; i < 4; i++) {
        for (const auto& m : player_state[i].fulou) {
            std::smatch match;
            if (std::regex_search(m, match, re_fulou)) {
                const auto s = m[0];
                const auto n = match.str()[0];
                const auto d = match.str()[1];
                const int l = (i + (d == '+' ? 1 : d == '=' ? 2 : 3)) % 4;
                player_state[l].he.emplace_back(std::string(1, s) + n);
                if (d == '+')
                    he_num[i]++;
                else if (d == '=') {
                    he_num[i]++;
                    he_num[(i + 1) % 4]++;
                }
                else if (d == '-') {
                    he_num[i]++;
                    he_num[(i + 1) % 4]++;
                    he_num[(i + 2) % 4]++;
                }
            }
        }
        he_num[i] += base_he_num;
        if (i < lunban)
            he_num[i]++;
        if (he_num[i] > max_he_num)
            max_he_num = he_num[i];
    }
    if (max_he_num > 14) {
        for (auto& num : he_num)
            num -= max_he_num - 14;
    }

    // 牌山シャッフル
    std::vector<std::string> rest_pai;
    for (auto itr = rest.begin(); itr != rest.end(); ++itr) {
        for (int i = 0; i < itr->second; i++) {
            rest_pai.emplace_back(itr->first);
        }
    }
    std::shuffle(rest_pai.begin(), rest_pai.end(), mt);
    // 聴牌の待ち牌を除いて河を生成
    for (int i = 0; i < 4; i++) {
        std::set<std::string> except;
        Shoupai shoupai_{ player_state[i].tingpai, player_state[i].tingpai_fulou };
        for (const auto& p : tingpai(shoupai_))
            except.emplace(p);

        auto& he = player_state[i].he;
        for (int j = 0; j < he_num[i]; j++) {
            for (auto itr = rest_pai.rbegin(); itr != rest_pai.rend(); ++itr) {
                if (except.find(*itr) == except.end()) {
                    he.emplace_back(*itr);
                    rest_pai.erase((++itr).base());
                    break;
                }
            }
        }
        // シャッフル
        std::shuffle(he.begin(), he.end(), mt);
    }

    // 手牌を生成
    std::array<Shoupai, 4> shoupai;
    for (int i = 0; i < 4; i++) {
        shoupai[i] = { player_state[i].pai, player_state[i].fulou };
        auto& shoupai_ = shoupai[i];
        // 聴牌している場合、確率的に立直
        if (bool_dist(mt) == 0 && xiangting(shoupai_) == 0) {
            shoupai_.set_lizhi(true);
            player_state[i].he.back() += '*';
        }
    }

    // 河を生成
    std::array<He, 4> he;
    for (int i = 0; i < 4; i++) {
        auto& he_ = he[i];
        for (const auto& p : player_state[i].he) {
            he_.dapai(p);
        }
    }

    // 牌山を生成
    Shan shan{ rest_pai, game.rule()};
    // 槓自摸、開槓の処理
    for (int i = 0; i < 4; i++) {
        for (const auto& m : player_state[i].fulou) {
            if (std::regex_match(m, re_gang)) {
                auto p = shan.gangzimo();
                shan.kaigang();
                shan.pai().emplace_back(std::move(p));
            }
        }
    }

    game.set(shoupai, he, shan, (lunban + 3) % 4);
}
