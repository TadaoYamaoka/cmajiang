#include "pch.h"
#include "../src_cpp/he.h"

TEST(HeTest, dapai) {
	// •s³‚È‘Å”v‚ª‚Å‚«‚È‚¢‚±‚Æ
	EXPECT_THROW(He().dapai("z8"), std::invalid_argument);
	// ‘Å”vŒãÌ‚Ä”v‚Ì’·‚³‚ª1‘‚¦‚é‚±‚Æ
	{
		He he;
		const auto expected = he.pai().size() + 1;
		EXPECT_EQ(expected, he.dapai("m1").pai().size());
	}
	// ƒcƒ‚Ø‚è‚ğ•\Œ»‚Å‚«‚é‚±‚Æ
	EXPECT_EQ("m1_", He().dapai("m1_").pai().back());
	// ƒŠ[ƒ`‚ğ•\Œ»‚Å‚«‚é‚±‚Æ
	EXPECT_EQ("m1*", He().dapai("m1*").pai().back());
	// ƒcƒ‚Ø‚èƒŠ[ƒ`‚ğ•\Œ»‚Å‚«‚é‚±‚Æ
	EXPECT_EQ("m1_*", He().dapai("m1_*").pai().back());
}

TEST(HeTest, fulou) {
	// •s³‚È–Êq‚Å–Â‚¯‚È‚¢‚±‚Æ
	EXPECT_THROW(He().dapai("m1").fulou("m1-"), std::invalid_argument);
	EXPECT_THROW(He().dapai("m1").fulou("m1111"), std::invalid_argument);
	EXPECT_THROW(He().dapai("m1").fulou("m12-3"), std::invalid_argument);
	// ‘Å”vŒãÌ‚Ä”v‚Ì’·‚³‚ª1‘‚¦‚é‚±‚Æ
	{
		He he = He().dapai("m1_");
		const auto expected = he.pai().size();
		EXPECT_EQ(expected, he.fulou("m111+").pai().size());
	}
	// ’N‚©‚ç–Â‚©‚ê‚½‚©•\Œ»‚Å‚«‚é‚±‚Æ
	{
		He he;
		he.dapai("m2*");
		EXPECT_EQ("m2*-", he.fulou("m12-3").pai().back());
	}
}

TEST(HeTest, find) {
	He he;
	// Ì‚Ä‚ç‚ê‚½”v‚ğ’T‚¹‚é‚±‚Æ
	EXPECT_TRUE(he.dapai("m1").find("m1"));
	// ƒcƒ‚Ø‚è‚Ì”v‚ğ’T‚¹‚é‚±‚Æ
	EXPECT_TRUE(he.dapai("m2_").find("m2"));
	// ƒŠ[ƒ`‘Å”v‚ğ’T‚¹‚é‚±‚Æ
	EXPECT_TRUE(he.dapai("m3*").find("m3"));
	// Ô”v‚ğ’T‚¹‚é‚±‚Æ
	EXPECT_TRUE(he.dapai("m0").find("m5"));
	// –Â‚©‚ê‚½”v‚ğ’T‚¹‚é‚±‚Æ
	EXPECT_TRUE(he.dapai("m4_").fulou("m234-").find("m4"));
	// “ü—Í‚ª³‹K‰»‚³‚ê‚Ä‚¢‚È‚¢ê‡‚Å‚à’T‚¹‚é‚±‚Æ
	EXPECT_TRUE(he.find("m0_*"));
}
