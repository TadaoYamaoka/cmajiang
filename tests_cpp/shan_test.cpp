#include "pch.h"
#include "../src_cpp/shan.h"

#include <algorithm>

TEST(ShanTest, zhenbaopai) {
    // ���� �� ����
    EXPECT_EQ("m2", Shan::zhenbaopai("m1"));
    // ���� �� ����
    EXPECT_EQ("m1", Shan::zhenbaopai("m9"));
    // �Ԍ��� �� �Z��
    EXPECT_EQ("m6", Shan::zhenbaopai("m0"));
    // �ꓛ �� ��
    EXPECT_EQ("p2", Shan::zhenbaopai("p1"));
    // �㓛 �� �ꓛ
    EXPECT_EQ("p1", Shan::zhenbaopai("p9"));
    // �Ԍܓ� �� �Z��
    EXPECT_EQ("p6", Shan::zhenbaopai("p0"));
    // ��� �� ���
    EXPECT_EQ("s2", Shan::zhenbaopai("s1"));
    // ��� �� ���
    EXPECT_EQ("s1", Shan::zhenbaopai("s9"));
    // �Ԍ܍� �� �Z��
    EXPECT_EQ("s6", Shan::zhenbaopai("s0"));
    // �� �� ��
    EXPECT_EQ("z2", Shan::zhenbaopai("z1"));
    // �k �� ��
    EXPECT_EQ("z1", Shan::zhenbaopai("z4"));
    // �� �� �
    EXPECT_EQ("z6", Shan::zhenbaopai("z5"));
    // �� �� ��
    EXPECT_EQ("z5", Shan::zhenbaopai("z7"));
    // �s���Ȕv �� �G���[
    EXPECT_THROW(Shan::zhenbaopai("z0"), std::invalid_argument);
}

TEST(ShanTest, constructor) {
    //�Ԕv�Ȃ��ŃC���X�^���X�������ł��邱��
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
    //�Ԕv����ŃC���X�^���X�������ł��邱��
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
    // �v�R��������̎c�v����122
    EXPECT_EQ(122, Shan().paishu());
}

TEST(ShanTest, baopai) {
    // �v�R��������̃h����1��
    EXPECT_EQ(1, Shan().baopai().size());
}

TEST(ShanTest, fubaopai) {
    // �v�R��������� 0 ��Ԃ�
    EXPECT_EQ(0, Shan().fubaopai().size());
    // �v�R�Œ��͗��h����Ԃ�
    EXPECT_EQ(1, Shan().close().fubaopai().size());
    // ���h���Ȃ��̏ꍇ�͔v�R�Œ��� 0 ��Ԃ�
    {
        Rule rule;
        rule.fubaopai = false;
        EXPECT_EQ(0, Shan(rule).close().fubaopai().size());
    }
}

TEST(ShanTest, zimo) {
    // �v�R��������Ƀc����邱��
    EXPECT_NO_THROW(Shan().zimo());
    // �c����Ɏc�v�������邱��
    {
        Shan shan;
        const auto paishu = shan.paishu();
        shan.zimo();
        EXPECT_EQ(paishu - 1, shan.paishu());
    }
    // ���v�̓c����Ȃ�����
    {
        Shan shan;
        while (shan.paishu()) { shan.zimo(); }
        EXPECT_THROW(shan.zimo(), std::runtime_error);
    }
    // �v�R�Œ��̓c����Ȃ�����
    EXPECT_THROW(Shan().close().zimo(), std::runtime_error);
}

TEST(ShanTest, gangzimo) {
    // �v�R��������ɞȃc���ł��邱��
    EXPECT_NO_THROW(Shan().gangzimo());
    // �ȃc����Ɏc�v�������邱��
    {
        Shan shan;
        const auto paishu = shan.paishu();
        shan.gangzimo();
        EXPECT_EQ(paishu - 1, shan.paishu());
    }
    // �ȃc������̓c����Ȃ�����
    {
        Shan shan;
        shan.gangzimo();
        EXPECT_THROW(shan.zimo(), std::runtime_error);
    }
    // �ȃc������ɑ����Ğȃc���ł��Ȃ�����
    {
        Shan shan;
        shan.gangzimo();
        EXPECT_THROW(shan.gangzimo(), std::runtime_error);
    }
    // �n�C�e�C�Şȃc���ł��Ȃ�����
    {
        Shan shan;
        while (shan.paishu()) { shan.zimo(); }
        EXPECT_THROW(shan.gangzimo(), std::runtime_error);
    }
    // �v�R�Œ��͞ȃc���ł��Ȃ�����
    EXPECT_THROW(Shan().close().gangzimo(), std::runtime_error);
    // 5�ڂ̞ȃc�����ł��Ȃ�����
    {
        Shan shan;
        for (int i = 0; i < 4; i++) {
            shan.gangzimo();
            shan.kaigang();
        }
        EXPECT_THROW(shan.gangzimo(), std::runtime_error);
    }
    // �J���h���Ȃ��ł�5�ڂ̞ȃc�����ł��Ȃ�����
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
    // �v�R��������ɊJ�Ȃł��Ȃ�����
    EXPECT_THROW(Shan().kaigang(), std::runtime_error);
    // �ȃc����ɊJ�Ȃł��邱��
    {
        Shan shan;
        shan.gangzimo();
        EXPECT_NO_THROW(shan.kaigang());
    }
    // �J�Ȃɂ��h���������邱��
    {
        Shan shan;
        shan.gangzimo();
        const auto expected = shan.baopai().size() + 1;
        EXPECT_EQ(expected, shan.kaigang().baopai().size());
    }
    // �J�Ȃɂ�藠�h���������邱��
    {
        Shan shan;
        shan.gangzimo();
        EXPECT_EQ(2, shan.kaigang().close().fubaopai().size());
    }
    // �J�Ȍ�̓c���ł��邱��
    {
        Shan shan;
        shan.gangzimo();
        EXPECT_NO_THROW(shan.kaigang().zimo());
    }
    // �J�Ȍ�͞ȃc���ł��邱��
    {
        Shan shan;
        shan.gangzimo();
        EXPECT_NO_THROW(shan.kaigang().gangzimo());
    }
    // �v�R�Œ��͊J�Ȃł��Ȃ�����
    {
        Shan shan;
        shan.gangzimo();
        EXPECT_THROW(Shan().close().kaigang(), std::runtime_error);
    }
    // �J���h���Ȃ��̏ꍇ�͊J�Ȃł��Ȃ�����
    {
        Rule rule;
        rule.gangbaopai = false;
        Shan shan(rule);
        EXPECT_THROW(shan.kaigang(), std::runtime_error);
    }
    // �J���h���Ȃ��ł�5�ڂ̞ȃc�����ł��Ȃ�����
    {
        Rule rule;
        rule.gangbaopai = false;
        Shan shan(rule);
        shan.gangzimo();
        EXPECT_THROW(shan.kaigang(), std::runtime_error);
    }
    // �J�����Ȃ��̏ꍇ�͊J�Ȃŗ��h���������Ȃ�����
    {
        Rule rule;
        rule.ganglibaopai = false;
        Shan shan(rule);
        shan.gangzimo();
        EXPECT_EQ(1, shan.kaigang().close().fubaopai().size());
    }
    // ���h���Ȃ��̏ꍇ�͊J�Ȃŗ��h���������Ȃ�����
    {
        Rule rule;
        rule.fubaopai = false;
        Shan shan(rule);
        shan.gangzimo();
        EXPECT_EQ(0, shan.kaigang().close().fubaopai().size());
    }
}
