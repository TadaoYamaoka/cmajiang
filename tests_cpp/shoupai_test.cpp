#include "pch.h"
#include "../src_cpp/shoupai.h"

TEST(ShoupaiTest, valid_pai) {
    // 正常
    EXPECT_TRUE(Shoupai::valid_pai("m1"));
    // 正常(ツモ切り)
    EXPECT_TRUE(Shoupai::valid_pai("p2_"));
    // 正常(リーチ)
    EXPECT_TRUE(Shoupai::valid_pai("s3*"));
    // 正常(ツモ切り・リーチ)
    EXPECT_TRUE(Shoupai::valid_pai("z4_*"));
    // 正常(被副露)
    EXPECT_TRUE(Shoupai::valid_pai("m0-"));
    // 正常(ツモ切り・被副露)
    EXPECT_TRUE(Shoupai::valid_pai("p5_+"));
    // 正常(リーチ・被副露)
    EXPECT_TRUE(Shoupai::valid_pai("s6*="));
    // 正常(ツモ切り・リーチ・被副露)
    EXPECT_TRUE(Shoupai::valid_pai("z7_*-"));
    // 不正(裏向き牌)
    EXPECT_FALSE(Shoupai::valid_pai("_"));
    // 不正
    EXPECT_FALSE(Shoupai::valid_pai("x"));
    // 不正
    EXPECT_FALSE(Shoupai::valid_pai("mm"));
    // 不正
    EXPECT_FALSE(Shoupai::valid_pai("z0"));
    // 不正
    EXPECT_FALSE(Shoupai::valid_pai("z8"));
    // 不正
    EXPECT_FALSE(Shoupai::valid_pai("m9x"));
    // 不正
    EXPECT_FALSE(Shoupai::valid_pai("m9=*"));
    // 不正
    EXPECT_FALSE(Shoupai::valid_pai("m9*_"));
    // 不正
    EXPECT_FALSE(Shoupai::valid_pai("m9=_"));
}

TEST(ShoupaiTest, valid_mianzi) {
    // 正常
    EXPECT_EQ("m111+", Shoupai::valid_mianzi("m111+"));
    // 正常
    EXPECT_EQ("p555=", Shoupai::valid_mianzi("p555="));
    // 正常
    EXPECT_EQ("s999-", Shoupai::valid_mianzi("s999-"));
    // 正常
    EXPECT_EQ("z777+7", Shoupai::valid_mianzi("z777+7"));
    // 正常
    EXPECT_EQ("m2222", Shoupai::valid_mianzi("m2222"));
    // 正常
    EXPECT_EQ("p550=", Shoupai::valid_mianzi("p550="));
    // 正常
    EXPECT_EQ("p5550=", Shoupai::valid_mianzi("p5550="));
    // 正常 => p505=
    EXPECT_EQ("p505=", Shoupai::valid_mianzi("p055="));
    // 正常 => p505=0
    EXPECT_EQ("p505=0", Shoupai::valid_mianzi("p055=0"));
    // 正常
    EXPECT_EQ("p000=0", Shoupai::valid_mianzi("p000=0"));
    // 正常 => s5505-
    EXPECT_EQ("s5505-", Shoupai::valid_mianzi("s0555-"));
    // 正常 => s5005-
    EXPECT_EQ("s5005-", Shoupai::valid_mianzi("s0055-"));
    // 正常 => s5000
    EXPECT_EQ("s5000", Shoupai::valid_mianzi("s0005"));
    // 正常 => s0000
    EXPECT_EQ("s0000", Shoupai::valid_mianzi("s0000"));
    // 正常
    EXPECT_EQ("m1-23", Shoupai::valid_mianzi("m1-23"));
    // 正常
    EXPECT_EQ("m12-3", Shoupai::valid_mianzi("m12-3"));
    // 正常
    EXPECT_EQ("m123-", Shoupai::valid_mianzi("m123-"));
    // 正常 => m1-23
    EXPECT_EQ("m1-23", Shoupai::valid_mianzi("m231-"));
    // 正常 => m12-3
    EXPECT_EQ("m12-3", Shoupai::valid_mianzi("m312-"));
    // 正常 => m123-
    EXPECT_EQ("m123-", Shoupai::valid_mianzi("m3-12"));
    // 正常 => m40-6
    EXPECT_EQ("m40-6", Shoupai::valid_mianzi("m460-"));
    // 不正
    EXPECT_EQ("", Shoupai::valid_mianzi("m1234-"));
    // 不正
    EXPECT_EQ("", Shoupai::valid_mianzi("m135-"));
    // 不正
    EXPECT_EQ("", Shoupai::valid_mianzi("m1234"));
    // 不正
    EXPECT_EQ("", Shoupai::valid_mianzi("m123"));
    // 不正
    EXPECT_EQ("", Shoupai::valid_mianzi("m111"));
    // 不正
    EXPECT_EQ("", Shoupai::valid_mianzi("z111=0"));
}

TEST(ShoupaiTest, constructor_qipai) {
    // インスタンスが生成できること
    EXPECT_NO_THROW(Shoupai());
    // 配牌を指定してインスタンスが生成できること
    EXPECT_NO_THROW(Shoupai(std::vector<std::string>{ "m0", "m1", "m9", "p0", "p1", "p9", "s0", "s1", "s9", "z1", "z2", "z6", "z7" }));
    // 裏向きの牌を指定してインスタンスが生成できること
    EXPECT_NO_THROW(Shoupai(std::vector<std::string>{ "_" }));
    // 不正な牌を含む配牌で例外が発生すること
    EXPECT_THROW(Shoupai(std::vector<std::string>{ "z0" }), std::invalid_argument);
    // 5枚目の牌を含む配牌で例外が発生すること
    EXPECT_THROW(Shoupai(std::vector<std::string>{ "m1", "m1", "m1", "m1", "m1" }), std::invalid_argument);
}

TEST(ShoupaiTest, constructor_paistr) {
    // 空文字列からでもインスタンスが生成できること
    EXPECT_EQ("", Shoupai("").toString());
    // 副露なしの場合にインスタンスが生成できること
    EXPECT_EQ("m123p456s789z4567", Shoupai("z7654s987p654m321").toString());
    // 副露ありの場合でもインスタンスが生成できること
    EXPECT_EQ("m1,p123-,s555=,z777+7,m9999", Shoupai("m1,p123-,s555=,z777+7,m9999").toString());
    // 伏せ牌がある場合でもインスタンスが生成できること
    EXPECT_EQ("____m123p456s789", Shoupai("m123p456s789____").toString());
    // 伏せ牌がある場合でもインスタンスが生成できること(副露あり)
    EXPECT_EQ("____m123p456,s789-", Shoupai("m123p456____,s789-").toString());
    // 少牌の場合でもインスタンスが生成できること
    EXPECT_EQ("m111p222s333", Shoupai("m111p222s333").toString());
    // 多牌の場合、14枚としてからインスタンスを生成すること
    EXPECT_EQ("m123456789p1234p5", Shoupai("m123456789p123456").toString());
    // 多牌の場合、14枚としてからインスタンスを生成すること(副露あり)
    EXPECT_EQ("m123456789p1p2,z111=", Shoupai("m123456789p123,z111=").toString());
    EXPECT_EQ("m1m2,z111=,p123-,s555=,z777+", Shoupai("m123,z111=,p123-,s555=,z777+").toString());
    // ツモ牌を再現してインスタンスを生成すること
    EXPECT_EQ("m1112345678999m1", Shoupai("m11123456789991").toString());
    // ツモ牌を再現してインスタンスを生成すること(赤牌をツモ)
    EXPECT_EQ("m1112345678999m0", Shoupai("m11123456789990").toString());
    // ツモ牌を再現してインスタンスを生成すること(副露あり)
    EXPECT_EQ("m12p345s678z23m3,z111=", Shoupai("m12p345s678z23m3,z111=").toString());
    // 手牌中の赤牌を再現してインスタンスを生成すること
    EXPECT_EQ("m0555p0055s0000", Shoupai("m5550p5500s0000z00").toString());
    // リーチを再現してインスタンスを生成すること
    EXPECT_EQ("m123p456s789z1112*", Shoupai("m123p456s789z1112*").toString());
    // リーチを再現してインスタンスを生成すること(暗槓あり)
    EXPECT_EQ("m123p456s789z2*,z1111", Shoupai("m123p456s789z2*,z1111").toString());
    // リーチを再現してインスタンスを生成すること(副露あり)
    EXPECT_EQ("m123p456s789z2*,z111+", Shoupai("m123p456s789z2*,z111+").toString());
    // 副露面子を正規化してインスタンスを生成すること(チー)
    EXPECT_EQ("m123p456s789z2,m3-40", Shoupai("m123p456s789z2,m403-").toString());
    EXPECT_EQ("m123p456s789z2,m34-0", Shoupai("m123p456s789z2,m304-").toString());
    EXPECT_EQ("m123p456s789z2,m345-", Shoupai("m123p456s789z2,m345-").toString());
    // 副露面子を正規化してインスタンスを生成すること(ポン)
    EXPECT_EQ("m123p456s789z2,p500+", Shoupai("m123p456s789z2,p050+").toString());
    EXPECT_EQ("m123p456s789z2,p505+", Shoupai("m123p456s789z2,p055+").toString());
    EXPECT_EQ("m123p456s789z2,p550+", Shoupai("m123p456s789z2,p550+").toString());
    // 副露面子を正規化してインスタンスを生成すること(カン)
    EXPECT_EQ("m123p456s789z2,s5505=", Shoupai("m123p456s789z2,s0555=").toString());
    EXPECT_EQ("m123p456s789z2,s5000=", Shoupai("m123p456s789z2,s0050=").toString());
    EXPECT_EQ("m123p456s789z2,s5500", Shoupai("m123p456s789z2,s0505").toString());
    // 不正な副露面子を無視してインスタンスを生成すること(不正な牌)
    EXPECT_EQ("m123p456s789z2", Shoupai("m123p456s789z2,z000+").toString());
    EXPECT_EQ("m123p456s789z2", Shoupai("m123p456s789z2,z888+").toString());
    // 不正な副露面子を無視してインスタンスを生成すること(字牌でのチー)
    EXPECT_EQ("m123p456s789z2", Shoupai("m123p456s789z2,z1-23").toString());
    // 不正な副露面子を無視してインスタンスを生成すること(下家からのチー)
    EXPECT_EQ("m123p456s789z2", Shoupai("m123p456s789z2,s1+23").toString());
    // 不正な副露面子を無視してインスタンスを生成すること(対子)
    EXPECT_EQ("m123p456s789z2", Shoupai("m123p456s789z2,z11-").toString());
    // 不正な副露面子を無視してインスタンスを生成すること(両嵌)
    EXPECT_EQ("m123p456s789z2", Shoupai("m123p456s789z2,s13-5").toString());
    // 不正な副露面子を無視してインスタンスを生成すること(連子)
    EXPECT_EQ("m123p456s789z2", Shoupai("m123p456s789z2,m1p2s3-").toString());
    // 副露直後の手牌を再現してインスタンスを生成すること
    EXPECT_EQ("p456s789z1,m12-3,p999=,", Shoupai("p456s789z1,m12-3,p999=,").toString());
}

TEST(ShoupaiTest, clone) {
    // インスタンスを複製すること
    {
        auto shoupai = Shoupai("");
        EXPECT_EQ(shoupai.toString(), shoupai.clone().toString());
    }
    // 手牌を再現してインスタンスを複製すること
    {
        auto shoupai = Shoupai("m123p456s789z4567");
        EXPECT_EQ(shoupai.toString(), shoupai.clone().toString());
    }
    // 副露を再現してインスタンスを複製すること
    {
        auto shoupai = Shoupai("m1,p123-,s555=,z777+7,m9999");
        EXPECT_EQ(shoupai.toString(), shoupai.clone().toString());
    }
    // ツモ牌を再現してインスタンスを複製すること
    {
        auto shoupai = Shoupai("m11123456789991");
        EXPECT_EQ(shoupai.toString(), shoupai.clone().toString());
    }
    // リーチを再現してインスタンスを複製すること
    {
        auto shoupai = Shoupai("m123p456s789z1112*");
        EXPECT_EQ(shoupai.toString(), shoupai.clone().toString());
    }
    // 伏せ牌を再現してインスタンスを複製すること
    {
        auto shoupai = Shoupai("___________,m123-");
        EXPECT_EQ(shoupai.toString(), shoupai.clone().toString());
    }
    // 複製後のツモが複製前の手牌に影響しないこと
    {
        auto shoupai = Shoupai("m123p456s789z4567");
        EXPECT_NE(shoupai.toString(), shoupai.clone().zimo("m1").toString());
    }
    // 複製後の打牌が複製前の手牌に影響しないこと
    {
        auto shoupai = Shoupai("m123p456s789z34567");
        EXPECT_NE(shoupai.toString(), shoupai.clone().dapai("m1").toString());
    }
    // 複製後の副露が複製前の手牌に影響しないこと
    {
        auto shoupai = Shoupai("m123p456s789z1167");
        EXPECT_NE(shoupai.toString(), shoupai.clone().fulou("z111=").toString());
    }
    // 複製後のカンが複製前の手牌に影響しないこと
    {
        auto shoupai = Shoupai("m123p456s789z11112");
        EXPECT_NE(shoupai.toString(), shoupai.clone().gang("z1111").toString());
    }
    // 複製後のリーチが複製前の手牌に影響しないこと
    {
        auto shoupai = Shoupai("m123p456s789z11223");
        EXPECT_NE(shoupai.toString(), shoupai.clone().dapai("z3*").toString());
    }
}

TEST(ShoupaiTest, zimo) {
    // 萬子をツモれること
    EXPECT_EQ("m123p456s789z4567m1", Shoupai("m123p456s789z4567").zimo("m1").toString());
    // 筒子をツモれること
    EXPECT_EQ("m123p456s789z4567p1", Shoupai("m123p456s789z4567").zimo("p1").toString());
    // 索子をツモれること
    EXPECT_EQ("m123p456s789z4567s1", Shoupai("m123p456s789z4567").zimo("s1").toString());
    // 字牌をツモれること
    EXPECT_EQ("m123p456s789z4567z1", Shoupai("m123p456s789z4567").zimo("z1").toString());
    // 赤牌をツモれること
    EXPECT_EQ("m123p456s789z4567m0", Shoupai("m123p456s789z4567").zimo("m0").toString());
    // 伏せ牌をツモれること
    EXPECT_EQ("m123p456s789z4567_", Shoupai("m123p456s789z4567").zimo("_").toString());
    // 不正な牌はツモれないこと
    EXPECT_THROW(Shoupai("m123p456s789z4567").zimo(""), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p456s789z4567").zimo("z0"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p456s789z4567").zimo("z8"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p456s789z4567").zimo("mm"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p456s789z4567").zimo("xx"), std::invalid_argument);
    // ツモの直後にはツモれないこと
    EXPECT_THROW(Shoupai("m123p456s789z34567").zimo("m1"), std::runtime_error);
    // 副露の直後にはツモれないこと
    EXPECT_THROW(Shoupai("m123p456z34567,s789-,").zimo("m1"), std::runtime_error);
    // 多牌となる牌をツモることもできること
    EXPECT_EQ("m123p456s789z34567m1", Shoupai("m123p456s789z34567").zimo("m1", false).toString());
    // 5枚目の牌はツモれないこと
    EXPECT_THROW(Shoupai("m123p456s789z1111").zimo("z1"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m455556s789z1111").zimo("m0"), std::invalid_argument);
}

TEST(ShoupaiTest, dapai) {
    // 萬子を打牌できること
    EXPECT_EQ("m23p456s789z34567", Shoupai("m123p456s789z34567").dapai("m1").toString());
    // 筒子を打牌できること
    EXPECT_EQ("m123p56s789z34567", Shoupai("m123p456s789z34567").dapai("p4").toString());
    // 索子を打牌できること
    EXPECT_EQ("m123p456s89z34567", Shoupai("m123p456s789z34567").dapai("s7").toString());
    // 字牌を打牌できること
    EXPECT_EQ("m123p456s789z4567", Shoupai("m123p456s789z34567").dapai("z3").toString());
    // 赤牌を打牌できること
    EXPECT_EQ("m123p46s789z34567", Shoupai("m123p406s789z34567").dapai("p0").toString());
    // リーチできること
    EXPECT_EQ("m123p456s789z3456*", Shoupai("m123p456s789z34567").dapai("z7*").toString());
    // リーチ後にもツモ切り以外の打牌ができること(チェックしない)
    EXPECT_EQ("m123p456s789z1223*", Shoupai("m123p456s789z11223*").dapai("z1").toString());
    // 伏せ牌がある場合、任意の牌が打牌できること
    EXPECT_EQ("_____________", Shoupai("______________").dapai("m1").toString());
    // 不正な牌は打牌できないこと
    EXPECT_THROW(Shoupai("m123p456s789z34567").dapai(""), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p456s789z34567").dapai("z0"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p456s789z34567").dapai("z8"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p456s789z34567").dapai("mm"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p456s789z34567").dapai("xx"), std::invalid_argument);
    // 伏せた牌は打牌できないこと
    EXPECT_THROW(Shoupai("m123p456s789z34567").dapai("_"), std::invalid_argument);
    // 打牌の直後には打牌できないこと
    EXPECT_THROW(Shoupai("m123p456s789z4567").dapai("m1"), std::runtime_error);
    // 少牌となる牌を打牌することもできること
    EXPECT_EQ("m23p456s789z4567", Shoupai("m123p456s789z4567").dapai("m1", false).toString());
    // 手牌にない牌は打牌できないこと
    EXPECT_THROW(Shoupai("m123p456s789z34567").dapai("z1"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p456s789z34567").dapai("p0"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p406s789z34567").dapai("p5"), std::invalid_argument);
}

TEST(ShoupaiTest, gang) {
    // 萬子で暗槓できること
    EXPECT_EQ("p456s789z4567,m1111", Shoupai("m1111p456s789z4567").gang("m1111").toString());
    // 萬子で加槓できること
    EXPECT_EQ("p456s789z4567,m111+1", Shoupai("m1p456s789z4567,m111+").gang("m111+1").toString());
    // 筒子で暗槓できること
    EXPECT_EQ("m123s789z4567,p5555", Shoupai("m123p5555s789z4567").gang("p5555").toString());
    // 筒子で加槓できること
    EXPECT_EQ("m123s789z4567,p555=5", Shoupai("m123p5s789z4567,p555=").gang("p555=5").toString());
    // 索子で暗槓できること
    EXPECT_EQ("m123p456z4567,s9999", Shoupai("m123p456s9999z4567").gang("s9999").toString());
    // 索子で加槓できること
    EXPECT_EQ("m123p456z4567,s999-9", Shoupai("m123p456s9z4567,s999-").gang("s999-9").toString());
    // 字牌で暗槓できること
    EXPECT_EQ("m123p456s789z6,z7777", Shoupai("m123p456s789z67777").gang("z7777").toString());
    // 字牌で加槓できること
    EXPECT_EQ("m123p456s789z6,z777+7", Shoupai("m123p456s789z67,z777+").gang("z777+7").toString());
    // 赤牌を含む暗槓ができること
    EXPECT_EQ("p456s789z4567,m5500", Shoupai("m0055p456s789z4567").gang("m5500").toString());
    // 赤牌を含む加槓ができること
    EXPECT_EQ("m123s789z4567,p505=5", Shoupai("m123p5s789z4567,p505=").gang("p505=5").toString());
    // 赤牌で加槓できること
    EXPECT_EQ("m123s789z4567,p555=0", Shoupai("m123p0s789z4567,p555=").gang("p555=0").toString());
    // リーチ後にも暗槓できること(チェックしない)
    EXPECT_EQ("p456s789z4567*,m1111", Shoupai("m1111p456s789z4567*").gang("m1111").toString());
    // リーチ後にも加槓できること(チェックしない)
    EXPECT_EQ("p456s789z4567*,m111+1", Shoupai("m1p456s789z4567*,m111+").gang("m111+1").toString());
    // 伏せ牌がある場合、それを使って暗槓できること
    EXPECT_EQ("__________,m5550", Shoupai("______________").gang("m5550").toString());
    // 伏せ牌がある場合、それを使って加槓できること
    EXPECT_EQ("__________,m555=0", Shoupai("___________,m555=").gang("m555=0").toString());
    // 順子で槓できないこと
    EXPECT_THROW(Shoupai("m1112456s789z4567").gang("m456-"), std::invalid_argument);
    // 刻子で槓できないこと
    EXPECT_THROW(Shoupai("m1112456s789z4567").gang("m111+"), std::invalid_argument);
    // 不正な面子で槓できないこと
    EXPECT_THROW(Shoupai("m1112456s789z4567").gang("m1112"), std::invalid_argument);
    // 不正な面子で槓できないこと
    EXPECT_THROW(Shoupai("m2456s789z4567,m111+").gang("m111+2"), std::invalid_argument);
    // 打牌の直後に槓できないこと
    EXPECT_THROW(Shoupai("m1111p456s789z456").gang("m1111"), std::runtime_error);
    // 副露の直後に槓できないこと
    EXPECT_THROW(Shoupai("m1111s789z4567,p456-,").gang("m1111"), std::runtime_error);
    // 槓の直後に槓できないこと
    EXPECT_THROW(Shoupai("m1111p4444s789z567").gang("m1111").gang("p4444"), std::runtime_error);
    // 少牌となる槓もできること
    EXPECT_EQ("p456s789z456,m1111", Shoupai("m1111p456s789z456").gang("m1111", false).toString());
    EXPECT_EQ("s789z4567,p456-,m1111", Shoupai("m1111s789z4567,p456-,").gang("m1111", false).toString());
    EXPECT_EQ("s789z567,m1111,p4444", Shoupai("m1111p4444s789z567").gang("m1111", false).gang("p4444", false).toString());
    // 手牌に4枚ない牌で暗槓できないこと
    EXPECT_THROW(Shoupai("m1112p456s789z4567").gang("m1111"), std::invalid_argument);
    // 手牌にない牌で加槓できないこと
    EXPECT_THROW(Shoupai("m13p456s789z567,m222=").gang("m2222"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m10p456s789z567,m555=").gang("m5555"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m15p456s789z567,m555=").gang("m5550"), std::invalid_argument);
    // 明刻がない牌で加槓できないこと
    EXPECT_THROW(Shoupai("m12p456s789z567,m222=").gang("m111=1"), std::invalid_argument);
}

TEST(ShoupaiTest, menqian) {
    // 副露がない場合、メンゼンと判定すること
    EXPECT_TRUE(Shoupai("m123p0s789z4567").menqian());
    // 副露がある場合、メンゼンと判定しないこと
    EXPECT_FALSE(Shoupai("p0s789z4567,m123-").menqian());
    // 暗カンを副露と判定しないこと
    EXPECT_TRUE(Shoupai("m123p0s789,z1111").menqian());
}

TEST(ShoupaiTest, lizhi) {
    // 配牌時はリーチ状態でないこと
    EXPECT_FALSE(Shoupai("_____________").lizhi());
    // リーチ宣言によりリーチ状態になること
    EXPECT_TRUE(Shoupai("_____________").zimo("z7").dapai("z7_*").lizhi());
}

TEST(ShoupaiTest, get_dapai) {
    // ツモ直後または副露直後以外は打牌できないこと
    EXPECT_THROW(Shoupai("m123p406s789z4567").get_dapai(), std::runtime_error);
    EXPECT_THROW(Shoupai("m123p406s789z2,z111+").get_dapai(), std::runtime_error);
    EXPECT_THROW(Shoupai("_____________").get_dapai(), std::runtime_error);
    EXPECT_THROW(Shoupai("__________,z111+").get_dapai(), std::runtime_error);
    // ツモ直後は任意の手牌を打牌できること(メンゼン手)
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m2", "m3", "p4", "p0", "p6", "s7", "s8", "s9", "z1", "z2", "z3_" }),
        Shoupai("m123p406s789z11123").get_dapai());
    // ツモ直後は任意の手牌を打牌できること(副露手)
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m2", "m3", "p4", "p0", "p6", "s7", "s8", "s9", "z1", "z2_" }),
        Shoupai("m123p406s789z12,z111+").get_dapai());
    // リーチ後はツモ切りしかできないこと
    EXPECT_EQ((std::vector<std::string>{
        "m1_" }),
        Shoupai("m123p456s789z1234m1*").get_dapai());
    // 赤牌を単独の牌として区別すること
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m2", "m3", "p4", "p0", "p5", "s7", "s8", "s9", "z1", "z2", "z3_" }),
        Shoupai("m123p405s789z11123").get_dapai());
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m2", "m3", "p4", "p5", "s7", "s8", "s9", "z1", "z2", "z3", "p0_" }),
        Shoupai("m123p45s789z11123p0").get_dapai());
    // 手出しとツモ切りを区別すること
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m2", "m3", "p4", "p5", "s7", "s8", "s9", "z1", "z2", "z3", "p5_" }),
        Shoupai("m123p45s789z11123p5").get_dapai());
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m2", "m3", "p4", "p0", "p5", "s7", "s8", "s9", "z1", "z2", "p0_" }),
        Shoupai("m123p405s789z1112p0").get_dapai());
    // 伏せ牌のみの手牌では空配列を返すこと
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("______________").get_dapai());
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("___________,m123-,").get_dapai());
    // 喰替えとなる打牌が許されないこと(両面鳴き)
    EXPECT_EQ((std::vector<std::string>{
        "m5", "p4", "p0", "p6", "s7", "s8", "s9", "z2", "z3" }),
        Shoupai("m145p406s789z23,m1-23,").get_dapai());
    EXPECT_EQ((std::vector<std::string>{
        "m5", "p4", "p0", "p6", "s7", "s8", "s9", "z2", "z3" }),
        Shoupai("m145p406s789z23,m234-,").get_dapai());
    // 喰替えとなる打牌が許されないこと(嵌張鳴き)
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m2", "m3", "p2", "p8", "s7", "s8", "s9","z2", "z3" }),
        Shoupai("m123p258s789z23,p45-6,").get_dapai());
    // 喰替えとなる打牌が許されないこと(辺張鳴き)
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m2", "m3", "p4", "p5", "p6", "s4", "s6", "z2", "z3" }),
        Shoupai("m123p456s467z23,s7-89,").get_dapai());
    // 喰替えとなる打牌が許されないこと(ポン)
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m2", "m3", "p4", "p5", "p6", "s7", "s8", "s9", "z2" }),
        Shoupai("m123p456s789z12,z111+,").get_dapai());
    // 喰替えとなる打牌が許されないこと(赤牌入りの鳴き)
    EXPECT_EQ((std::vector<std::string>{
        "m6", "p4", "p5", "p6", "s7", "s8", "s9", "z1", "z2" }),
        Shoupai("m256p456s789z12,m340-,").get_dapai());
    // 喰替えとなる打牌が許されないこと(赤牌打牌)
    EXPECT_EQ((std::vector<std::string>{
        "m6", "p4", "p5", "p6", "s7", "s8", "s9", "z1", "z2" }),
        Shoupai("m206p456s789z12,m345-,").get_dapai());
    // 赤牌によるポンの際に正しく喰い替え判定すること
    EXPECT_EQ((std::vector<std::string>{
        "m2", "p1", "s1", "s2", "s6", "s7", "s8" }),
        Shoupai("m25p1s12678,z666+,m550-,").get_dapai());
    // 喰替えのため打牌できない場合があること
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m14,p456-,s789-,z111+,m234-,").get_dapai());
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m14,p456-,s789-,z111+,m1-23,").get_dapai());
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m22,p456-,s789-,z111+,m12-3,").get_dapai());
    // 喰い替えを許すこともできること
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m4", "m5", "p4", "p0", "p6", "s7", "s8", "s9", "z2", "z3" }),
        Shoupai("m145p406s789z23,m1-23,").get_dapai(false));
}

TEST(ShoupaiTest, get_chi_mianzi) {
    // ツモ直後と副露の直後にチーできないこと
    EXPECT_THROW(Shoupai("m123p456s789z12345").get_chi_mianzi("m1-"), std::runtime_error);
    EXPECT_THROW(Shoupai("m123p456s789z12,z333=,").get_chi_mianzi("m1-"), std::runtime_error);
    EXPECT_THROW(Shoupai("______________").get_chi_mianzi("m1-"), std::runtime_error);
    // チーできるメンツがない場合
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m123p456s789z1234").get_chi_mianzi("m5-"));
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("_____________").get_chi_mianzi("m5-"));
    // チーできるメンツが1つの場合
    EXPECT_EQ((std::vector<std::string>{
        "m123-" }),
        Shoupai("m123p456s789z1234").get_chi_mianzi("m3-"));
    // チーできるメンツが2つの場合
    EXPECT_EQ((std::vector<std::string>{
        "m123-", "m23-4" }),
        Shoupai("m1234p456s789z123").get_chi_mianzi("m3-"));
    // チーできるメンツが3つの場合
    EXPECT_EQ((std::vector<std::string>{
        "m123-", "m23-4", "m3-45" }),
        Shoupai("m12345p456s789z12").get_chi_mianzi("m3-"));
    // 赤牌でチーできること
    EXPECT_EQ((std::vector<std::string>{
        "p40-6" }),
        Shoupai("m123p456s789z1234").get_chi_mianzi("p0-"));
    // 赤牌含みでチーできること
    EXPECT_EQ((std::vector<std::string>{
        "p3-40" }),
        Shoupai("m123p34067s789z12").get_chi_mianzi("p3-"));
    EXPECT_EQ((std::vector<std::string>{
        "p34-0", "p4-06" }),
        Shoupai("m123p34067s789z12").get_chi_mianzi("p4-"));
    EXPECT_EQ((std::vector<std::string>{
        "p406-", "p06-7" }),
        Shoupai("m123p34067s789z12").get_chi_mianzi("p6-"));
    EXPECT_EQ((std::vector<std::string>{
        "p067-" }),
        Shoupai("m123p34067s789z12").get_chi_mianzi("p7-"));
    // 赤牌なしのメンツも選択すること
    EXPECT_EQ((std::vector<std::string>{
        "p3-40", "p3-45" }),
        Shoupai("m123p340567s789z1").get_chi_mianzi("p3-"));
    EXPECT_EQ((std::vector<std::string>{
        "p34-0", "p34-5", "p4-06", "p4-56" }),
        Shoupai("m123p340567s789z1").get_chi_mianzi("p4-"));
    EXPECT_EQ((std::vector<std::string>{
        "p406-", "p456-", "p06-7", "p56-7" }),
        Shoupai("m123p340567s789z1").get_chi_mianzi("p6-"));
    EXPECT_EQ((std::vector<std::string>{
        "p067-", "p567-" }),
        Shoupai("m123p340567s789z1").get_chi_mianzi("p7-"));
    // ツモ切りの牌をチーできること
    EXPECT_EQ((std::vector<std::string>{
        "m123-" }),
        Shoupai("m123p456s789z1234").get_chi_mianzi("m3_-"));
    // リーチ宣言牌をチーできること
    EXPECT_EQ((std::vector<std::string>{
        "m123-" }),
        Shoupai("m123p456s789z1234").get_chi_mianzi("m3*-"));
    // ツモ切りリーチの宣言牌をチーできること
    EXPECT_EQ((std::vector<std::string>{
        "m123-" }),
        Shoupai("m123p456s789z1234").get_chi_mianzi("m3_*-"));
    // リーチ後はチーできないこと
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m123p456s789z1234*").get_chi_mianzi("m3-"));
    // 打牌選択できなくなる鳴き方を含めないこと
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("s6789,m123-,p456-,z111+").get_chi_mianzi("s6-"));
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("s6789,m123-,p456-,z111+").get_chi_mianzi("s9-"));
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("s7889,m123-,p456-,z111+").get_chi_mianzi("s8-"));
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("s7899,m123-,p456-,z111+").get_chi_mianzi("s9-"));
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("s7789,m123-,p456-,z111+").get_chi_mianzi("s7-"));
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("s6678999,m123-,p456-").get_chi_mianzi("s6-"));
    // 喰い替えをとなる鳴きを許すこともできること
    EXPECT_EQ((std::vector<std::string>{
        "s6-78" }),
        Shoupai("s6789,m123-,p456-,z111+").get_chi_mianzi("s6-", false));
    EXPECT_EQ((std::vector<std::string>{
        "s789-" }),
        Shoupai("s6789,m123-,p456-,z111+").get_chi_mianzi("s9-", false));
    EXPECT_EQ((std::vector<std::string>{
        "s78-9" }),
        Shoupai("s7889,m123-,p456-,z111+").get_chi_mianzi("s8-", false));
    EXPECT_EQ((std::vector<std::string>{
        "s789-" }),
        Shoupai("s7899,m123-,p456-,z111+").get_chi_mianzi("s9-", false));
    EXPECT_EQ((std::vector<std::string>{
        "s7-89" }),
        Shoupai("s7789,m123-,p456-,z111+").get_chi_mianzi("s7-", false));
    EXPECT_EQ((std::vector<std::string>{
        "s6-78" }),
        Shoupai("s6678999,m123-,p456-").get_chi_mianzi("s6-", false));
    // 不正な牌でチーできないこと
    EXPECT_THROW(Shoupai("m123p456s789z1234").get_chi_mianzi("mm-"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p456s789z1234").get_chi_mianzi("m1"), std::invalid_argument);
    // 字牌でチーできないこと
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m123p456s789z1234").get_chi_mianzi("z1-"));
    // 上家以外からチーできないこと
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m123p456s789z1234").get_chi_mianzi("m1+"));
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m123p456s789z1234").get_chi_mianzi("m1="));
}

TEST(ShoupaiTest, get_gang_mianzi) {
    // ツモ直後と副露の直後に大明槓できないこと
    EXPECT_THROW(Shoupai("m111p456s789z12345").get_gang_mianzi("m1+"), std::runtime_error);
    EXPECT_THROW(Shoupai("m111p456s789z12,z333=,").get_gang_mianzi("m1+"), std::runtime_error);
    EXPECT_THROW(Shoupai("______________").get_gang_mianzi("m1-"), std::runtime_error);
    // 大明槓できるメンツがない場合
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m123p456s789z1122").get_gang_mianzi("z1+"));
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("_____________").get_gang_mianzi("z1="));
    // 下家から大明槓できること
    EXPECT_EQ((std::vector<std::string>{
        "m1111+" }),
        Shoupai("m111p456s789z1234").get_gang_mianzi("m1+"));
    // 対面から大明槓できること
    EXPECT_EQ((std::vector<std::string>{
        "p4444=" }),
        Shoupai("m123p444s789z1234").get_gang_mianzi("p4="));
    // 上家から大明槓できること
    EXPECT_EQ((std::vector<std::string>{
        "s7777-" }),
        Shoupai("m123p456s777z1234").get_gang_mianzi("s7-"));
    // 赤牌で大明槓できること
    EXPECT_EQ((std::vector<std::string>{
        "p5550+" }),
        Shoupai("m123p555s789z1234").get_gang_mianzi("p0+"));
    // 赤牌入りの大明槓ができること
    EXPECT_EQ((std::vector<std::string>{
        "p5505+" }),
        Shoupai("m123p055s789z1234").get_gang_mianzi("p5+"));
    EXPECT_EQ((std::vector<std::string>{
        "p5005+" }),
        Shoupai("m123p005s789z1234").get_gang_mianzi("p5+"));
    EXPECT_EQ((std::vector<std::string>{
        "p0005+" }),
        Shoupai("m123p000s789z1234").get_gang_mianzi("p5+"));
    // ツモ切りの牌を大明槓できること
    EXPECT_EQ((std::vector<std::string>{
        "m1111+" }),
        Shoupai("m111p456s789z1234").get_gang_mianzi("m1_+"));
    // リーチ宣言牌を大明槓できること
    EXPECT_EQ((std::vector<std::string>{
        "m1111+" }),
        Shoupai("m111p456s789z1234").get_gang_mianzi("m1*+"));
    // ツモ切りリーチの宣言牌を大明槓できること
    EXPECT_EQ((std::vector<std::string>{
        "m1111+" }),
        Shoupai("m111p456s789z1234").get_gang_mianzi("m1_*+"));
    // リーチ後は大明槓できないこと
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m111p456s789z1234*").get_gang_mianzi("m1+"));
    // 不正な牌で大明槓できないこと
    EXPECT_THROW(Shoupai("m111p555s999z1234").get_gang_mianzi("mm+"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m111p555s999z1234").get_gang_mianzi("m1"), std::invalid_argument);
    // 打牌と副露の直後には暗槓できないこと
    EXPECT_THROW(Shoupai("m1111p555s999z123").get_gang_mianzi(), std::runtime_error);
    EXPECT_THROW(Shoupai("m1111p555s999,z333=").get_gang_mianzi(), std::runtime_error);
    EXPECT_THROW(Shoupai("m11112p555s999,z333=,").get_gang_mianzi(), std::runtime_error);
    EXPECT_THROW(Shoupai("_____________").get_gang_mianzi(), std::runtime_error);
    // 暗槓できるメンツがない場合
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m123p456s789z12345").get_gang_mianzi());
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("______________").get_gang_mianzi());
    // 萬子で暗槓できること
    EXPECT_EQ((std::vector<std::string>{
        "m1111" }),
        Shoupai("m1111p456s789z1234").get_gang_mianzi());
    // 筒子で暗槓できること
    EXPECT_EQ((std::vector<std::string>{
        "p4444" }),
        Shoupai("m123p4444s789z1234").get_gang_mianzi());
    // 索子で暗槓できること
    EXPECT_EQ((std::vector<std::string>{
        "s7777" }),
        Shoupai("m123p456s7777z1234").get_gang_mianzi());
    // 字牌で暗槓できること
    EXPECT_EQ((std::vector<std::string>{
        "z1111" }),
        Shoupai("m123p456s789z11112").get_gang_mianzi());
    // 赤牌入りで暗槓できること
    EXPECT_EQ((std::vector<std::string>{
        "p5550" }),
        Shoupai("m123p0555s789z1234").get_gang_mianzi());
    EXPECT_EQ((std::vector<std::string>{
        "p5500" }),
        Shoupai("m123p0055s789z1234").get_gang_mianzi());
    EXPECT_EQ((std::vector<std::string>{
        "p5000" }),
        Shoupai("m123p0005s789z1234").get_gang_mianzi());
    EXPECT_EQ((std::vector<std::string>{
        "p0000" }),
        Shoupai("m123p0000s789z1234").get_gang_mianzi());
    // リーチ後も暗槓できること
    EXPECT_EQ((std::vector<std::string>{
        "m1111" }),
        Shoupai("m111p456s789z1122m1*").get_gang_mianzi());
    // リーチ後は送り槓できないこと
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m111123p456s78z11m4*").get_gang_mianzi());
    // 暗槓できるメンツが複数の場合
    EXPECT_EQ((std::vector<std::string>{
        "m1111", "z1111" }),
        Shoupai("m1111p456s789z1111").get_gang_mianzi());
    // 打牌と副露の直後には加槓できないこと
    EXPECT_THROW(Shoupai("m1p555s999z123,m111-").get_gang_mianzi(), std::runtime_error);
    EXPECT_THROW(Shoupai("m1p555s999,z333=,m111-").get_gang_mianzi(), std::runtime_error);
    EXPECT_THROW(Shoupai("m12p555s999,z333=,m111-,").get_gang_mianzi(), std::runtime_error);
    EXPECT_THROW(Shoupai("__________,m111-,").get_gang_mianzi(), std::runtime_error);
    // 加槓できるメンツがない場合
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m123p456s789z12,z777+").get_gang_mianzi());
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("___________,z777+").get_gang_mianzi());
    // 萬子で加槓できること
    EXPECT_EQ((std::vector<std::string>{
        "m111+1" }),
        Shoupai("m1p456s789z1234,m111+").get_gang_mianzi());
    // 筒子で加槓できること
    EXPECT_EQ((std::vector<std::string>{
        "p444=4" }),
        Shoupai("m123p4s789z1234,p444=").get_gang_mianzi());
    // 索子で加槓できること
    EXPECT_EQ((std::vector<std::string>{
        "s777-7" }),
        Shoupai("m123p456s7z1234,s777-").get_gang_mianzi());
    // 字牌で加槓できること
    EXPECT_EQ((std::vector<std::string>{
        "z111+1" }),
        Shoupai("m123p456s789z12,z111+").get_gang_mianzi());
    // 赤牌で加槓できること
    EXPECT_EQ((std::vector<std::string>{
        "p555=0" }),
        Shoupai("m123p0s789z1234,p555=").get_gang_mianzi());
    // 赤牌入りで加槓できること
    EXPECT_EQ((std::vector<std::string>{
        "p550-5" }),
        Shoupai("m123p5s789z1234,p550-").get_gang_mianzi());
    // リーチ後は加槓できないこと
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("p456s789z1234m1*,m111+").get_gang_mianzi());
    // 加槓できるメンツが複数の場合
    EXPECT_EQ((std::vector<std::string>{
        "m111+1", "p444=4" }),
        Shoupai("m1p4s789z123,m111+,p444=").get_gang_mianzi());
    // 暗槓と加槓が同時にできる場合
    EXPECT_EQ((std::vector<std::string>{
        "m111+1", "z1111" }),
        Shoupai("m1p456s789z1111,m111+").get_gang_mianzi());
}
