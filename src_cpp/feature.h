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

void status_featuers(const Game& game, const int lunban, float(*data)[9][4]);
void shoupai_features(const Shoupai& shoupai, float(*data)[9][4]);
void fulou_features(const Shoupai& shoupai, float(*data)[9][4]);
void pai_features(const std::string& pai, float(*data)[9][4]);
void he_features(const He& he, float(*data)[9][4]);
void tajiadapai_features(const Game& game, const int lunban, float(*data)[9][4]);
void baopai_features(const std::vector<std::string>& baopai, float(*data)[9][4]);

void game_public_features(const Game& game, const int lunban, float(*data)[9][4]);

void shan_features(const Shan& shan, float(*data)[9][4]);

void game_private_features(const Game& game, const int lunban, float(*data)[9][4]);

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
