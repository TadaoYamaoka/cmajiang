#include "pch.h"
#include "../src_cpp/shan.h"

#include <algorithm>

TEST(ShanTest, zhenbaopai) {
    // 一萬 → 二萬
    EXPECT_EQ("m2", Shan::zhenbaopai("m1"));
    // 九萬 → 一萬
    EXPECT_EQ("m1", Shan::zhenbaopai("m9"));
    // 赤五萬 → 六萬
    EXPECT_EQ("m6", Shan::zhenbaopai("m0"));
    // 一筒 → 二筒
    EXPECT_EQ("p2", Shan::zhenbaopai("p1"));
    // 九筒 → 一筒
    EXPECT_EQ("p1", Shan::zhenbaopai("p9"));
    // 赤五筒 → 六筒
    EXPECT_EQ("p6", Shan::zhenbaopai("p0"));
    // 一索 → 二索
    EXPECT_EQ("s2", Shan::zhenbaopai("s1"));
    // 九索 → 一索
    EXPECT_EQ("s1", Shan::zhenbaopai("s9"));
    // 赤五索 → 六索
    EXPECT_EQ("s6", Shan::zhenbaopai("s0"));
    // 東 → 南
    EXPECT_EQ("z2", Shan::zhenbaopai("z1"));
    // 北 → 東
    EXPECT_EQ("z1", Shan::zhenbaopai("z4"));
    // 白 → 發
    EXPECT_EQ("z6", Shan::zhenbaopai("z5"));
    // 中 → 白
    EXPECT_EQ("z5", Shan::zhenbaopai("z7"));
    // 不正な牌 → エラー
    EXPECT_THROW(Shan::zhenbaopai("z0"), std::invalid_argument);
}

TEST(ShanTest, constructor) {
    //赤牌なしでインスタンスが生成できること
    {
        Rule rule;
        rule.hongpai = { 0, 0, 0 };
        auto pai = Shan(rule).pai();
        std::sort(pai.begin(), pai.end());

        EXPECT_EQ((std::vector<std::string> {
            "m1", "m1", "m1", "m1", "m2", "m2", "m2", "m2", "m3", "m3", "m3", "m3", "m4", "m4", "m4", "m4", "m5", "m5", "m5", "m5",
            "m6", "m6", "m6", "m6", "m7", "m7", "m7", "m7", "m8", "m8", "m8", "m8", "m9", "m9", "m9", "m9",
            "p1", "p1", "p1", "p1", "p2", "p2", "p2", "p2", "p3", "p3", "p3", "p3", "p4", "p4", "p4", "p4", "p5", "p5", "p5", "p5",
            "p6", "p6", "p6", "p6", "p7", "p7", "p7", "p7", "p8", "p8", "p8", "p8", "p9", "p9", "p9", "p9",
            "s1", "s1", "s1", "s1", "s2", "s2", "s2", "s2", "s3", "s3", "s3", "s3", "s4", "s4", "s4", "s4", "s5", "s5", "s5", "s5",
            "s6", "s6", "s6", "s6", "s7", "s7", "s7", "s7", "s8", "s8", "s8", "s8", "s9", "s9", "s9", "s9",
            "z1", "z1", "z1", "z1", "z2", "z2", "z2", "z2", "z3", "z3", "z3", "z3", "z4", "z4", "z4", "z4",
            "z5", "z5", "z5", "z5", "z6", "z6", "z6", "z6", "z7", "z7", "z7", "z7" }), pai);
    }
    //赤牌ありでインスタンスが生成できること
    {
        Rule rule;
        rule.hongpai = { 1, 2, 3 };
        auto pai = Shan(rule).pai();
        std::sort(pai.begin(), pai.end());

        EXPECT_EQ((std::vector<std::string> {
            "m0", "m1", "m1", "m1", "m1", "m2", "m2", "m2", "m2", "m3", "m3", "m3", "m3", "m4", "m4", "m4", "m4", "m5", "m5", "m5",
            "m6", "m6", "m6", "m6", "m7", "m7", "m7", "m7", "m8", "m8", "m8", "m8", "m9", "m9", "m9", "m9",
            "p0", "p0", "p1", "p1", "p1", "p1", "p2", "p2", "p2", "p2", "p3", "p3", "p3", "p3", "p4", "p4", "p4", "p4", "p5", "p5",
            "p6", "p6", "p6", "p6", "p7", "p7", "p7", "p7", "p8", "p8", "p8", "p8", "p9", "p9", "p9", "p9",
            "s0", "s0", "s0", "s1", "s1", "s1", "s1", "s2", "s2", "s2", "s2", "s3", "s3", "s3", "s3", "s4", "s4", "s4", "s4", "s5",
            "s6", "s6", "s6", "s6", "s7", "s7", "s7", "s7", "s8", "s8", "s8", "s8", "s9", "s9", "s9", "s9",
            "z1", "z1", "z1", "z1", "z2", "z2", "z2", "z2", "z3", "z3", "z3", "z3", "z4", "z4", "z4", "z4",
            "z5", "z5", "z5", "z5", "z6", "z6", "z6", "z6", "z7", "z7", "z7", "z7" }), pai);
    }
}

TEST(ShanTest, paishu) {
    // 牌山生成直後の残牌数は122
    EXPECT_EQ(122, Shan().paishu());
}

TEST(ShanTest, baopai) {
    // 牌山生成直後のドラは1枚
    EXPECT_EQ(1, Shan().baopai().size());
}

TEST(ShanTest, fubaopai) {
    // 牌山生成直後は 0 を返す
    EXPECT_EQ(0, Shan().fubaopai().size());
    // 牌山固定後は裏ドラを返す
    EXPECT_EQ(1, Shan().close().fubaopai().size());
    // 裏ドラなしの場合は牌山固定後も 0 を返す
    {
        Rule rule;
        rule.fubaopai = false;
        EXPECT_EQ(0, Shan(rule).close().fubaopai().size());
    }
}

TEST(ShanTest, zimo) {
    // 牌山生成直後にツモれること
    EXPECT_NO_THROW(Shan().zimo());
    // ツモ後に残牌数が減ること
    {
        Shan shan;
        const auto paishu = shan.paishu();
        shan.zimo();
        EXPECT_EQ(paishu - 1, shan.paishu());
    }
    // 王牌はツモれないこと
    {
        Shan shan;
        while (shan.paishu()) { shan.zimo(); }
        EXPECT_THROW(shan.zimo(), std::runtime_error);
    }
    // 牌山固定後はツモれないこと
    EXPECT_THROW(Shan().close().zimo(), std::runtime_error);
}

TEST(ShanTest, gangzimo) {
    // 牌山生成直後に槓ツモできること
    EXPECT_NO_THROW(Shan().gangzimo());
    // 槓ツモ後に残牌数が減ること
    {
        Shan shan;
        const auto paishu = shan.paishu();
        shan.gangzimo();
        EXPECT_EQ(paishu - 1, shan.paishu());
    }
    // 槓ツモ直後はツモれないこと
    {
        Shan shan;
        shan.gangzimo();
        EXPECT_THROW(shan.zimo(), std::runtime_error);
    }
    // 槓ツモ直後に続けて槓ツモできないこと
    {
        Shan shan;
        shan.gangzimo();
        EXPECT_THROW(shan.gangzimo(), std::runtime_error);
    }
    // ハイテイで槓ツモできないこと
    {
        Shan shan;
        while (shan.paishu()) { shan.zimo(); }
        EXPECT_THROW(shan.gangzimo(), std::runtime_error);
    }
    // 牌山固定後は槓ツモできないこと
    EXPECT_THROW(Shan().close().gangzimo(), std::runtime_error);
    // 5つ目の槓ツモができないこと
    {
        Shan shan;
        for (int i = 0; i < 4; i++) {
            shan.gangzimo();
            shan.kaigang();
        }
        EXPECT_THROW(shan.gangzimo(), std::runtime_error);
    }
    // カンドラなしでも5つ目の槓ツモができないこと
    {
        Rule rule;
        rule.gangbaopai = false;
        Shan shan(rule);
        for (int i = 0; i < 4; i++) {
            shan.gangzimo();
        }
        EXPECT_EQ(1, shan.baopai().size());
        EXPECT_THROW(shan.gangzimo(), std::runtime_error);
    }
}

TEST(ShanTest, kaigang) {
    // 牌山生成直後に開槓できないこと
    EXPECT_THROW(Shan().kaigang(), std::runtime_error);
    // 槓ツモ後に開槓できること
    {
        Shan shan;
        shan.gangzimo();
        EXPECT_NO_THROW(shan.kaigang());
    }
    // 開槓によりドラが増えること
    {
        Shan shan;
        shan.gangzimo();
        const auto expected = shan.baopai().size() + 1;
        EXPECT_EQ(expected, shan.kaigang().baopai().size());
    }
    // 開槓により裏ドラが増えること
    {
        Shan shan;
        shan.gangzimo();
        EXPECT_EQ(2, shan.kaigang().close().fubaopai().size());
    }
    // 開槓後はツモできること
    {
        Shan shan;
        shan.gangzimo();
        EXPECT_NO_THROW(shan.kaigang().zimo());
    }
    // 開槓後は槓ツモできること
    {
        Shan shan;
        shan.gangzimo();
        EXPECT_NO_THROW(shan.kaigang().gangzimo());
    }
    // 牌山固定後は開槓できないこと
    {
        Shan shan;
        shan.gangzimo();
        EXPECT_THROW(Shan().close().kaigang(), std::runtime_error);
    }
    // カンドラなしの場合は開槓できないこと
    {
        Rule rule;
        rule.gangbaopai = false;
        Shan shan(rule);
        EXPECT_THROW(shan.kaigang(), std::runtime_error);
    }
    // カンドラなしでも5つ目の槓ツモができないこと
    {
        Rule rule;
        rule.gangbaopai = false;
        Shan shan(rule);
        shan.gangzimo();
        EXPECT_THROW(shan.kaigang(), std::runtime_error);
    }
    // カン裏なしの場合は開槓で裏ドラが増えないこと
    {
        Rule rule;
        rule.ganglibaopai = false;
        Shan shan(rule);
        shan.gangzimo();
        EXPECT_EQ(1, shan.kaigang().close().fubaopai().size());
    }
    // 裏ドラなしの場合は開槓で裏ドラ発生しないこと
    {
        Rule rule;
        rule.fubaopai = false;
        Shan shan(rule);
        shan.gangzimo();
        EXPECT_EQ(0, shan.kaigang().close().fubaopai().size());
    }
}
