#include "feature.h"
#include "xiangting.h"

#include <cassert>

// 状態の特徴量
void status_featuers(const Game& game, const int lunban, channel_t* data) {
    // 打牌、副露x3他家
    const auto status = game.status();
    switch (status)
    {
    case Game::Status::ZIMO:
    case Game::Status::GANGZIMO:
        fill_channel(data, 1.0f);
        break;
    case Game::Status::DAPAI:
    {
        const int i = (game.lunban() + 4 - lunban) % 4;
        assert(i < 4);
        fill_channel(data + i, 1.0f);
        break;
    }
    case Game::Status::FULOU:
        fill_channel(data + 4, 1.0f);
        break;
    default:
        break;
    }
}

// 手牌の特徴量
void shoupai_features(const Shoupai& shoupai, channel_t* data) {
    // 手牌(牌種+赤牌) 自摸牌は含めない
    const auto& zimo = shoupai.zimo_();
    int suit = 0;
    for (const auto s : { 'm', 'p', 's', 'z' }) {
        const auto& bingpai = shoupai.bingpai(s);
        auto n_hongpai = s == 'z' ? 0 : bingpai[0];
        for (int n = 1; n < bingpai.size(); n++) {
            auto n_pai = bingpai[n];
            if (!shoupai.zimo_().empty()) {
                if (zimo.size() == 2 && zimo[0] == s && to_int(zimo[1]) == n) {
                    n_pai--;
                }
                if (n == 5 && zimo.size() == 2 && zimo[0] == s && zimo[1] == '0') {
                    n_pai--;
                    n_hongpai--;
                }
            }
            if (n_pai > 0) {
                std::fill_n(data[suit][n - 1], n_pai, 1.0f);
            }
            if (n_hongpai > 0) {
                std::fill_n(data[4 + suit][4], n_hongpai, 1.0f);
            }
        }
        suit++;
    }
}

// 副露の特徴量
void fulou_features(const Shoupai& shoupai, channel_t* data) {
    // 副露 順子 3(牌種)
    // 副露 刻子 4(牌種)
    // 副露 暗槓 4(牌種)
    // 副露 赤牌 3(牌種)
    int n_chi = 0;
    for (auto m : shoupai.fulou_()) {
        const auto s = m[0];
        const int suit = index_of(s);
        int n_hongpai = replace_honpai(m);
        // 暗槓
        if (std::regex_match(m, re_angang)) {
            const auto n = to_int(m[1]);
            std::fill_n(data[3 + 4 + suit][n - 1], 4, 1.0f);
        }
        // ポン、明槓
        else if (std::regex_match(m, re_peng_gang)) {
            int n = to_int(m[1]);
            int n_pai = 0;
            for (std::sregex_iterator it(m.begin() + 1, m.end(), re_digit), end; it != end; ++it) {
                n_pai++;
            }
            std::fill_n(data[3 + suit][n - 1], n_pai, 1.0f);
        }
        // チー
        else {
            int n = to_int(m[1]);
            for (int i = 0; i < 3; i++) {
                data[suit][n - 1 + i][n_chi] = 1.0f;
            }
            n_chi++;
        }
        // 赤牌
        if (n_hongpai > 0) {
            data[3 + 4 + 4 + suit][4][0] = 1.0f;
        }
    }
}

// 牌の特徴量
void pai_features(const std::string& pai, channel_t* data) {
    // 牌種4+赤牌3
    const auto s = pai[0];
    const int suit = index_of(s);
    int n = to_int(pai[1]);
    if (n == 0) {
        n = 5;
        data[4 + suit][4][0] = 1.0f;
    }
    data[suit][n - 1][0] = 1.0f;
}

// 河の特徴量
void he_features(const He& he, channel_t* data) {
    int pai_count[4][9]{};
    int lizhi_count[4][9]{};
    bool lizhi = false;
    for (const auto& pai : he.pai()) {
        // 牌種4+赤牌3
        const auto s = pai[0];
        const int suit = index_of(s);
        int n = to_int(pai[1]);
        if (n == 0) {
            n = 5;
            data[4 + suit][4][0] = 1.0f;
        }
        data[suit][n - 1][pai_count[suit][n - 1]++] = 1.0f;

        // 立直後捨て牌
        if (lizhi) {
            // 牌種4
            data[7 + suit][n - 1][lizhi_count[suit][n - 1]++] = 1.0f;
        }
        else if (pai.back() == '*')
            lizhi = true;
    }
}

// 他家の直前の捨て牌の特徴量
void tajiadapai_features(const Game& game, const int lunban, channel_t* data) {
    // 他家の通った捨て牌をロンするとフリテンになるため必要な特徴量
    for (int i = 1; i < 4; i++) {
        int l = (lunban + i) % 4;
        if (game.lunban() == l)
            break;
        if (game.he_(l).pai().size() == 0)
            break;
        const auto& pai = game.he_(l).pai().back();
        const auto s = pai[0];
        const int suit = index_of(s);
        int n = to_int(pai[1]);
        if (n == 0) {
            n = 5;
        }
        data[suit][n - 1][0] = 1.0f;
    }
}

// ドラの特徴量
void baopai_features(const std::vector<std::string>& baopai, channel_t* data) {
    int pai_count[4][9]{};
    for (const auto& p : baopai) {
        // 表示牌ではない現物のドラ
        const auto p_ = Shan::zhenbaopai(p);
        const auto s = p_[0];
        const int suit = index_of(s);
        int n = to_int(p_[1]);
        data[suit][n - 1][pai_count[suit][n - 1]++] = 1.0f;
        // 赤牌は表示牌
        if (p[1] == '0') {
            data[4 + suit][4][0] = 1.0f;
        }
    }
}

void public_features(const Game& game, const int lunban, channel_t* data) {
    // 状態
    status_featuers(game, lunban, data);
    data += N_CHANNELS_STATUS;

    // 手牌
    const auto& shoupai = game.shoupai_(lunban);
    shoupai_features(shoupai, data);
    data += N_CHANNELS_SHOUPAI;

    // 副露
    for (int i = 0; i < 4; i++) {
        int l = (lunban + i) % 4;
        fulou_features(game.shoupai_(l), data);
        data += N_CHANNELS_FULOU;
    }

    // 自摸牌
    if (game.lunban() == lunban && (game.status() == Game::Status::ZIMO || game.status() == Game::Status::GANGZIMO)) {
        pai_features(shoupai.zimo_(), data);
    }
    data += N_CHANNELS_PAI;

    // 他家打牌
    if (game.lunban() != lunban) {
        pai_features(game.dapai_(), data);
    }
    data += N_CHANNELS_PAI;

    // 聴牌
    if (xiangting(shoupai) == 0) {
        fill_channel(data, 1.0f);
    }
    data += N_CHANNELS_TINGPAI;

    // 立直
    for (int i = 0; i < 4; i++) {
        int l = (lunban + i) % 4;
        if (game.lizhi_(l))
            fill_channel(data, 1.0f);
        data += N_CHANNELS_LIZHI;
    }

    // 河牌
    for (int i = 0; i < 4; i++) {
        int l = (lunban + i) % 4;
        he_features(game.he_(l), data);
        data += N_CHANNELS_HE;
    }

    // 他家の直前の捨て牌
    tajiadapai_features(game, lunban, data);
    data += N_CHANNELS_TAJIADAPAI;

    // ドラ
    baopai_features(game.shan().baopai(), data);
    data += N_CHANNELS_BAOPAI;

    // 自風
    fill_channel(data + lunban, 1.0f);
    data += N_CHANNELS_MENFENG;

    // 場風
    fill_channel(data + game.zhuangfeng(), 1.0f);
    data += N_CHANNELS_ZHUANGFENG;

    // 残り牌数
    fill_channel(data, (float)game.shan().paishu() / MAX_PAISHU);
    data += N_CHANNELS_PAISHU;
}


// 残り牌の特徴量
void shan_features(const Shan& shan, channel_t* data) {
    int pai_count[4][9]{};
    const auto& shan_pai = shan.pai();
    for (auto itr = shan_pai.cbegin() + 14; itr != shan_pai.cend(); ++itr) {
        const auto& pai = *itr;
        const auto s = pai[0];
        const int suit = index_of(s);
        int n = to_int(pai[1]);
        if (n == 0) {
            n = 5;
            data[4 + suit][4][0] = 1.0f;
        }
        data[suit][n - 1][pai_count[suit][n - 1]++] = 1.0f;
    }
}

void private_features(const Game& game, const int lunban, channel_t* data) {
    // 他家の手牌
    for (int i = 1; i < 4; i++) {
        int l = (lunban + i) % 4;
        const auto& shoupai = game.shoupai_(l);
        shoupai_features(shoupai, data);
        data += N_CHANNELS_SHOUPAI;
    }

    // 他家の聴牌
    for (int i = 1; i < 4; i++) {
        int l = (lunban + i) % 4;
        const auto& shoupai = game.shoupai_(l);
        if (xiangting(shoupai) == 0) {
            fill_channel(data, 1.0f);
        }
        data += N_CHANNELS_TINGPAI;
    }

    // 残り牌
    shan_features(game.shan(), data);
    data += N_CHANNELS_SHAN;

    // 裏ドラ
    baopai_features(game.shan().libaopai_(), data);
}
