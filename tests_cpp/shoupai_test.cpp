#include "pch.h"
#include "../src_cpp/shoupai.h"

TEST(ShoupaiTest, valid_pai) {
    // ����
    EXPECT_TRUE(Shoupai::valid_pai("m1"));
    // ����(�c���؂�)
    EXPECT_TRUE(Shoupai::valid_pai("p2_"));
    // ����(���[�`)
    EXPECT_TRUE(Shoupai::valid_pai("s3*"));
    // ����(�c���؂�E���[�`)
    EXPECT_TRUE(Shoupai::valid_pai("z4_*"));
    // ����(�핛�I)
    EXPECT_TRUE(Shoupai::valid_pai("m0-"));
    // ����(�c���؂�E�핛�I)
    EXPECT_TRUE(Shoupai::valid_pai("p5_+"));
    // ����(���[�`�E�핛�I)
    EXPECT_TRUE(Shoupai::valid_pai("s6*="));
    // ����(�c���؂�E���[�`�E�핛�I)
    EXPECT_TRUE(Shoupai::valid_pai("z7_*-"));
    // �s��(�������v)
    EXPECT_FALSE(Shoupai::valid_pai("_"));
    // �s��
    EXPECT_FALSE(Shoupai::valid_pai("x"));
    // �s��
    EXPECT_FALSE(Shoupai::valid_pai("mm"));
    // �s��
    EXPECT_FALSE(Shoupai::valid_pai("z0"));
    // �s��
    EXPECT_FALSE(Shoupai::valid_pai("z8"));
    // �s��
    EXPECT_FALSE(Shoupai::valid_pai("m9x"));
    // �s��
    EXPECT_FALSE(Shoupai::valid_pai("m9=*"));
    // �s��
    EXPECT_FALSE(Shoupai::valid_pai("m9*_"));
    // �s��
    EXPECT_FALSE(Shoupai::valid_pai("m9=_"));
}

TEST(ShoupaiTest, valid_mianzi) {
    // ����
    EXPECT_EQ("m111+", Shoupai::valid_mianzi("m111+"));
    // ����
    EXPECT_EQ("p555=", Shoupai::valid_mianzi("p555="));
    // ����
    EXPECT_EQ("s999-", Shoupai::valid_mianzi("s999-"));
    // ����
    EXPECT_EQ("z777+7", Shoupai::valid_mianzi("z777+7"));
    // ����
    EXPECT_EQ("m2222", Shoupai::valid_mianzi("m2222"));
    // ����
    EXPECT_EQ("p550=", Shoupai::valid_mianzi("p550="));
    // ����
    EXPECT_EQ("p5550=", Shoupai::valid_mianzi("p5550="));
    // ���� => p505=
    EXPECT_EQ("p505=", Shoupai::valid_mianzi("p055="));
    // ���� => p505=0
    EXPECT_EQ("p505=0", Shoupai::valid_mianzi("p055=0"));
    // ����
    EXPECT_EQ("p000=0", Shoupai::valid_mianzi("p000=0"));
    // ���� => s5505-
    EXPECT_EQ("s5505-", Shoupai::valid_mianzi("s0555-"));
    // ���� => s5005-
    EXPECT_EQ("s5005-", Shoupai::valid_mianzi("s0055-"));
    // ���� => s5000
    EXPECT_EQ("s5000", Shoupai::valid_mianzi("s0005"));
    // ���� => s0000
    EXPECT_EQ("s0000", Shoupai::valid_mianzi("s0000"));
    // ����
    EXPECT_EQ("m1-23", Shoupai::valid_mianzi("m1-23"));
    // ����
    EXPECT_EQ("m12-3", Shoupai::valid_mianzi("m12-3"));
    // ����
    EXPECT_EQ("m123-", Shoupai::valid_mianzi("m123-"));
    // ���� => m1-23
    EXPECT_EQ("m1-23", Shoupai::valid_mianzi("m231-"));
    // ���� => m12-3
    EXPECT_EQ("m12-3", Shoupai::valid_mianzi("m312-"));
    // ���� => m123-
    EXPECT_EQ("m123-", Shoupai::valid_mianzi("m3-12"));
    // ���� => m40-6
    EXPECT_EQ("m40-6", Shoupai::valid_mianzi("m460-"));
    // �s��
    EXPECT_EQ("", Shoupai::valid_mianzi("m1234-"));
    // �s��
    EXPECT_EQ("", Shoupai::valid_mianzi("m135-"));
    // �s��
    EXPECT_EQ("", Shoupai::valid_mianzi("m1234"));
    // �s��
    EXPECT_EQ("", Shoupai::valid_mianzi("m123"));
    // �s��
    EXPECT_EQ("", Shoupai::valid_mianzi("m111"));
    // �s��
    EXPECT_EQ("", Shoupai::valid_mianzi("z111=0"));
}

TEST(ShoupaiTest, constructor_qipai) {
    // �C���X�^���X�������ł��邱��
    EXPECT_NO_THROW(Shoupai());
    // �z�v���w�肵�ăC���X�^���X�������ł��邱��
    EXPECT_NO_THROW(Shoupai(std::vector<std::string>{ "m0", "m1", "m9", "p0", "p1", "p9", "s0", "s1", "s9", "z1", "z2", "z6", "z7" }));
    // �������̔v���w�肵�ăC���X�^���X�������ł��邱��
    EXPECT_NO_THROW(Shoupai(std::vector<std::string>{ "_" }));
    // �s���Ȕv���܂ޔz�v�ŗ�O���������邱��
    EXPECT_THROW(Shoupai(std::vector<std::string>{ "z0" }), std::invalid_argument);
    // 5���ڂ̔v���܂ޔz�v�ŗ�O���������邱��
    EXPECT_THROW(Shoupai(std::vector<std::string>{ "m1", "m1", "m1", "m1", "m1" }), std::invalid_argument);
}

TEST(ShoupaiTest, constructor_paistr) {
    // �󕶎��񂩂�ł��C���X�^���X�������ł��邱��
    EXPECT_EQ("", Shoupai("").toString());
    // ���I�Ȃ��̏ꍇ�ɃC���X�^���X�������ł��邱��
    EXPECT_EQ("m123p456s789z4567", Shoupai("z7654s987p654m321").toString());
    // ���I����̏ꍇ�ł��C���X�^���X�������ł��邱��
    EXPECT_EQ("m1,p123-,s555=,z777+7,m9999", Shoupai("m1,p123-,s555=,z777+7,m9999").toString());
    // �����v������ꍇ�ł��C���X�^���X�������ł��邱��
    EXPECT_EQ("____m123p456s789", Shoupai("m123p456s789____").toString());
    // �����v������ꍇ�ł��C���X�^���X�������ł��邱��(���I����)
    EXPECT_EQ("____m123p456,s789-", Shoupai("m123p456____,s789-").toString());
    // ���v�̏ꍇ�ł��C���X�^���X�������ł��邱��
    EXPECT_EQ("m111p222s333", Shoupai("m111p222s333").toString());
    // ���v�̏ꍇ�A14���Ƃ��Ă���C���X�^���X�𐶐����邱��
    EXPECT_EQ("m123456789p1234p5", Shoupai("m123456789p123456").toString());
    // ���v�̏ꍇ�A14���Ƃ��Ă���C���X�^���X�𐶐����邱��(���I����)
    EXPECT_EQ("m123456789p1p2,z111=", Shoupai("m123456789p123,z111=").toString());
    EXPECT_EQ("m1m2,z111=,p123-,s555=,z777+", Shoupai("m123,z111=,p123-,s555=,z777+").toString());
    // �c���v���Č����ăC���X�^���X�𐶐����邱��
    EXPECT_EQ("m1112345678999m1", Shoupai("m11123456789991").toString());
    // �c���v���Č����ăC���X�^���X�𐶐����邱��(�Ԕv���c��)
    EXPECT_EQ("m1112345678999m0", Shoupai("m11123456789990").toString());
    // �c���v���Č����ăC���X�^���X�𐶐����邱��(���I����)
    EXPECT_EQ("m12p345s678z23m3,z111=", Shoupai("m12p345s678z23m3,z111=").toString());
    // ��v���̐Ԕv���Č����ăC���X�^���X�𐶐����邱��
    EXPECT_EQ("m0555p0055s0000", Shoupai("m5550p5500s0000z00").toString());
    // ���[�`���Č����ăC���X�^���X�𐶐����邱��
    EXPECT_EQ("m123p456s789z1112*", Shoupai("m123p456s789z1112*").toString());
    // ���[�`���Č����ăC���X�^���X�𐶐����邱��(�ÞȂ���)
    EXPECT_EQ("m123p456s789z2*,z1111", Shoupai("m123p456s789z2*,z1111").toString());
    // ���[�`���Č����ăC���X�^���X�𐶐����邱��(���I����)
    EXPECT_EQ("m123p456s789z2*,z111+", Shoupai("m123p456s789z2*,z111+").toString());
    // ���I�ʎq�𐳋K�����ăC���X�^���X�𐶐����邱��(�`�[)
    EXPECT_EQ("m123p456s789z2,m3-40", Shoupai("m123p456s789z2,m403-").toString());
    EXPECT_EQ("m123p456s789z2,m34-0", Shoupai("m123p456s789z2,m304-").toString());
    EXPECT_EQ("m123p456s789z2,m345-", Shoupai("m123p456s789z2,m345-").toString());
    // ���I�ʎq�𐳋K�����ăC���X�^���X�𐶐����邱��(�|��)
    EXPECT_EQ("m123p456s789z2,p500+", Shoupai("m123p456s789z2,p050+").toString());
    EXPECT_EQ("m123p456s789z2,p505+", Shoupai("m123p456s789z2,p055+").toString());
    EXPECT_EQ("m123p456s789z2,p550+", Shoupai("m123p456s789z2,p550+").toString());
    // ���I�ʎq�𐳋K�����ăC���X�^���X�𐶐����邱��(�J��)
    EXPECT_EQ("m123p456s789z2,s5505=", Shoupai("m123p456s789z2,s0555=").toString());
    EXPECT_EQ("m123p456s789z2,s5000=", Shoupai("m123p456s789z2,s0050=").toString());
    EXPECT_EQ("m123p456s789z2,s5500", Shoupai("m123p456s789z2,s0505").toString());
    // �s���ȕ��I�ʎq�𖳎����ăC���X�^���X�𐶐����邱��(�s���Ȕv)
    EXPECT_EQ("m123p456s789z2", Shoupai("m123p456s789z2,z000+").toString());
    EXPECT_EQ("m123p456s789z2", Shoupai("m123p456s789z2,z888+").toString());
    // �s���ȕ��I�ʎq�𖳎����ăC���X�^���X�𐶐����邱��(���v�ł̃`�[)
    EXPECT_EQ("m123p456s789z2", Shoupai("m123p456s789z2,z1-23").toString());
    // �s���ȕ��I�ʎq�𖳎����ăC���X�^���X�𐶐����邱��(���Ƃ���̃`�[)
    EXPECT_EQ("m123p456s789z2", Shoupai("m123p456s789z2,s1+23").toString());
    // �s���ȕ��I�ʎq�𖳎����ăC���X�^���X�𐶐����邱��(�Ύq)
    EXPECT_EQ("m123p456s789z2", Shoupai("m123p456s789z2,z11-").toString());
    // �s���ȕ��I�ʎq�𖳎����ăC���X�^���X�𐶐����邱��(����)
    EXPECT_EQ("m123p456s789z2", Shoupai("m123p456s789z2,s13-5").toString());
    // �s���ȕ��I�ʎq�𖳎����ăC���X�^���X�𐶐����邱��(�A�q)
    EXPECT_EQ("m123p456s789z2", Shoupai("m123p456s789z2,m1p2s3-").toString());
    // ���I����̎�v���Č����ăC���X�^���X�𐶐����邱��
    EXPECT_EQ("p456s789z1,m12-3,p999=,", Shoupai("p456s789z1,m12-3,p999=,").toString());
}

TEST(ShoupaiTest, clone) {
    // �C���X�^���X�𕡐����邱��
    {
        auto shoupai = Shoupai("");
        EXPECT_EQ(shoupai.toString(), shoupai.clone().toString());
    }
    // ��v���Č����ăC���X�^���X�𕡐����邱��
    {
        auto shoupai = Shoupai("m123p456s789z4567");
        EXPECT_EQ(shoupai.toString(), shoupai.clone().toString());
    }
    // ���I���Č����ăC���X�^���X�𕡐����邱��
    {
        auto shoupai = Shoupai("m1,p123-,s555=,z777+7,m9999");
        EXPECT_EQ(shoupai.toString(), shoupai.clone().toString());
    }
    // �c���v���Č����ăC���X�^���X�𕡐����邱��
    {
        auto shoupai = Shoupai("m11123456789991");
        EXPECT_EQ(shoupai.toString(), shoupai.clone().toString());
    }
    // ���[�`���Č����ăC���X�^���X�𕡐����邱��
    {
        auto shoupai = Shoupai("m123p456s789z1112*");
        EXPECT_EQ(shoupai.toString(), shoupai.clone().toString());
    }
    // �����v���Č����ăC���X�^���X�𕡐����邱��
    {
        auto shoupai = Shoupai("___________,m123-");
        EXPECT_EQ(shoupai.toString(), shoupai.clone().toString());
    }
    // ������̃c���������O�̎�v�ɉe�����Ȃ�����
    {
        auto shoupai = Shoupai("m123p456s789z4567");
        EXPECT_NE(shoupai.toString(), shoupai.clone().zimo("m1").toString());
    }
    // ������̑Ŕv�������O�̎�v�ɉe�����Ȃ�����
    {
        auto shoupai = Shoupai("m123p456s789z34567");
        EXPECT_NE(shoupai.toString(), shoupai.clone().dapai("m1").toString());
    }
    // ������̕��I�������O�̎�v�ɉe�����Ȃ�����
    {
        auto shoupai = Shoupai("m123p456s789z1167");
        EXPECT_NE(shoupai.toString(), shoupai.clone().fulou("z111=").toString());
    }
    // ������̃J���������O�̎�v�ɉe�����Ȃ�����
    {
        auto shoupai = Shoupai("m123p456s789z11112");
        EXPECT_NE(shoupai.toString(), shoupai.clone().gang("z1111").toString());
    }
    // ������̃��[�`�������O�̎�v�ɉe�����Ȃ�����
    {
        auto shoupai = Shoupai("m123p456s789z11223");
        EXPECT_NE(shoupai.toString(), shoupai.clone().dapai("z3*").toString());
    }
}

TEST(ShoupaiTest, zimo) {
    // �ݎq���c����邱��
    EXPECT_EQ("m123p456s789z4567m1", Shoupai("m123p456s789z4567").zimo("m1").toString());
    // ���q���c����邱��
    EXPECT_EQ("m123p456s789z4567p1", Shoupai("m123p456s789z4567").zimo("p1").toString());
    // ���q���c����邱��
    EXPECT_EQ("m123p456s789z4567s1", Shoupai("m123p456s789z4567").zimo("s1").toString());
    // ���v���c����邱��
    EXPECT_EQ("m123p456s789z4567z1", Shoupai("m123p456s789z4567").zimo("z1").toString());
    // �Ԕv���c����邱��
    EXPECT_EQ("m123p456s789z4567m0", Shoupai("m123p456s789z4567").zimo("m0").toString());
    // �����v���c����邱��
    EXPECT_EQ("m123p456s789z4567_", Shoupai("m123p456s789z4567").zimo("_").toString());
    // �s���Ȕv�̓c����Ȃ�����
    EXPECT_THROW(Shoupai("m123p456s789z4567").zimo(""), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p456s789z4567").zimo("z0"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p456s789z4567").zimo("z8"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p456s789z4567").zimo("mm"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p456s789z4567").zimo("xx"), std::invalid_argument);
    // �c���̒���ɂ̓c����Ȃ�����
    EXPECT_THROW(Shoupai("m123p456s789z34567").zimo("m1"), std::runtime_error);
    // ���I�̒���ɂ̓c����Ȃ�����
    EXPECT_THROW(Shoupai("m123p456z34567,s789-,").zimo("m1"), std::runtime_error);
    // ���v�ƂȂ�v���c���邱�Ƃ��ł��邱��
    EXPECT_EQ("m123p456s789z34567m1", Shoupai("m123p456s789z34567").zimo("m1", false).toString());
    // 5���ڂ̔v�̓c����Ȃ�����
    EXPECT_THROW(Shoupai("m123p456s789z1111").zimo("z1"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m455556s789z1111").zimo("m0"), std::invalid_argument);
}

TEST(ShoupaiTest, dapai) {
    // �ݎq��Ŕv�ł��邱��
    EXPECT_EQ("m23p456s789z34567", Shoupai("m123p456s789z34567").dapai("m1").toString());
    // ���q��Ŕv�ł��邱��
    EXPECT_EQ("m123p56s789z34567", Shoupai("m123p456s789z34567").dapai("p4").toString());
    // ���q��Ŕv�ł��邱��
    EXPECT_EQ("m123p456s89z34567", Shoupai("m123p456s789z34567").dapai("s7").toString());
    // ���v��Ŕv�ł��邱��
    EXPECT_EQ("m123p456s789z4567", Shoupai("m123p456s789z34567").dapai("z3").toString());
    // �Ԕv��Ŕv�ł��邱��
    EXPECT_EQ("m123p46s789z34567", Shoupai("m123p406s789z34567").dapai("p0").toString());
    // ���[�`�ł��邱��
    EXPECT_EQ("m123p456s789z3456*", Shoupai("m123p456s789z34567").dapai("z7*").toString());
    // ���[�`��ɂ��c���؂�ȊO�̑Ŕv���ł��邱��(�`�F�b�N���Ȃ�)
    EXPECT_EQ("m123p456s789z1223*", Shoupai("m123p456s789z11223*").dapai("z1").toString());
    // �����v������ꍇ�A�C�ӂ̔v���Ŕv�ł��邱��
    EXPECT_EQ("_____________", Shoupai("______________").dapai("m1").toString());
    // �s���Ȕv�͑Ŕv�ł��Ȃ�����
    EXPECT_THROW(Shoupai("m123p456s789z34567").dapai(""), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p456s789z34567").dapai("z0"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p456s789z34567").dapai("z8"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p456s789z34567").dapai("mm"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p456s789z34567").dapai("xx"), std::invalid_argument);
    // �������v�͑Ŕv�ł��Ȃ�����
    EXPECT_THROW(Shoupai("m123p456s789z34567").dapai("_"), std::invalid_argument);
    // �Ŕv�̒���ɂ͑Ŕv�ł��Ȃ�����
    EXPECT_THROW(Shoupai("m123p456s789z4567").dapai("m1"), std::runtime_error);
    // ���v�ƂȂ�v��Ŕv���邱�Ƃ��ł��邱��
    EXPECT_EQ("m23p456s789z4567", Shoupai("m123p456s789z4567").dapai("m1", false).toString());
    // ��v�ɂȂ��v�͑Ŕv�ł��Ȃ�����
    EXPECT_THROW(Shoupai("m123p456s789z34567").dapai("z1"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p456s789z34567").dapai("p0"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p406s789z34567").dapai("p5"), std::invalid_argument);
}

TEST(ShoupaiTest, gang) {
    // �ݎq�ňÞȂł��邱��
    EXPECT_EQ("p456s789z4567,m1111", Shoupai("m1111p456s789z4567").gang("m1111").toString());
    // �ݎq�ŉ��Ȃł��邱��
    EXPECT_EQ("p456s789z4567,m111+1", Shoupai("m1p456s789z4567,m111+").gang("m111+1").toString());
    // ���q�ňÞȂł��邱��
    EXPECT_EQ("m123s789z4567,p5555", Shoupai("m123p5555s789z4567").gang("p5555").toString());
    // ���q�ŉ��Ȃł��邱��
    EXPECT_EQ("m123s789z4567,p555=5", Shoupai("m123p5s789z4567,p555=").gang("p555=5").toString());
    // ���q�ňÞȂł��邱��
    EXPECT_EQ("m123p456z4567,s9999", Shoupai("m123p456s9999z4567").gang("s9999").toString());
    // ���q�ŉ��Ȃł��邱��
    EXPECT_EQ("m123p456z4567,s999-9", Shoupai("m123p456s9z4567,s999-").gang("s999-9").toString());
    // ���v�ňÞȂł��邱��
    EXPECT_EQ("m123p456s789z6,z7777", Shoupai("m123p456s789z67777").gang("z7777").toString());
    // ���v�ŉ��Ȃł��邱��
    EXPECT_EQ("m123p456s789z6,z777+7", Shoupai("m123p456s789z67,z777+").gang("z777+7").toString());
    // �Ԕv���܂ވÞȂ��ł��邱��
    EXPECT_EQ("p456s789z4567,m5500", Shoupai("m0055p456s789z4567").gang("m5500").toString());
    // �Ԕv���܂މ��Ȃ��ł��邱��
    EXPECT_EQ("m123s789z4567,p505=5", Shoupai("m123p5s789z4567,p505=").gang("p505=5").toString());
    // �Ԕv�ŉ��Ȃł��邱��
    EXPECT_EQ("m123s789z4567,p555=0", Shoupai("m123p0s789z4567,p555=").gang("p555=0").toString());
    // ���[�`��ɂ��ÞȂł��邱��(�`�F�b�N���Ȃ�)
    EXPECT_EQ("p456s789z4567*,m1111", Shoupai("m1111p456s789z4567*").gang("m1111").toString());
    // ���[�`��ɂ����Ȃł��邱��(�`�F�b�N���Ȃ�)
    EXPECT_EQ("p456s789z4567*,m111+1", Shoupai("m1p456s789z4567*,m111+").gang("m111+1").toString());
    // �����v������ꍇ�A������g���ĈÞȂł��邱��
    EXPECT_EQ("__________,m5550", Shoupai("______________").gang("m5550").toString());
    // �����v������ꍇ�A������g���ĉ��Ȃł��邱��
    EXPECT_EQ("__________,m555=0", Shoupai("___________,m555=").gang("m555=0").toString());
    // ���q�ŞȂł��Ȃ�����
    EXPECT_THROW(Shoupai("m1112456s789z4567").gang("m456-"), std::invalid_argument);
    // ���q�ŞȂł��Ȃ�����
    EXPECT_THROW(Shoupai("m1112456s789z4567").gang("m111+"), std::invalid_argument);
    // �s���Ȗʎq�ŞȂł��Ȃ�����
    EXPECT_THROW(Shoupai("m1112456s789z4567").gang("m1112"), std::invalid_argument);
    // �s���Ȗʎq�ŞȂł��Ȃ�����
    EXPECT_THROW(Shoupai("m2456s789z4567,m111+").gang("m111+2"), std::invalid_argument);
    // �Ŕv�̒���ɞȂł��Ȃ�����
    EXPECT_THROW(Shoupai("m1111p456s789z456").gang("m1111"), std::runtime_error);
    // ���I�̒���ɞȂł��Ȃ�����
    EXPECT_THROW(Shoupai("m1111s789z4567,p456-,").gang("m1111"), std::runtime_error);
    // �Ȃ̒���ɞȂł��Ȃ�����
    EXPECT_THROW(Shoupai("m1111p4444s789z567").gang("m1111").gang("p4444"), std::runtime_error);
    // ���v�ƂȂ�Ȃ��ł��邱��
    EXPECT_EQ("p456s789z456,m1111", Shoupai("m1111p456s789z456").gang("m1111", false).toString());
    EXPECT_EQ("s789z4567,p456-,m1111", Shoupai("m1111s789z4567,p456-,").gang("m1111", false).toString());
    EXPECT_EQ("s789z567,m1111,p4444", Shoupai("m1111p4444s789z567").gang("m1111", false).gang("p4444", false).toString());
    // ��v��4���Ȃ��v�ňÞȂł��Ȃ�����
    EXPECT_THROW(Shoupai("m1112p456s789z4567").gang("m1111"), std::invalid_argument);
    // ��v�ɂȂ��v�ŉ��Ȃł��Ȃ�����
    EXPECT_THROW(Shoupai("m13p456s789z567,m222=").gang("m2222"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m10p456s789z567,m555=").gang("m5555"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m15p456s789z567,m555=").gang("m5550"), std::invalid_argument);
    // �������Ȃ��v�ŉ��Ȃł��Ȃ�����
    EXPECT_THROW(Shoupai("m12p456s789z567,m222=").gang("m111=1"), std::invalid_argument);
}

TEST(ShoupaiTest, menqian) {
    // ���I���Ȃ��ꍇ�A�����[���Ɣ��肷�邱��
    EXPECT_TRUE(Shoupai("m123p0s789z4567").menqian());
    // ���I������ꍇ�A�����[���Ɣ��肵�Ȃ�����
    EXPECT_FALSE(Shoupai("p0s789z4567,m123-").menqian());
    // �ÃJ���𕛘I�Ɣ��肵�Ȃ�����
    EXPECT_TRUE(Shoupai("m123p0s789,z1111").menqian());
}

TEST(ShoupaiTest, lizhi) {
    // �z�v���̓��[�`��ԂłȂ�����
    EXPECT_FALSE(Shoupai("_____________").lizhi());
    // ���[�`�錾�ɂ�胊�[�`��ԂɂȂ邱��
    EXPECT_TRUE(Shoupai("_____________").zimo("z7").dapai("z7_*").lizhi());
}

TEST(ShoupaiTest, get_dapai) {
    // �c������܂��͕��I����ȊO�͑Ŕv�ł��Ȃ�����
    EXPECT_THROW(Shoupai("m123p406s789z4567").get_dapai(), std::runtime_error);
    EXPECT_THROW(Shoupai("m123p406s789z2,z111+").get_dapai(), std::runtime_error);
    EXPECT_THROW(Shoupai("_____________").get_dapai(), std::runtime_error);
    EXPECT_THROW(Shoupai("__________,z111+").get_dapai(), std::runtime_error);
    // �c������͔C�ӂ̎�v��Ŕv�ł��邱��(�����[����)
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m2", "m3", "p4", "p0", "p6", "s7", "s8", "s9", "z1", "z2", "z3_" }),
        Shoupai("m123p406s789z11123").get_dapai());
    // �c������͔C�ӂ̎�v��Ŕv�ł��邱��(���I��)
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m2", "m3", "p4", "p0", "p6", "s7", "s8", "s9", "z1", "z2_" }),
        Shoupai("m123p406s789z12,z111+").get_dapai());
    // ���[�`��̓c���؂肵���ł��Ȃ�����
    EXPECT_EQ((std::vector<std::string>{
        "m1_" }),
        Shoupai("m123p456s789z1234m1*").get_dapai());
    // �Ԕv��P�Ƃ̔v�Ƃ��ċ�ʂ��邱��
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m2", "m3", "p4", "p0", "p5", "s7", "s8", "s9", "z1", "z2", "z3_" }),
        Shoupai("m123p405s789z11123").get_dapai());
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m2", "m3", "p4", "p5", "s7", "s8", "s9", "z1", "z2", "z3", "p0_" }),
        Shoupai("m123p45s789z11123p0").get_dapai());
    // ��o���ƃc���؂����ʂ��邱��
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m2", "m3", "p4", "p5", "s7", "s8", "s9", "z1", "z2", "z3", "p5_" }),
        Shoupai("m123p45s789z11123p5").get_dapai());
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m2", "m3", "p4", "p0", "p5", "s7", "s8", "s9", "z1", "z2", "p0_" }),
        Shoupai("m123p405s789z1112p0").get_dapai());
    // �����v�݂̂̎�v�ł͋�z���Ԃ�����
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("______________").get_dapai());
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("___________,m123-,").get_dapai());
    // ��ւ��ƂȂ�Ŕv��������Ȃ�����(���ʖ�)
    EXPECT_EQ((std::vector<std::string>{
        "m5", "p4", "p0", "p6", "s7", "s8", "s9", "z2", "z3" }),
        Shoupai("m145p406s789z23,m1-23,").get_dapai());
    EXPECT_EQ((std::vector<std::string>{
        "m5", "p4", "p0", "p6", "s7", "s8", "s9", "z2", "z3" }),
        Shoupai("m145p406s789z23,m234-,").get_dapai());
    // ��ւ��ƂȂ�Ŕv��������Ȃ�����(�ƒ���)
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m2", "m3", "p2", "p8", "s7", "s8", "s9","z2", "z3" }),
        Shoupai("m123p258s789z23,p45-6,").get_dapai());
    // ��ւ��ƂȂ�Ŕv��������Ȃ�����(�Ӓ���)
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m2", "m3", "p4", "p5", "p6", "s4", "s6", "z2", "z3" }),
        Shoupai("m123p456s467z23,s7-89,").get_dapai());
    // ��ւ��ƂȂ�Ŕv��������Ȃ�����(�|��)
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m2", "m3", "p4", "p5", "p6", "s7", "s8", "s9", "z2" }),
        Shoupai("m123p456s789z12,z111+,").get_dapai());
    // ��ւ��ƂȂ�Ŕv��������Ȃ�����(�Ԕv����̖�)
    EXPECT_EQ((std::vector<std::string>{
        "m6", "p4", "p5", "p6", "s7", "s8", "s9", "z1", "z2" }),
        Shoupai("m256p456s789z12,m340-,").get_dapai());
    // ��ւ��ƂȂ�Ŕv��������Ȃ�����(�Ԕv�Ŕv)
    EXPECT_EQ((std::vector<std::string>{
        "m6", "p4", "p5", "p6", "s7", "s8", "s9", "z1", "z2" }),
        Shoupai("m206p456s789z12,m345-,").get_dapai());
    // �Ԕv�ɂ��|���̍ۂɐ������򂢑ւ����肷�邱��
    EXPECT_EQ((std::vector<std::string>{
        "m2", "p1", "s1", "s2", "s6", "s7", "s8" }),
        Shoupai("m25p1s12678,z666+,m550-,").get_dapai());
    // ��ւ��̂��ߑŔv�ł��Ȃ��ꍇ�����邱��
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m14,p456-,s789-,z111+,m234-,").get_dapai());
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m14,p456-,s789-,z111+,m1-23,").get_dapai());
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m22,p456-,s789-,z111+,m12-3,").get_dapai());
    // �򂢑ւ����������Ƃ��ł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m4", "m5", "p4", "p0", "p6", "s7", "s8", "s9", "z2", "z3" }),
        Shoupai("m145p406s789z23,m1-23,").get_dapai(false));
}

TEST(ShoupaiTest, get_chi_mianzi) {
    // �c������ƕ��I�̒���Ƀ`�[�ł��Ȃ�����
    EXPECT_THROW(Shoupai("m123p456s789z12345").get_chi_mianzi("m1-"), std::runtime_error);
    EXPECT_THROW(Shoupai("m123p456s789z12,z333=,").get_chi_mianzi("m1-"), std::runtime_error);
    EXPECT_THROW(Shoupai("______________").get_chi_mianzi("m1-"), std::runtime_error);
    // �`�[�ł��郁���c���Ȃ��ꍇ
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m123p456s789z1234").get_chi_mianzi("m5-"));
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("_____________").get_chi_mianzi("m5-"));
    // �`�[�ł��郁���c��1�̏ꍇ
    EXPECT_EQ((std::vector<std::string>{
        "m123-" }),
        Shoupai("m123p456s789z1234").get_chi_mianzi("m3-"));
    // �`�[�ł��郁���c��2�̏ꍇ
    EXPECT_EQ((std::vector<std::string>{
        "m123-", "m23-4" }),
        Shoupai("m1234p456s789z123").get_chi_mianzi("m3-"));
    // �`�[�ł��郁���c��3�̏ꍇ
    EXPECT_EQ((std::vector<std::string>{
        "m123-", "m23-4", "m3-45" }),
        Shoupai("m12345p456s789z12").get_chi_mianzi("m3-"));
    // �Ԕv�Ń`�[�ł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "p40-6" }),
        Shoupai("m123p456s789z1234").get_chi_mianzi("p0-"));
    // �Ԕv�܂݂Ń`�[�ł��邱��
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
    // �Ԕv�Ȃ��̃����c���I�����邱��
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
    // �c���؂�̔v���`�[�ł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "m123-" }),
        Shoupai("m123p456s789z1234").get_chi_mianzi("m3_-"));
    // ���[�`�錾�v���`�[�ł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "m123-" }),
        Shoupai("m123p456s789z1234").get_chi_mianzi("m3*-"));
    // �c���؂胊�[�`�̐錾�v���`�[�ł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "m123-" }),
        Shoupai("m123p456s789z1234").get_chi_mianzi("m3_*-"));
    // ���[�`��̓`�[�ł��Ȃ�����
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m123p456s789z1234*").get_chi_mianzi("m3-"));
    // �Ŕv�I���ł��Ȃ��Ȃ�������܂߂Ȃ�����
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("s6789,m123-,p456-,z111+").get_chi_mianzi("s6-"));
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("s6789,m123-,p456-,z111+").get_chi_mianzi("s9-"));
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("s7889,m123-,p456-,z111+").get_chi_mianzi("s8-"));
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("s7899,m123-,p456-,z111+").get_chi_mianzi("s9-"));
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("s7789,m123-,p456-,z111+").get_chi_mianzi("s7-"));
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("s6678999,m123-,p456-").get_chi_mianzi("s6-"));
    // �򂢑ւ����ƂȂ�����������Ƃ��ł��邱��
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
    // �s���Ȕv�Ń`�[�ł��Ȃ�����
    EXPECT_THROW(Shoupai("m123p456s789z1234").get_chi_mianzi("mm-"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m123p456s789z1234").get_chi_mianzi("m1"), std::invalid_argument);
    // ���v�Ń`�[�ł��Ȃ�����
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m123p456s789z1234").get_chi_mianzi("z1-"));
    // ��ƈȊO����`�[�ł��Ȃ�����
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m123p456s789z1234").get_chi_mianzi("m1+"));
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m123p456s789z1234").get_chi_mianzi("m1="));
}

TEST(ShoupaiTest, get_gang_mianzi) {
    // �c������ƕ��I�̒���ɑ喾�Ȃł��Ȃ�����
    EXPECT_THROW(Shoupai("m111p456s789z12345").get_gang_mianzi("m1+"), std::runtime_error);
    EXPECT_THROW(Shoupai("m111p456s789z12,z333=,").get_gang_mianzi("m1+"), std::runtime_error);
    EXPECT_THROW(Shoupai("______________").get_gang_mianzi("m1-"), std::runtime_error);
    // �喾�Ȃł��郁���c���Ȃ��ꍇ
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m123p456s789z1122").get_gang_mianzi("z1+"));
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("_____________").get_gang_mianzi("z1="));
    // ���Ƃ���喾�Ȃł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "m1111+" }),
        Shoupai("m111p456s789z1234").get_gang_mianzi("m1+"));
    // �Ζʂ���喾�Ȃł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "p4444=" }),
        Shoupai("m123p444s789z1234").get_gang_mianzi("p4="));
    // ��Ƃ���喾�Ȃł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "s7777-" }),
        Shoupai("m123p456s777z1234").get_gang_mianzi("s7-"));
    // �Ԕv�ő喾�Ȃł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "p5550+" }),
        Shoupai("m123p555s789z1234").get_gang_mianzi("p0+"));
    // �Ԕv����̑喾�Ȃ��ł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "p5505+" }),
        Shoupai("m123p055s789z1234").get_gang_mianzi("p5+"));
    EXPECT_EQ((std::vector<std::string>{
        "p5005+" }),
        Shoupai("m123p005s789z1234").get_gang_mianzi("p5+"));
    EXPECT_EQ((std::vector<std::string>{
        "p0005+" }),
        Shoupai("m123p000s789z1234").get_gang_mianzi("p5+"));
    // �c���؂�̔v��喾�Ȃł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "m1111+" }),
        Shoupai("m111p456s789z1234").get_gang_mianzi("m1_+"));
    // ���[�`�錾�v��喾�Ȃł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "m1111+" }),
        Shoupai("m111p456s789z1234").get_gang_mianzi("m1*+"));
    // �c���؂胊�[�`�̐錾�v��喾�Ȃł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "m1111+" }),
        Shoupai("m111p456s789z1234").get_gang_mianzi("m1_*+"));
    // ���[�`��͑喾�Ȃł��Ȃ�����
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m111p456s789z1234*").get_gang_mianzi("m1+"));
    // �s���Ȕv�ő喾�Ȃł��Ȃ�����
    EXPECT_THROW(Shoupai("m111p555s999z1234").get_gang_mianzi("mm+"), std::invalid_argument);
    EXPECT_THROW(Shoupai("m111p555s999z1234").get_gang_mianzi("m1"), std::invalid_argument);
    // �Ŕv�ƕ��I�̒���ɂ͈ÞȂł��Ȃ�����
    EXPECT_THROW(Shoupai("m1111p555s999z123").get_gang_mianzi(), std::runtime_error);
    EXPECT_THROW(Shoupai("m1111p555s999,z333=").get_gang_mianzi(), std::runtime_error);
    EXPECT_THROW(Shoupai("m11112p555s999,z333=,").get_gang_mianzi(), std::runtime_error);
    EXPECT_THROW(Shoupai("_____________").get_gang_mianzi(), std::runtime_error);
    // �ÞȂł��郁���c���Ȃ��ꍇ
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m123p456s789z12345").get_gang_mianzi());
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("______________").get_gang_mianzi());
    // �ݎq�ňÞȂł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "m1111" }),
        Shoupai("m1111p456s789z1234").get_gang_mianzi());
    // ���q�ňÞȂł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "p4444" }),
        Shoupai("m123p4444s789z1234").get_gang_mianzi());
    // ���q�ňÞȂł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "s7777" }),
        Shoupai("m123p456s7777z1234").get_gang_mianzi());
    // ���v�ňÞȂł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "z1111" }),
        Shoupai("m123p456s789z11112").get_gang_mianzi());
    // �Ԕv����ňÞȂł��邱��
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
    // ���[�`����ÞȂł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "m1111" }),
        Shoupai("m111p456s789z1122m1*").get_gang_mianzi());
    // ���[�`��͑���Ȃł��Ȃ�����
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m111123p456s78z11m4*").get_gang_mianzi());
    // �ÞȂł��郁���c�������̏ꍇ
    EXPECT_EQ((std::vector<std::string>{
        "m1111", "z1111" }),
        Shoupai("m1111p456s789z1111").get_gang_mianzi());
    // �Ŕv�ƕ��I�̒���ɂ͉��Ȃł��Ȃ�����
    EXPECT_THROW(Shoupai("m1p555s999z123,m111-").get_gang_mianzi(), std::runtime_error);
    EXPECT_THROW(Shoupai("m1p555s999,z333=,m111-").get_gang_mianzi(), std::runtime_error);
    EXPECT_THROW(Shoupai("m12p555s999,z333=,m111-,").get_gang_mianzi(), std::runtime_error);
    EXPECT_THROW(Shoupai("__________,m111-,").get_gang_mianzi(), std::runtime_error);
    // ���Ȃł��郁���c���Ȃ��ꍇ
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("m123p456s789z12,z777+").get_gang_mianzi());
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("___________,z777+").get_gang_mianzi());
    // �ݎq�ŉ��Ȃł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "m111+1" }),
        Shoupai("m1p456s789z1234,m111+").get_gang_mianzi());
    // ���q�ŉ��Ȃł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "p444=4" }),
        Shoupai("m123p4s789z1234,p444=").get_gang_mianzi());
    // ���q�ŉ��Ȃł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "s777-7" }),
        Shoupai("m123p456s7z1234,s777-").get_gang_mianzi());
    // ���v�ŉ��Ȃł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "z111+1" }),
        Shoupai("m123p456s789z12,z111+").get_gang_mianzi());
    // �Ԕv�ŉ��Ȃł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "p555=0" }),
        Shoupai("m123p0s789z1234,p555=").get_gang_mianzi());
    // �Ԕv����ŉ��Ȃł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "p550-5" }),
        Shoupai("m123p5s789z1234,p550-").get_gang_mianzi());
    // ���[�`��͉��Ȃł��Ȃ�����
    EXPECT_EQ((std::vector<std::string>{}), Shoupai("p456s789z1234m1*,m111+").get_gang_mianzi());
    // ���Ȃł��郁���c�������̏ꍇ
    EXPECT_EQ((std::vector<std::string>{
        "m111+1", "p444=4" }),
        Shoupai("m1p4s789z123,m111+,p444=").get_gang_mianzi());
    // �ÞȂƉ��Ȃ������ɂł���ꍇ
    EXPECT_EQ((std::vector<std::string>{
        "m111+1", "z1111" }),
        Shoupai("m1p456s789z1111,m111+").get_gang_mianzi());
}
