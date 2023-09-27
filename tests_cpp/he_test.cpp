#include "pch.h"
#include "../src_cpp/he.h"

TEST(HeTest, dapai) {
	// 不正な打牌ができないこと
	EXPECT_THROW(He().dapai("z8"), std::invalid_argument);
	// 打牌後捨て牌の長さが1増えること
	{
		He he;
		const auto expected = he.pai().size() + 1;
		EXPECT_EQ(expected, he.dapai("m1").pai().size());
	}
	// ツモ切りを表現できること
	EXPECT_EQ("m1_", He().dapai("m1_").pai().back());
	// リーチを表現できること
	EXPECT_EQ("m1*", He().dapai("m1*").pai().back());
	// ツモ切りリーチを表現できること
	EXPECT_EQ("m1_*", He().dapai("m1_*").pai().back());
}

TEST(HeTest, fulou) {
	// 不正な面子で鳴けないこと
	EXPECT_THROW(He().dapai("m1").fulou("m1-"), std::invalid_argument);
	EXPECT_THROW(He().dapai("m1").fulou("m1111"), std::invalid_argument);
	EXPECT_THROW(He().dapai("m1").fulou("m12-3"), std::invalid_argument);
	// 打牌後捨て牌の長さが1増えること
	{
		He he = He().dapai("m1_");
		const auto expected = he.pai().size();
		EXPECT_EQ(expected, he.fulou("m111+").pai().size());
	}
	// 誰から鳴かれたか表現できること
	{
		He he;
		he.dapai("m2*");
		EXPECT_EQ("m2*-", he.fulou("m12-3").pai().back());
	}
}

TEST(HeTest, find) {
	He he;
	// 捨てられた牌を探せること
	EXPECT_TRUE(he.dapai("m1").find("m1"));
	// ツモ切りの牌を探せること
	EXPECT_TRUE(he.dapai("m2_").find("m2"));
	// リーチ打牌を探せること
	EXPECT_TRUE(he.dapai("m3*").find("m3"));
	// 赤牌を探せること
	EXPECT_TRUE(he.dapai("m0").find("m5"));
	// 鳴かれた牌を探せること
	EXPECT_TRUE(he.dapai("m4_").fulou("m234-").find("m4"));
	// 入力が正規化されていない場合でも探せること
	EXPECT_TRUE(he.find("m0_*"));
}
