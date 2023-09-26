#include "pch.h"
#include "../src_cpp/he.h"

TEST(HeTest, dapai) {
	// �s���ȑŔv���ł��Ȃ�����
	EXPECT_THROW(He().dapai("z8"), std::invalid_argument);
	// �Ŕv��̂Ĕv�̒�����1�����邱��
	{
		He he;
		const auto expected = he.pai().size() + 1;
		EXPECT_EQ(expected, he.dapai("m1").pai().size());
	}
	// �c���؂��\���ł��邱��
	EXPECT_EQ("m1_", He().dapai("m1_").pai().back());
	// ���[�`��\���ł��邱��
	EXPECT_EQ("m1*", He().dapai("m1*").pai().back());
	// �c���؂胊�[�`��\���ł��邱��
	EXPECT_EQ("m1_*", He().dapai("m1_*").pai().back());
}

TEST(HeTest, fulou) {
	// �s���Ȗʎq�Ŗ��Ȃ�����
	EXPECT_THROW(He().dapai("m1").fulou("m1-"), std::invalid_argument);
	EXPECT_THROW(He().dapai("m1").fulou("m1111"), std::invalid_argument);
	EXPECT_THROW(He().dapai("m1").fulou("m12-3"), std::invalid_argument);
	// �Ŕv��̂Ĕv�̒�����1�����邱��
	{
		He he = He().dapai("m1_");
		const auto expected = he.pai().size();
		EXPECT_EQ(expected, he.fulou("m111+").pai().size());
	}
	// �N������ꂽ���\���ł��邱��
	{
		He he;
		he.dapai("m2*");
		EXPECT_EQ("m2*-", he.fulou("m12-3").pai().back());
	}
}

TEST(HeTest, find) {
	He he;
	// �̂Ă�ꂽ�v��T���邱��
	EXPECT_TRUE(he.dapai("m1").find("m1"));
	// �c���؂�̔v��T���邱��
	EXPECT_TRUE(he.dapai("m2_").find("m2"));
	// ���[�`�Ŕv��T���邱��
	EXPECT_TRUE(he.dapai("m3*").find("m3"));
	// �Ԕv��T���邱��
	EXPECT_TRUE(he.dapai("m0").find("m5"));
	// ���ꂽ�v��T���邱��
	EXPECT_TRUE(he.dapai("m4_").fulou("m234-").find("m4"));
	// ���͂����K������Ă��Ȃ��ꍇ�ł��T���邱��
	EXPECT_TRUE(he.find("m0_*"));
}
