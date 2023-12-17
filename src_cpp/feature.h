#pragma once

#include "game.h"

constexpr int N_CHANNELS_STATUS = 5;
constexpr int N_CHANNELS_SHOUPAI = 4 + 3;
constexpr int N_CHANNELS_FULOU = 4 + 3 + 3 + 4;
constexpr int N_CHANNELS_PAI = 4 + 3;
constexpr int N_CHANNELS_TINGPAI = 1;
constexpr int N_CHANNELS_LIZHI = 1;
constexpr int N_CHANNELS_HE = 7 + 4;
constexpr int N_CHANNELS_TAJIADAPAI = 4;
constexpr int N_CHANNELS_BAOPAI = 4 + 3;
constexpr int N_CHANNELS_MENFENG = 4;
constexpr int N_CHANNELS_ZHUANGFENG = 4;
constexpr int N_CHANNELS_PAISHU = 1;

constexpr int MAX_PAISHU = 136 - 14 - 13 * 4 - 1;

constexpr int N_CHANNELS_PUBLIC
= N_CHANNELS_STATUS
+ N_CHANNELS_SHOUPAI
+ N_CHANNELS_FULOU * 4
+ N_CHANNELS_PAI
+ N_CHANNELS_PAI
+ N_CHANNELS_TINGPAI
+ N_CHANNELS_LIZHI * 4
+ N_CHANNELS_HE * 4
+ N_CHANNELS_TAJIADAPAI
+ N_CHANNELS_BAOPAI
+ N_CHANNELS_MENFENG
+ N_CHANNELS_ZHUANGFENG
+ N_CHANNELS_PAISHU;

constexpr int N_CHANNELS_SHAN = 4 + 3;

constexpr int N_CHANNELS_PRIVATE
= N_CHANNELS_SHOUPAI * 3
+ N_CHANNELS_TINGPAI * 3
+ N_CHANNELS_SHAN
+ N_CHANNELS_BAOPAI;

enum class Action {
    // 鳴かない、ロンしない
    NO_ACTION,
    // 打牌
    DAPAI_M1, DAPAI_M2, DAPAI_M3, DAPAI_M4, DAPAI_M5, DAPAI_M6, DAPAI_M7, DAPAI_M8, DAPAI_M9, DAPAI_M0,
    DAPAI_P1, DAPAI_P2, DAPAI_P3, DAPAI_P4, DAPAI_P5, DAPAI_P6, DAPAI_P7, DAPAI_P8, DAPAI_P9, DAPAI_P0,
    DAPAI_S1, DAPAI_S2, DAPAI_S3, DAPAI_S4, DAPAI_S5, DAPAI_S6, DAPAI_S7, DAPAI_S8, DAPAI_S9, DAPAI_S0,
    DAPAI_Z1, DAPAI_Z2, DAPAI_Z3, DAPAI_Z4, DAPAI_Z5, DAPAI_Z6, DAPAI_Z7,
    // 自摸切り
    DAPAI_ZIMO,
    // 立直
    LIZHI_M1, LIZHI_M2, LIZHI_M3, LIZHI_M4, LIZHI_M5, LIZHI_M6, LIZHI_M7, LIZHI_M8, LIZHI_M9, LIZHI_M0,
    LIZHI_P1, LIZHI_P2, LIZHI_P3, LIZHI_P4, LIZHI_P5, LIZHI_P6, LIZHI_P7, LIZHI_P8, LIZHI_P9, LIZHI_P0,
    LIZHI_S1, LIZHI_S2, LIZHI_S3, LIZHI_S4, LIZHI_S5, LIZHI_S6, LIZHI_S7, LIZHI_S8, LIZHI_S9, LIZHI_S0,
    LIZHI_Z1, LIZHI_Z2, LIZHI_Z3, LIZHI_Z4, LIZHI_Z5, LIZHI_Z6, LIZHI_Z7,
    LIZHI_ZIMO,
    // チー
    CHI_L, CHI_Z, CHI_R,
    // チー(手牌の赤牌を含める)
    CHI_L_H, CHI_Z_H, CHI_R_H,
    // ポン
    PENG,
    // ポン(手牌の赤牌を含める)
    PENG_H,
    // カン
    GANG,
    // 暗槓もしくは加槓
    GANG_M1, GANG_M2, GANG_M3, GANG_M4, GANG_M5, GANG_M6, GANG_M7, GANG_M8, GANG_M9,
    GANG_P1, GANG_P2, GANG_P3, GANG_P4, GANG_P5, GANG_P6, GANG_P7, GANG_P8, GANG_P9,
    GANG_S1, GANG_S2, GANG_S3, GANG_S4, GANG_S5, GANG_S6, GANG_S7, GANG_S8, GANG_S9,
    GANG_Z1, GANG_Z2, GANG_Z3, GANG_Z4, GANG_Z5, GANG_Z6, GANG_Z7,
    // 和了
    HULE,
};
constexpr size_t N_ACTIONS = (size_t)Action::HULE + 1;
inline Action operator+(const Action action, const int i) {
    return (Action)((const int)action + i);
}


typedef float channel_t[9][4];

void status_featuers(const Game& game, const int lunban, channel_t* data);
void shoupai_features(const Shoupai& shoupai, channel_t* data);
void fulou_features(const Shoupai& shoupai, channel_t* data);
void pai_features(const std::string& pai, channel_t* data);
void he_features(const He& he, channel_t* data);
void tajiadapai_features(const Game& game, const int lunban, channel_t* data);
void baopai_features(const std::vector<std::string>& baopai, channel_t* data);
void public_features(const Game& game, const int lunban, channel_t* data);
void shan_features(const Shan& shan, channel_t* data);
void private_features(const Game& game, const int lunban, channel_t* data);

inline int index_of(const char s) {
    switch (s)
    {
    case 'm':
        return 0;
    case 'p':
        return 1;
    case 's':
        return 2;
    case 'z':
        return 3;
    default:
        throw std::invalid_argument(std::string(1, s));
    }
}

inline int replace_honpai(std::string& m) {
    int n_hongpai = 0;
    for (size_t i = 1; i < m.size(); i++) {
        if (m[i] == '0') {
            m[i] = '5';
            n_hongpai++;
        }
    }
    return n_hongpai;
}

inline void fill_channel(channel_t* data, const float val) {
    std::fill(reinterpret_cast<float*>(data), reinterpret_cast<float*>(data + 1), val);
}
