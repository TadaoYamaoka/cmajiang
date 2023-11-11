#include "pch.h"
#include "../src_cpp/feature.h"

template <size_t N>
void fill_channel(std::array<std::array<std::array<float, 4>, 9>, N>& data, size_t index, const float val) {
    for (size_t s = 0; s < 9; s++) {
        for (size_t n = 0; n < 4; n++) {
            data[index][s][n] = val;
        }
    }
}

template <size_t N>
void set_pai(std::array<std::array<std::array<float, 4>, 9>, N>& data, size_t index, const std::string& pai, const bool hongpai = true) {
    const auto s = pai[0];
    const int suit = index_of(s);
    int n = to_int(pai[1]);
    if (n == 0) {
        n = 5;
        if (hongpai)
            data[index + 4 + suit][4][0] = 1.0f;
    }
    int i = 0;
    for (; i < 4; i++)
        if (data[index + suit][(size_t)n - 1][i] == 0.0f)
            break;
    data[index + suit][(size_t)n - 1][i] = 1.0f;
}

template <size_t N>
void set_baopai(std::array<std::array<std::array<float, 4>, 9>, N>& data, size_t index, const std::string& baopai) {
    const auto pai = Shan::zhenbaopai(baopai);
    const auto s = pai[0];
    const int suit = index_of(s);
    int n = to_int(pai[1]);
    if (baopai[1] == '0') {
        data[index + 4 + suit][4][0] = 1.0f;
    }
    int i = 0;
    for (; i < 4; i++)
        if (data[index + suit][(size_t)n - 1][i] == 0.0f)
            break;
    data[index + suit][(size_t)n - 1][i] = 1.0f;
}

template <size_t N>
void diff_data(std::array<std::array<std::array<float, 4>, 9>, N>& expected, std::array<std::array<std::array<float, 4>, 9>, N>& data) {
    for (size_t index = 0; index < N; index++) {
        const auto& channel = data[index];
        for (size_t suit = 0; suit < 9; suit++) {
            for (size_t n = 0; n < 4; n++) {
                if (expected[index][suit][n] != data[index][suit][n]) {
                    std::cerr << index << "," << suit << "," << n
                        << ": expected " << expected[index][suit][n]
                        << ": data " << data[index][suit][n] << std::endl;
                }
            }
        }
    }
}

TEST(FeatureTest, fulou_features) {
    // 副露なし
    {
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_FULOU> data{};
        fulou_features(Shoupai("z7654s987p654m321"), (float(*)[9][4])(data.data()));
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_FULOU> expected{};
        EXPECT_EQ(expected, data);
    }
    // チー
    {
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_FULOU> data{};
        fulou_features(Shoupai("z1234,m123-,p45-6,s7-89"), (float(*)[9][4])(data.data()));
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_FULOU> expected{};
        expected[0][0][0] = 1.0f;  expected[0][1][0] = 1.0f; expected[0][2][0] = 1.0f;
        expected[1][3][1] = 1.0f;  expected[1][4][1] = 1.0f; expected[1][5][1] = 1.0f;
        expected[2][6][2] = 1.0f;  expected[2][7][2] = 1.0f; expected[2][8][2] = 1.0f;
        EXPECT_EQ(expected, data);
    }
    // ポン
    {
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_FULOU> data{};
        fulou_features(Shoupai("z4,m111-,p444=,s777+,z222-"), (float(*)[9][4])(data.data()));
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_FULOU> expected{};
        expected[3][0][0] = 1.0f; expected[3][0][1] = 1.0f; expected[3][0][2] = 1.0f;
        expected[4][3][0] = 1.0f; expected[4][3][1] = 1.0f; expected[4][3][2] = 1.0f;
        expected[5][6][0] = 1.0f; expected[5][6][1] = 1.0f; expected[5][6][2] = 1.0f;
        expected[6][1][0] = 1.0f; expected[6][1][1] = 1.0f; expected[6][1][2] = 1.0f;
        EXPECT_EQ(expected, data);
    }
    // カン
    {
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_FULOU> data{};
        fulou_features(Shoupai("z4,m1111-,p444=4,s7777+,z2222-"), (float(*)[9][4])(data.data()));
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_FULOU> expected{};
        expected[3][0][0] = 1.0f; expected[3][0][1] = 1.0f; expected[3][0][2] = 1.0f; expected[3][0][3] = 1.0f;
        expected[4][3][0] = 1.0f; expected[4][3][1] = 1.0f; expected[4][3][2] = 1.0f; expected[4][3][3] = 1.0f;
        expected[5][6][0] = 1.0f; expected[5][6][1] = 1.0f; expected[5][6][2] = 1.0f; expected[5][6][3] = 1.0f;
        expected[6][1][0] = 1.0f; expected[6][1][1] = 1.0f; expected[6][1][2] = 1.0f; expected[6][1][3] = 1.0f;
        EXPECT_EQ(expected, data);
    }
    // 暗槓
    {
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_FULOU> data{};
        fulou_features(Shoupai("z4,m1111,p4444,s7777,z2222"), (float(*)[9][4])(data.data()));
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_FULOU> expected{};
        expected[7][0][0] = 1.0f; expected[7][0][1] = 1.0f; expected[7][0][2] = 1.0f; expected[7][0][3] = 1.0f;
        expected[8][3][0] = 1.0f; expected[8][3][1] = 1.0f; expected[8][3][2] = 1.0f; expected[8][3][3] = 1.0f;
        expected[9][6][0] = 1.0f; expected[9][6][1] = 1.0f; expected[9][6][2] = 1.0f; expected[9][6][3] = 1.0f;
        expected[10][1][0] = 1.0f; expected[10][1][1] = 1.0f; expected[10][1][2] = 1.0f; expected[10][1][3] = 1.0f;
        EXPECT_EQ(expected, data);
    }
    // 赤牌(チー)
    {
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_FULOU> data{};
        fulou_features(Shoupai("z1234,m406-,p0-67,s3-40"), (float(*)[9][4])(data.data()));
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_FULOU> expected{};
        expected[0][3][0] = 1.0f;  expected[0][4][0] = 1.0f; expected[0][5][0] = 1.0f;
        expected[1][4][1] = 1.0f;  expected[1][5][1] = 1.0f; expected[1][6][1] = 1.0f;
        expected[2][2][2] = 1.0f;  expected[2][3][2] = 1.0f; expected[2][4][2] = 1.0f;
        expected[11][4][0] = 1.0f;
        expected[12][4][0] = 1.0f;
        expected[13][4][0] = 1.0f;
        EXPECT_EQ(expected, data);
    }
    // 赤牌(ポン、カン、暗槓)
    {
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_FULOU> data{};
        fulou_features(Shoupai("z1234,m550-,p555=0,s5550"), (float(*)[9][4])(data.data()));
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_FULOU> expected{};
        expected[3][4][0] = 1.0f; expected[3][4][1] = 1.0f; expected[3][4][2] = 1.0f;
        expected[4][4][0] = 1.0f; expected[4][4][1] = 1.0f; expected[4][4][2] = 1.0f; expected[4][4][3] = 1.0f;
        expected[9][4][0] = 1.0f; expected[9][4][1] = 1.0f; expected[9][4][2] = 1.0f; expected[9][4][3] = 1.0f;
        expected[11][4][0] = 1.0f;
        expected[12][4][0] = 1.0f;
        expected[13][4][0] = 1.0f;
        EXPECT_EQ(expected, data);
    }
}

TEST(FeatureTest, game_features) {
    Game game;
    game.kaiju();
    std::vector<std::string> qipai{
        "m1", "m2", "m3", "m4", "m0", "m6", "m7", "m8", "m9", "p1", "p2", "p3", "p4", "p5",
        "m1", "m2", "m3", "m4", "m5", "m6", "m7", "m8", "m9", "m9",
        "p1", "p2", "p2", "p3", "p3", "p4", "p4", "p5", "p5", "p6", "p6", "p6", "p7", "p7", "p7", "p8", "p8", "p8", "p9", "p9", "p9", "p9",
        "s1", "s1", "s1", "s2", "s2", "s2", "s3", "s3", "s3", "s4", "s4", "s4", "s5", "s5", "s5", "s6", "s6", "s6", "s8", "s8", "s8", "s8", "s9", "s9", "s9", "s9",
        "z3", "z4", "z5", "z5", "z5", "z6", "z6", "z6", "z7", "z7", "z7", "p1",
        "z3", "z4", "z5", "z6", "z7", "m1", "m2", "m3", "m4", "m5", "m6", "m7", "m8",
        "z1", "z2", "z3", "z4", "s1", "s2", "s3", "s4", "s0", "s6", "s7", "s7", "s7",
        "z1", "z2", "z3", "z4", "p1", "p2", "p3", "p4", "p0", "p6", "p7", "p8", "s7",
        "z1", "z1", "z2", "z2", "m1", "m2", "m3", "m4", "m5", "m6", "m7", "m8", "m9",
    };
    assert(qipai.size() == 136);
    Shan shan(qipai);
    game.qipai_(shan);

    // 自摸
    {
        game.zimo();
        EXPECT_EQ(0, game.lunban());

        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_PUBLIC> data{};
        public_features(game, 0, (float(*)[9][4])(data.data()));

        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_PUBLIC> expected{};
        size_t index = 0;
        // 状態
        fill_channel(expected, index, 1.0f);
        index += N_CHANNELS_STATUS;
        // 手牌
        for (const auto& pai : std::vector<std::string>{ qipai.end() - 13, qipai.end() })
            set_pai(expected, index, pai);
        index += N_CHANNELS_SHOUPAI;
        // 副露
        index += N_CHANNELS_FULOU * 4;
        // 自摸牌
        set_pai(expected, index, "p1");
        index += N_CHANNELS_PAI;
        // 他家打牌
        index += N_CHANNELS_PAI;
        // 聴牌
        fill_channel(expected, index, 1.0f);
        index += N_CHANNELS_TINGPAI;
        // 立直
        index += N_CHANNELS_LIZHI * 4;
        // 河牌
        index += N_CHANNELS_HE * 4;
        // 他家の直前の捨て牌
        index += N_CHANNELS_TAJIADAPAI;
        // ドラ
        set_baopai(expected, index, qipai[4]);
        index += N_CHANNELS_BAOPAI;
        // 自風
        fill_channel(expected, index, 1.0f);
        index += N_CHANNELS_MENFENG;
        // 場風
        fill_channel(expected, index, 1.0f);
        index += N_CHANNELS_ZHUANGFENG;
        // 残り牌数
        fill_channel(expected, index, 1.0f);

        diff_data(expected, data);
        EXPECT_EQ(expected, data);
    }
    {
        // 非公開情報
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_PRIVATE> data{};
        private_features(game, 0, (float(*)[9][4])(data.data()));

        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_PRIVATE> expected{};
        size_t index = 0;
        // 他家の手牌
        for (size_t i = 1; i < 4; i++) {
            for (const auto& pai : std::vector<std::string>{ qipai.end() - 13 * (i + 1), qipai.end() - 13 * i})
                set_pai(expected, index, pai);
            index += N_CHANNELS_SHOUPAI;
        }
        // 他家の聴牌
        index += N_CHANNELS_TINGPAI * 3;
        // 残り牌
        for (const auto& pai : std::vector<std::string>{ qipai.begin() + 14, qipai.end() - 13 * 4 - 1 })
            set_pai(expected, index, pai);
        index += N_CHANNELS_SHAN;
        // 裏ドラ
        set_baopai(expected, index, qipai[9]);

        diff_data(expected, data);
        EXPECT_EQ(expected, data);
    }
    // 打牌
    {
        // 立直
        game.dapai("p1_*");
        EXPECT_EQ(0, game.lunban());

        // 南家の特徴量
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_PUBLIC> data{};
        public_features(game, 1, (float(*)[9][4])(data.data()));

        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_PUBLIC> expected{};
        size_t index = 0;
        // 状態
        fill_channel(expected, index + 3, 1.0f);
        index += N_CHANNELS_STATUS;
        // 手牌
        for (const auto& pai : std::vector<std::string>{ qipai.end() - 13 * 2, qipai.end() - 13 })
            set_pai(expected, index, pai);
        index += N_CHANNELS_SHOUPAI;
        // 副露
        index += N_CHANNELS_FULOU * 4;
        // 自摸牌
        index += N_CHANNELS_PAI;
        // 他家打牌
        set_pai(expected, index, "p1_*");
        index += N_CHANNELS_PAI;
        // 聴牌
        index += N_CHANNELS_TINGPAI;
        // 立直
        fill_channel(expected, index + 3, 1.0f);
        index += N_CHANNELS_LIZHI * 4;
        // 河牌
        set_pai(expected, index + N_CHANNELS_HE * 3, "p1_*");
        index += N_CHANNELS_HE * 4;
        // 他家の直前の捨て牌
        index += N_CHANNELS_TAJIADAPAI;
        // ドラ
        set_baopai(expected, index, qipai[4]);
        index += N_CHANNELS_BAOPAI;
        // 自風
        fill_channel(expected, index + 1, 1.0f);
        index += N_CHANNELS_MENFENG;
        // 場風
        fill_channel(expected, index, 1.0f);
        index += N_CHANNELS_ZHUANGFENG;
        // 残り牌数
        fill_channel(expected, index, 1.0f);

        diff_data(expected, data);
        EXPECT_EQ(expected, data);
    }
    {
        // 非公開情報
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_PRIVATE> data{};
        private_features(game, 1, (float(*)[9][4])(data.data()));

        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_PRIVATE> expected{};
        size_t index = 0;
        // 他家の手牌
        for (size_t i = 1; i < 4; i++) {
            size_t l = (i + 1) % 4;
            for (const auto& pai : std::vector<std::string>{ qipai.end() - 13 * (l + 1), qipai.end() - 13 * l })
                set_pai(expected, index, pai);
            index += N_CHANNELS_SHOUPAI;
        }
        // 他家の聴牌
        fill_channel(expected, index + 2, 1.0f);
        index += N_CHANNELS_TINGPAI * 3;
        // 残り牌
        for (const auto& pai : std::vector<std::string>{ qipai.begin() + 14, qipai.end() - 13 * 4 - 1 })
            set_pai(expected, index, pai);
        index += N_CHANNELS_SHAN;
        // 裏ドラ
        set_baopai(expected, index, qipai[9]);

        diff_data(expected, data);
        EXPECT_EQ(expected, data);
    }
    // チー
    {
        game.fulou("p1-23");
        EXPECT_EQ(1, game.lunban());

        // 南家の特徴量
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_PUBLIC> data{};
        public_features(game, 1, (float(*)[9][4])(data.data()));

        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_PUBLIC> expected{};
        size_t index = 0;
        // 状態
        fill_channel(expected, index + 4, 1.0f);
        index += N_CHANNELS_STATUS;
        // 手牌
        for (const auto& pai : std::vector<std::string>{ "z1", "z2", "z3", "z4", "p1", "p4", "p0", "p6", "p7", "p8", "s7" })
            set_pai(expected, index, pai);
        index += N_CHANNELS_SHOUPAI;
        // 副露
        set_pai(expected, index, "p1");
        set_pai(expected, index, "p2");
        set_pai(expected, index, "p3");
        index += N_CHANNELS_FULOU * 4;
        // 自摸牌
        index += N_CHANNELS_PAI;
        // 他家打牌
        index += N_CHANNELS_PAI;
        // 聴牌
        index += N_CHANNELS_TINGPAI;
        // 立直
        fill_channel(expected, index + 3, 1.0f);
        index += N_CHANNELS_LIZHI * 4;
        // 河牌
        set_pai(expected, index + N_CHANNELS_HE * 3, "p1_*");
        index += N_CHANNELS_HE * 4;
        // 他家の直前の捨て牌
        index += N_CHANNELS_TAJIADAPAI;
        // ドラ
        set_baopai(expected, index, qipai[4]);
        index += N_CHANNELS_BAOPAI;
        // 自風
        fill_channel(expected, index + 1, 1.0f);
        index += N_CHANNELS_MENFENG;
        // 場風
        fill_channel(expected, index, 1.0f);
        index += N_CHANNELS_ZHUANGFENG;
        // 残り牌数
        fill_channel(expected, index, 1.0f);

        diff_data(expected, data);
        EXPECT_EQ(expected, data);
    }
    {
        // 非公開情報
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_PRIVATE> data{};
        private_features(game, 1, (float(*)[9][4])(data.data()));

        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_PRIVATE> expected{};
        size_t index = 0;
        // 他家の手牌
        for (size_t i = 1; i < 4; i++) {
            size_t l = (i + 1) % 4;
            for (const auto& pai : std::vector<std::string>{ qipai.end() - 13 * (l + 1), qipai.end() - 13 * l })
                set_pai(expected, index, pai);
            index += N_CHANNELS_SHOUPAI;
        }
        // 他家の聴牌
        fill_channel(expected, index + 2, 1.0f);
        index += N_CHANNELS_TINGPAI * 3;
        // 残り牌
        for (const auto& pai : std::vector<std::string>{ qipai.begin() + 14, qipai.end() - 13 * 4 - 1 })
            set_pai(expected, index, pai);
        index += N_CHANNELS_SHAN;
        // 裏ドラ
        set_baopai(expected, index, qipai[9]);

        diff_data(expected, data);
        EXPECT_EQ(expected, data);
    }
    // 打牌→カン→カン自摸
    {
        game.dapai("s7");
        game.fulou("s7777-");
        game.gangzimo();
        game.kaigang();
        EXPECT_EQ(2, game.lunban());

        // 西家の特徴量
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_PUBLIC> data{};
        public_features(game, 2, (float(*)[9][4])(data.data()));

        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_PUBLIC> expected{};
        size_t index = 0;
        // 状態
        fill_channel(expected, index, 1.0f);
        index += N_CHANNELS_STATUS;
        // 手牌
        for (const auto& pai : std::vector<std::string>{ "z1", "z2", "z3", "z4", "s1", "s2", "s3", "s4", "s0", "s6" })
            set_pai(expected, index, pai);
        index += N_CHANNELS_SHOUPAI;
        // 副露
        set_pai(expected, index + 3, "s7");
        set_pai(expected, index + 3, "s7");
        set_pai(expected, index + 3, "s7");
        set_pai(expected, index + 3, "s7");
        set_pai(expected, index + N_CHANNELS_FULOU * 3, "p1");
        set_pai(expected, index + N_CHANNELS_FULOU * 3, "p2");
        set_pai(expected, index + N_CHANNELS_FULOU * 3, "p3");
        index += N_CHANNELS_FULOU * 4;
        // 自摸牌
        set_pai(expected, index, "m1");
        index += N_CHANNELS_PAI;
        // 他家打牌
        index += N_CHANNELS_PAI;
        // 聴牌
        index += N_CHANNELS_TINGPAI;
        // 立直
        fill_channel(expected, index + 2, 1.0f);
        index += N_CHANNELS_LIZHI * 4;
        // 河牌
        set_pai(expected, index + N_CHANNELS_HE * 2, "p1_*");
        set_pai(expected, index + N_CHANNELS_HE * 3, "s7_");
        index += N_CHANNELS_HE * 4;
        // 他家の直前の捨て牌
        index += N_CHANNELS_TAJIADAPAI;
        // ドラ
        set_baopai(expected, index, qipai[4]);
        set_baopai(expected, index, qipai[5]);
        index += N_CHANNELS_BAOPAI;
        // 自風
        fill_channel(expected, index + 2, 1.0f);
        index += N_CHANNELS_MENFENG;
        // 場風
        fill_channel(expected, index, 1.0f);
        index += N_CHANNELS_ZHUANGFENG;
        // 残り牌数
        fill_channel(expected, index, (float)(MAX_PAISHU - 1) / MAX_PAISHU);

        diff_data(expected, data);
        EXPECT_EQ(expected, data);
    }
    {
        // 非公開情報
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_PRIVATE> data{};
        private_features(game, 2, (float(*)[9][4])(data.data()));

        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_PRIVATE> expected{};
        size_t index = 0;
        // 他家の手牌
        for (const auto& pai : std::vector<std::string>{ "z3", "z4", "z5", "z6", "z7", "m1", "m2", "m3", "m4", "m5", "m6", "m7", "m8" })
            set_pai(expected, index, pai);
        index += N_CHANNELS_SHOUPAI;
        for (const auto& pai : std::vector<std::string>{ "z1", "z1", "z2", "z2", "m1", "m2", "m3", "m4", "m5", "m6", "m7", "m8", "m9" })
            set_pai(expected, index, pai);
        index += N_CHANNELS_SHOUPAI;
        for (const auto& pai : std::vector<std::string>{ "z1", "z2", "z3", "z4", "p1", "p4", "p0", "p6", "p7", "p8" })
            set_pai(expected, index, pai);
        index += N_CHANNELS_SHOUPAI;
        // 他家の聴牌
        fill_channel(expected, index + 1, 1.0f);
        index += N_CHANNELS_TINGPAI * 3;
        // 残り牌
        for (const auto& pai : std::vector<std::string>{ qipai.begin() + 15, qipai.end() - 13 * 4 - 1 })
            set_pai(expected, index, pai);
        index += N_CHANNELS_SHAN;
        // 裏ドラ
        set_baopai(expected, index, qipai[9]);
        set_baopai(expected, index, qipai[10]);

        diff_data(expected, data);
        EXPECT_EQ(expected, data);
    }
    // 打牌
    {
        game.dapai("m1_");
        EXPECT_EQ(2, game.lunban());

        // 北家の特徴量
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_PUBLIC> data{};
        public_features(game, 3, (float(*)[9][4])(data.data()));

        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_PUBLIC> expected{};
        size_t index = 0;
        // 状態
        fill_channel(expected, index + 3, 1.0f);
        index += N_CHANNELS_STATUS;
        // 手牌
        for (const auto& pai : std::vector<std::string>{ "z3", "z4", "z5", "z6", "z7", "m1", "m2", "m3", "m4", "m5", "m6", "m7", "m8" })
            set_pai(expected, index, pai);
        index += N_CHANNELS_SHOUPAI;
        // 副露
        set_pai(expected, index + N_CHANNELS_FULOU * 3 + 3, "s7");
        set_pai(expected, index + N_CHANNELS_FULOU * 3 + 3, "s7");
        set_pai(expected, index + N_CHANNELS_FULOU * 3 + 3, "s7");
        set_pai(expected, index + N_CHANNELS_FULOU * 3 + 3, "s7");
        set_pai(expected, index + N_CHANNELS_FULOU * 2, "p1");
        set_pai(expected, index + N_CHANNELS_FULOU * 2, "p2");
        set_pai(expected, index + N_CHANNELS_FULOU * 2, "p3");
        index += N_CHANNELS_FULOU * 4;
        // 自摸牌
        index += N_CHANNELS_PAI;
        // 他家打牌
        set_pai(expected, index, "m1");
        index += N_CHANNELS_PAI;
        // 聴牌
        index += N_CHANNELS_TINGPAI;
        // 立直
        fill_channel(expected, index + 1, 1.0f);
        index += N_CHANNELS_LIZHI * 4;
        // 河牌
        set_pai(expected, index + N_CHANNELS_HE * 1, "p1_*");
        set_pai(expected, index + N_CHANNELS_HE * 2, "s7_");
        set_pai(expected, index + N_CHANNELS_HE * 3, "m1_");
        index += N_CHANNELS_HE * 4;
        // 他家の直前の捨て牌
        set_pai(expected, index, "p1_*");
        set_pai(expected, index, "s7_");
        index += N_CHANNELS_TAJIADAPAI;
        // ドラ
        set_baopai(expected, index, qipai[4]);
        set_baopai(expected, index, qipai[5]);
        index += N_CHANNELS_BAOPAI;
        // 自風
        fill_channel(expected, index + 3, 1.0f);
        index += N_CHANNELS_MENFENG;
        // 場風
        fill_channel(expected, index, 1.0f);
        index += N_CHANNELS_ZHUANGFENG;
        // 残り牌数
        fill_channel(expected, index, (float)(MAX_PAISHU - 1) / MAX_PAISHU);

        diff_data(expected, data);
        EXPECT_EQ(expected, data);
    }
    {
        // 非公開情報
        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_PRIVATE> data{};
        private_features(game, 3, (float(*)[9][4])(data.data()));

        std::array<std::array<std::array<float, 4>, 9>, N_CHANNELS_PRIVATE> expected{};
        size_t index = 0;
        // 他家の手牌
        for (const auto& pai : std::vector<std::string>{ "z1", "z1", "z2", "z2", "m1", "m2", "m3", "m4", "m5", "m6", "m7", "m8", "m9" })
            set_pai(expected, index, pai);
        index += N_CHANNELS_SHOUPAI;
        for (const auto& pai : std::vector<std::string>{ "z1", "z2", "z3", "z4", "p1", "p4", "p0", "p6", "p7", "p8" })
            set_pai(expected, index, pai);
        index += N_CHANNELS_SHOUPAI;
        for (const auto& pai : std::vector<std::string>{ "z1", "z2", "z3", "z4", "s1", "s2", "s3", "s4", "s0", "s6" })
            set_pai(expected, index, pai);
        index += N_CHANNELS_SHOUPAI;
        // 他家の聴牌
        fill_channel(expected, index, 1.0f);
        index += N_CHANNELS_TINGPAI * 3;
        // 残り牌
        for (const auto& pai : std::vector<std::string>{ qipai.begin() + 15, qipai.end() - 13 * 4 - 1 })
            set_pai(expected, index, pai);
        index += N_CHANNELS_SHAN;
        // 裏ドラ
        set_baopai(expected, index, qipai[9]);
        set_baopai(expected, index, qipai[10]);

        diff_data(expected, data);
        EXPECT_EQ(expected, data);
    }
}
