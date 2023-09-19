#include "pch.h"
#include "../src_cpp/hule.h"

#include <fstream>
#include <filesystem>

TEST(HuleTest, hule_mianzi) {
    // ��ʎ�(�c���a��)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "z22_!", "m123", "p555", "s789", "z111" }}),
        hule_mianzi(Shoupai("m123p055s789z11122")));
    // ��ʎ�(�����a��)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "z22+!", "m123", "p555", "s789", "z111" }}),
        hule_mianzi(Shoupai("m123p055s789z1112"), "z2+"));
    // ��ʎ�(���I����)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "z22=!", "m123", "p555", "z111", "s7-89" }}),
        hule_mianzi(Shoupai("m123p055z1112,s7-89"), "z2="));
    // ���Ύq�`
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "m22", "m55-!", "p44", "p66", "s11", "s99", "z33" }}),
        hule_mianzi(Shoupai("m225p4466s1199z33"), "m0-"));
    // ���m���o�`(�c��)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "z77", "m1_!", "m9", "p1", "p9", "s1", "s9", "z1", "z2", "z3", "z4", "z5", "z6" }}),
        hule_mianzi(Shoupai("m9p19s19z12345677m1"), {}));
    // ���m���o�`(13�ʑ҂�����)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "m99+!", "m1", "p1", "p9", "s1", "s9", "z1", "z2", "z3", "z4", "z5", "z6", "z7" }}),
        hule_mianzi(Shoupai("m19p19s19z1234567"), "m9+"));
    // ��@�󓕌`
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "m55=!", "m111", "m234", "m678", "m999" },
        std::vector<std::string>{ "m11123456789995=!" }}),
        hule_mianzi(Shoupai("m1112345678999"), "m0="));
    // �a���`�ȊO(���v)
    EXPECT_EQ((std::vector<std::vector<std::string>>{}),
        hule_mianzi(Shoupai("m123p055s789z1122")));
    // �a���`�ȊO(�O�ʎq)
    EXPECT_EQ((std::vector<std::vector<std::string>>{}),
        hule_mianzi(Shoupai("___m123p055z2,s7-89"), "z2="));
    // �a���`�ȊO(��Ύq)
    EXPECT_EQ((std::vector<std::vector<std::string>>{}),
        hule_mianzi(Shoupai("m22")));
    // �a���`�ȊO(���m���o�e���p�C)
    EXPECT_EQ((std::vector<std::vector<std::string>>{}),
        hule_mianzi(Shoupai("m19p19s19z123456"), "z7="));
    // �a���`�ȊO(��@�󓕃e���p�C)
    EXPECT_EQ((std::vector<std::vector<std::string>>{}),
        hule_mianzi(Shoupai("m111234567899"), "m9="));
    // �����̘a���`�Ƃ��Ȃ�(���q�D��)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "z11_!", "m123", "m111", "p789", "p999" }}),
        hule_mianzi(Shoupai("m111123p789999z1z1")));
    // �����̘a���`(��u�������Ύq��)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "s88_!", "m234", "m234", "p567", "p567" },
        std::vector<std::string>{ "m22", "m33", "m44", "p55", "p66", "p77", "s88_!" }}),
        hule_mianzi(Shoupai("m223344p556677s88")));
    // �����̘a���`(���q�����q��)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "p99", "m123", "m123", "m123", "p7_!89" },
        std::vector<std::string>{ "p99", "m111", "m222", "m333", "p7_!89" }}),
        hule_mianzi(Shoupai("m111222333p89997")));
    // �����̘a���`(�����̑I���A���a���T���V���N��)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "m22", "m3_!45", "m345", "p234", "s234" },
        std::vector<std::string>{ "m55", "m23_!4", "m234", "p234", "s234" }}),
        hule_mianzi(Shoupai("m2234455p234s234m3")));
    // �����̘a���`(�Í����܂ތ`)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "s33", "m1_!23", "p567", "s345", "s666" },
        std::vector<std::string>{ "s66", "m1_!23", "p567", "s333", "s456" }}),
        hule_mianzi(Shoupai("m23p567s33345666m1")));
    // �����̘a���`(��@�󓕌`)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "s99", "s111", "s2_!34", "s456", "s789" },
        std::vector<std::string>{ "s11134456789992_!" }}),
        hule_mianzi(Shoupai("s1113445678999s2")));
}

TEST(HuleTest, hule) {
    // �p�����[�^�s��
    EXPECT_THROW(hule(Shoupai(), "m1", {}), std::invalid_argument);
    // �a���`�ȊO
    {
        auto hule_ = hule(Shoupai(), {}, {});
        EXPECT_EQ(0, hule_.defen);
    }
    // ���v�Z: ���a�c����20��
    {
        auto hule_ = hule(Shoupai("m345567p234s33789"), {}, {});
        EXPECT_EQ(20, hule_.fu);
    }
    // ���v�Z: ���a������30��
    {
        auto hule_ = hule(Shoupai("m345567p234s3378"), "s9=", {});
        EXPECT_EQ(30, hule_.fu);
    }
    // ���v�Z: �I�^���̐����ɕ��͂��Ȃ�
    {
        auto hule_ = hule(Shoupai("m112233p456z33s78"), "s9=", {});
        EXPECT_EQ(30, hule_.fu);
    }
    // ���v�Z: �ꕗ�̐�����2��
    {
        auto hule_ = hule(Shoupai("m112233p456z11s78"), "s9=", {});
        EXPECT_EQ(40, hule_.fu);
    }
    // ���v�Z: �����̐�����2��
    {
        auto hule_ = hule(Shoupai("m112233p456z22s78"), "s9=", {});
        EXPECT_EQ(40, hule_.fu);
    }
    // ���v�Z: �O���v�̐�����2��
    {
        auto hule_ = hule(Shoupai("m112233p456z55s78"), "s9=", {});
        EXPECT_EQ(40, hule_.fu);
    }
    // ���v�Z: �A���v�̐�����4��
    {
        Param param;
        param.menfeng = 0;
        auto hule_ = hule(Shoupai("m112233z444z11s78"), "s9=", param);
        EXPECT_EQ(50, hule_.fu);
    }
    // ���v�Z: �����v�̖�����2��
    {
        Param param;
        param.menfeng = 0;
        auto hule_ = hule(Shoupai("m123z11m88,p888+,s888-"), "m8=", param);
        EXPECT_EQ(30, hule_.fu);
    }
    // ���v�Z: ���v�̖�����4��
    {
        auto hule_ = hule(Shoupai("m123p22s99,z222+,p111-"), "s9=", {});
        EXPECT_EQ(40, hule_.fu);
    }
    // ���v�Z: �����v�̈Í���4��
    {
        auto hule_ = hule(Shoupai("z33p222777s888m23"), "m4=", {});
        EXPECT_EQ(50, hule_.fu);
    }
    // ���v�Z: ���v�̈Í���8��
    {
        auto hule_ = hule(Shoupai("s33p111999z555m23"), "m4=", {});
        EXPECT_EQ(60, hule_.fu);
    }
    // ���v�Z: ���v�Z: �����v�̖��Ȃ�8��
    {
        auto hule_ = hule(Shoupai("p33m22245667,s444+4"), "m8=", {});
        EXPECT_EQ(40, hule_.fu);
    }
    // ���v�Z: ���v�̖��Ȃ�16��
    {
        auto hule_ = hule(Shoupai("p33m23445667,z6666-"), "m8=", {});
        EXPECT_EQ(40, hule_.fu);
    }
    // ���v�Z: �����v�̈ÞȂ�16��
    {
        auto hule_ = hule(Shoupai("p33m23445667,s4444"), "m8=", {});
        EXPECT_EQ(50, hule_.fu);
    }
    // ���v�Z: ���v�̈ÞȂ�32��
    {
        auto hule_ = hule(Shoupai("p33m23445667,z7777"), "m8=", {});
        EXPECT_EQ(70, hule_.fu);
    }
    // ���v�Z: �c���a����2�����Z
    {
        auto hule_ = hule(Shoupai("p33m222s222345,s888-"), {}, {});
        EXPECT_EQ(40, hule_.fu);
    }
    // ���v�Z: �P�R�҂���2�����Z
    {
        auto hule_ = hule(Shoupai("m222s222345p3,s888-"), "p3=", {});
        EXPECT_EQ(40, hule_.fu);
    }
    // ���v�Z: �ƒ��҂���2�����Z
    {
        auto hule_ = hule(Shoupai("p33m222s22235,s888-"), "s4=", {});
        EXPECT_EQ(40, hule_.fu);
    }
    // ���v�Z: �Ӓ��҂���2�����Z
    {
        auto hule_ = hule(Shoupai("p33z111m12389,s222-"), "m7=", {});
        EXPECT_EQ(40, hule_.fu);
    }
    // ���v�Z: �򂢕��a��30��
    {
        auto hule_ = hule(Shoupai("m22p345678s34,s67-8"), "s5=", {});
        EXPECT_EQ(30, hule_.fu);
    }
    // ���v�Z: ���Ύq��25��
    {
        auto hule_ = hule(Shoupai("m2255p88s1166z1155"), {}, {});
        EXPECT_EQ(25, hule_.fu);
    }
    // ���v�Z: ���m���o�͕��Ȃ�
    {
        auto hule_ = hule(Shoupai("m19p19s1z12345677s9"), {}, {});
        EXPECT_EQ(0, hule_.fu);
    }
    // ���v�Z: ��@�󓕂͕��Ȃ�
    {
        auto hule_ = hule(Shoupai("m11123456789995"), {}, {});
        EXPECT_EQ(0, hule_.fu);
    }
    // �a����: ���Ȃ�
    {
        auto hule_ = hule(Shoupai("m344556s24678z66"), "s3=", {});
        EXPECT_EQ((std::vector<Hupai>{}), hule_.hupai);
    }
    // �a����: ����
    {
        Param param;
        param.hupai.lizhi = 1;
        auto hule_ = hule(Shoupai("m344556s24678z66*"), "s3=", param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "����", 1 } }), hule_.hupai);
    }
    // �a����: �_�u������
    {
        Param param;
        param.hupai.lizhi = 2;
        auto hule_ = hule(Shoupai("m344556s24678z66*"), "s3=", param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "�_�u������", 2 } }), hule_.hupai);
    }
    // �a����: �����E�ꔭ
    {
        Param param;
        param.hupai.lizhi = 1;
        param.hupai.yifa = true;
        auto hule_ = hule(Shoupai("m344556s24678z66*"), "s3=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ "����", 1 },
            Hupai{ "�ꔭ", 1 } }), hule_.hupai);
    }
    // �a����: �C��̌�
    {
        Param param;
        param.hupai.haidi = 1;
        auto hule_ = hule(Shoupai("m344556s24z66s3,s6-78"), {}, param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "�C��̌�", 1 } }), hule_.hupai);
    }
    // �a����: �͒ꝝ��
    {
        Param param;
        param.hupai.haidi = 2;
        auto hule_ = hule(Shoupai("m344556s24678z66"), "s3=", param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "�͒ꝝ��", 1 } }), hule_.hupai);
    }
    // �a����: ���J��
    {
        Param param;
        param.hupai.lingshang = true;
        auto hule_ = hule(Shoupai("m344556s24z66s3,s777+7"), {}, param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "���J��", 1 } }), hule_.hupai);
    }
    // �a����: ����
    {
        Param param;
        param.hupai.qianggang = true;
        auto hule_ = hule(Shoupai("m344556s24678z66"), "s3=", param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "����", 1 } }), hule_.hupai);
    }
    // �a����: �V�a
    {
        Param param;
        param.hupai.tianhu = true;
        auto hule_ = hule(Shoupai("m344556s24678z66s3"), {}, param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "�V�a", -1 } }), hule_.hupai);
    }
    // �a����: �n�a
    {
        Param param;
        param.hupai.tianhu = 2;
        auto hule_ = hule(Shoupai("m344556s24678z66s3"), {}, param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "�n�a", -1 } }), hule_.hupai);
    }
    // �a����: ��O�����̘a
    {
        auto hule_ = hule(Shoupai("m344556s24678z66s3"), {}, {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "��O�����̘a", 1 } }), hule_.hupai);
    }
    // �a����: �ꕗ ��
    {
        auto hule_ = hule(Shoupai("m345567s3378z111"), "s9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "�ꕗ ��", 1 } }), hule_.hupai);
    }
    // �a����: ���� ��
    {
        Param param;
        param.menfeng = 2;
        auto hule_ = hule(Shoupai("m345567s33789,z333+"), {}, param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "���� ��", 1 } }), hule_.hupai);
    }
    // �a����: �A���v ��
    {
        Param param;
        param.zhuangfeng = 1;
        auto hule_ = hule(Shoupai("m345567s33z22,s789-"), "z2=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ "�ꕗ ��", 1 },
            Hupai{ "���� ��", 1 } }), hule_.hupai);
    }
    // �a����: �|�v ��
    {
        auto hule_ = hule(Shoupai("m345567s33789,z555+5"), {}, {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "�|�v ��", 1 } }), hule_.hupai);
    }
    // �a����: �|�v ᢁE��
    {
        auto hule_ = hule(Shoupai("m345567s33,z6666+,z7777"), {}, {});
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ "�|�v �", 1 },
            Hupai{ "�|�v ��", 1 } }), hule_.hupai);
    }
    // �a����: ���a
    {
        auto hule_ = hule(Shoupai("z33m234456p78s123"), "p9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "���a", 1 } }), hule_.hupai);
    }
    // �a����: ���a�E�c��
    {
        auto hule_ = hule(Shoupai("z33m234456p78s123p9"), {}, {});
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ "��O�����̘a", 1 },
            Hupai{ "���a", 1 } }), hule_.hupai);
    }
    // �a����: �򂢕��a(���Ȃ�)
    {
        auto hule_ = hule(Shoupai("z33m234456p78,s1-23"), "p9=", {});
        EXPECT_EQ((std::vector<Hupai>{}), hule_.hupai);
    }
    // �a����: �f���
    {
        auto hule_ = hule(Shoupai("m22555p234s78,p777-"), "s6=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "�f���", 1 } }), hule_.hupai);
    }
    // �a����: �f���(���Ύq�`)
    {
        auto hule_ = hule(Shoupai("m2255p4488s33667"), "s7=", {});
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ "�f���", 1 },
            Hupai{ "���Ύq", 2 } }), hule_.hupai);
    }
    // �a����: ��u��
    {
        auto hule_ = hule(Shoupai("m33455p111s33789"), "m4=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "��u��", 1 } }), hule_.hupai);
    }
    // �a����: �򂢈�u��(���Ȃ�)
    {
        auto hule_ = hule(Shoupai("m33455p111s33,s78-9"), "m4=", {});
        EXPECT_EQ((std::vector<Hupai>{}), hule_.hupai);
    }
    // �a����: �O�F����
    {
        auto hule_ = hule(Shoupai("m567p567s2256799"), "s9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "�O�F����", 2 } }), hule_.hupai);
    }
    // �a����: �O�F����(�򂢉�����)
    {
        auto hule_ = hule(Shoupai("m567s2256799,p56-7"), "s9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "�O�F����", 1 } }), hule_.hupai);
    }
    // �a����: ��C�ʊ�
    {
        auto hule_ = hule(Shoupai("m12456789s33789"), "m3=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "��C�ʊ�", 2 } }), hule_.hupai);
    }
    // �a����: ��C�ʊ�(�򂢉�����)
    {
        auto hule_ = hule(Shoupai("m12789s33789,m4-56"), "m3=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "��C�ʊ�", 1 } }), hule_.hupai);
    }
    // �a����: ���S�ћ��
    {
        auto hule_ = hule(Shoupai("m123999p789z33s12"), "s3=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "���S�ћ��", 2 } }), hule_.hupai);
    }
    // �a����: ���S�ћ��(�򂢉�����)
    {
        auto hule_ = hule(Shoupai("m123p789z33s12,m999+"), "s3=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "���S�ћ��", 1 } }), hule_.hupai);
    }
    // �a����: ���Ύq
    {
        auto hule_ = hule(Shoupai("m115599p2233s8z22"), "s8=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "���Ύq", 2 } }), hule_.hupai);
    }
    // �a����: �΁X�a
    {
        auto hule_ = hule(Shoupai("m55888z333s22,p111="), "s2=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "�΁X�a", 2 } }), hule_.hupai);
    }
    // �a����: �O�Í�
    {
        auto hule_ = hule(Shoupai("p99s111m555,p345-,s3333"), {}, {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "�O�Í�", 2 } }), hule_.hupai);
    }
    // �a����: �O�Ȏq
    {
        auto hule_ = hule(Shoupai("p11m45,s2222+,m888=8,z4444"), "m3=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "�O�Ȏq", 2 } }), hule_.hupai);
    }
    // �a����: �O�F����
    {
        auto hule_ = hule(Shoupai("s12377m22,p222-,s222-"), "m2=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "�O�F����", 2 } }), hule_.hupai);
    }
    // �a����: ���V��(�΁X�a�`)
    {
        auto hule_ = hule(Shoupai("z11p11199,m111=,z333+"), "p9=", {});
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ "�΁X�a", 2 },
            Hupai{ "���V��", 2 } }), hule_.hupai);
    }
    // �a����: ���V��(���Ύq�`)
    {
        auto hule_ = hule(Shoupai("m1199p11s99z11335"), "z5=", {});
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ "���Ύq", 2 },
            Hupai{ "���V��", 2 } }), hule_.hupai);
    }
    // �a����: ���O��
    {
        auto hule_ = hule(Shoupai("z55577m567p22,z666-"), "p2=", {});
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ "�|�v ��", 1 },
            Hupai{ "�|�v �", 1 },
            Hupai{ "���O��", 2 } }), hule_.hupai);
    }
    // �a����: ����F
    {
        auto hule_ = hule(Shoupai("m111234789z1133"), "z3=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "����F", 3 } }), hule_.hupai);
    }
    // �a����: ����F(�򂢉�����)
    {
        auto hule_ = hule(Shoupai("z11333p23478,p111+"), "p9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "����F", 2 } }), hule_.hupai);
    }
    // �a����: ����F(���Ύq�`)
    {
        auto hule_ = hule(Shoupai("s11224488z22557"), "z7=", {});
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ "���Ύq", 2 },
            Hupai{ "����F", 3 } }), hule_.hupai);
    }
    // �a����: ���S�ћ��
    {
        auto hule_ = hule(Shoupai("m11s123p789s789m99"), "m9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "���S�ћ��", 3 } }), hule_.hupai);
    }
    // �a����: ���S�ћ��(�򂢉�����)
    {
        auto hule_ = hule(Shoupai("m11s123p789s78,m999="), "s9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "���S�ћ��", 2 } }), hule_.hupai);
    }
    // �a����: ���S�ћ��(�򂢉�����)
    {
        auto hule_ = hule(Shoupai("m11s123p789s78,m999="), "s9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "���S�ћ��", 2 } }), hule_.hupai);
    }
    // �a����: ��u��
    {
        auto hule_ = hule(Shoupai("m223344p667788s9"), "s9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "��u��", 3 } }), hule_.hupai);
    }
    // �a����: ��u��(4���g��)
    {
        auto hule_ = hule(Shoupai("m222233334444s9"), "s9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "��u��", 3 } }), hule_.hupai);
    }
    // �a����: �򂢓�u��(���Ȃ�)
    {
        auto hule_ = hule(Shoupai("m223344p678s9,p678-"), "s9=", {});
        EXPECT_EQ((std::vector<Hupai>{}), hule_.hupai);
    }
    // �a����: ����F
    {
        auto hule_ = hule(Shoupai("m1113456677778"), "m9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "����F", 6 } }), hule_.hupai);
    }
    // �a����: ����F(�򂢉�����)
    {
        auto hule_ = hule(Shoupai("p2344555,p12-3,p7-89"), "p1=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "����F", 5 } }), hule_.hupai);
    }
    // �a����: ����F(���Ύq�`)
    {
        auto hule_ = hule(Shoupai("s1122445577889"), "s9=", {});
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ "���Ύq", 2 },
            Hupai{ "����F", 6 } }), hule_.hupai);
    }
    // �a����: ���m���o
    {
        auto hule_ = hule(Shoupai("m119p19s19z1234567"), {}, {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "���m���o", -1 } }), hule_.hupai);
    }
    // �a����: ���m���o�\�O��
    {
        auto hule_ = hule(Shoupai("m19p19s19z1234567m1"), {}, {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "���m���o�\�O��", -2 } }), hule_.hupai);
    }
    // �a����: �l�Í�
    {
        auto hule_ = hule(Shoupai("m33m111p333s777z111"), {}, {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "�l�Í�", -1 } }), hule_.hupai);
    }
    // �a����: �l�Í��P�R
    {
        auto hule_ = hule(Shoupai("m111p333s777z111m3"), "m3=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "�l�Í��P�R", -2 } }), hule_.hupai);
    }
    // �a����: ��O��
    {
        auto hule_ = hule(Shoupai("z555m456p22z66,z777+"), "z6=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "��O��", -1 } }), hule_.hupai);
    }
    // �a����: ��O��(�p�I)
    {
        auto hule_ = hule(Shoupai("m2234,z555-5,z6666,z777+"), "m5=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "��O��", -1, '+'} }), hule_.hupai);
    }
    // �a����: ���l��
    {
        auto hule_ = hule(Shoupai("m234z2244,z333+,z111-"), "z4=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "���l��", -1 } }), hule_.hupai);
    }
    // �a����: ��l��
    {
        auto hule_ = hule(Shoupai("m22z22244,z333+,z111-"), "z4=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "��l��", -2 } }), hule_.hupai);
    }
    // �a����: ��l��(�p�I)
    {
        auto hule_ = hule(Shoupai("m2,z222+,z4444,z333+,z111-"), "m2=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "��l��", -2, '-'} }), hule_.hupai);
    }
    // �a����: ����F
    {
        auto hule_ = hule(Shoupai("z1112277,z555=,z444+"), "z7=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "����F", -1 } }), hule_.hupai);
    }
    // �a����: ����F(���Ύq�`)
    {
        auto hule_ = hule(Shoupai("z1122334455667"), "z7=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "����F", -1 } }), hule_.hupai);
    }
    // �a����: �Έ�F
    {
        auto hule_ = hule(Shoupai("s22334466z66,s888+"), "z6=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "�Έ�F", -1 } }), hule_.hupai);
    }
    // �a����: �Έ�F(ᢂȂ�)
    {
        auto hule_ = hule(Shoupai("s4466,s222=,s333+,s888-"), "s6=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "�Έ�F", -1 } }), hule_.hupai);
    }
    // �a����: ���V��
    {
        auto hule_ = hule(Shoupai("s11p111m11,s999-,m999="), "m1=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "���V��", -1 } }), hule_.hupai);
    }
    // �a����: �l�Ȏq
    {
        auto hule_ = hule(Shoupai("m1,z5555,p222+2,p777-7,s1111-"), "m1=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "�l�Ȏq", -1 } }), hule_.hupai);
    }
    // �a����: ��@��
    {
        auto hule_ = hule(Shoupai("m1112235678999"), "m4=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "��@��", -1 } }), hule_.hupai);
    }
    // �a����: ������@��
    {
        auto hule_ = hule(Shoupai("m1112345678999"), "m2=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "������@��", -2 } }), hule_.hupai);
    }

    // �h��: �h���Ȃ�
    {
        Param param;
        param.baopai = { "s1" };
        auto hule_ = hule(Shoupai("p55m234s78,m4-56,z111+"), "s9=", param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "�ꕗ ��", 1 } }), hule_.hupai);
    }
    // �h��: ��v��: 1
    {
        Param param;
        param.baopai = { "m2" };
        auto hule_ = hule(Shoupai("p55m234s78,m4-56,z111+"), "s9=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ "�ꕗ ��", 1 },
            Hupai{ "�h��", 1 } }), hule_.hupai);
    }
    // �h��: ��v��: 2
    {
        Param param;
        param.baopai = { "p4" };
        auto hule_ = hule(Shoupai("p55m234s78,m4-56,z111+"), "s9=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ "�ꕗ ��", 1 },
            Hupai{ "�h��", 2 } }), hule_.hupai);
    }
    // �h��: ��v��: 1, ���I��: 1
    {
        Param param;
        param.baopai = { "m3" };
        auto hule_ = hule(Shoupai("p55m23s789,m4-56,z111+"), "m4=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ "�ꕗ ��", 1 },
            Hupai{ "�h��", 2 } }), hule_.hupai);
    }
    // �h��: �ȃh��: 1
    {
        Param param;
        param.baopai = { "s1", "m2"};
        auto hule_ = hule(Shoupai("p55m234s78,m4-56,z111+"), "s9=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ "�ꕗ ��", 1 },
            Hupai{ "�h��", 1 } }), hule_.hupai);
    }
    // �h��: �ԃh��: 2
    {
        Param param;
        param.baopai = { "s4" };
        auto hule_ = hule(Shoupai("p50m234s78,m4-06,z111+"), "s9=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ "�ꕗ ��", 1 },
            Hupai{ "�ԃh��", 2 } }), hule_.hupai);
    }
    // �h��: �Ԃ̃_�u�h��
    {
        Param param;
        param.baopai = { "m4" };
        auto hule_ = hule(Shoupai("p55m234s78,m4-06,z111+"), "s9=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ "�ꕗ ��", 1 },
            Hupai{ "�h��", 1 },
            Hupai{ "�ԃh��", 1 } }), hule_.hupai);
    }
    // �h��: �h���\���v���Ԕv
    {
        Param param;
        param.baopai = { "m0" };
        auto hule_ = hule(Shoupai("p55m234s78,m4-56,z111+"), "s9=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ "�ꕗ ��", 1 },
            Hupai{ "�h��", 1 } }), hule_.hupai);
    }
    // �h��: ���h���Ȃ�
    {
        Param param;
        param.hupai.lizhi = 1;
        param.baopai = { "s9" };
        param.fubaopai = { "s9" };
        auto hule_ = hule(Shoupai("m344556s24678z66*"), "s3=", param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "����", 1 } }), hule_.hupai);
    }
    // �h��: ���h��: 1
    {
        Param param;
        param.hupai.lizhi = 1;
        param.baopai = { "s9" };
        param.fubaopai = { "m2" };
        auto hule_ = hule(Shoupai("m344556s24678z66*"), "s3=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ "����", 1 },
            Hupai{ "���h��", 1 } }), hule_.hupai);
    }
    // �h��: �h��: 1, ���h��: 1
    {
        Param param;
        param.hupai.lizhi = 1;
        param.baopai = { "m2" };
        param.fubaopai = { "m2" };
        auto hule_ = hule(Shoupai("m344556s24678z66*"), "s3=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ "����", 1 },
            Hupai{ "�h��", 1 },
            Hupai{ "���h��", 1 } }), hule_.hupai);
    }
    // �h��: �h���݂̂ł̘a���͕s��
    {
        Param param;
        param.baopai = { "m2" };
        auto hule_ = hule(Shoupai("m344556s24678z66"), "s3=", param);
        EXPECT_EQ((std::vector<Hupai>{}), hule_.hupai);
    }
    // �h��: �𖞂Ƀh���͂��Ȃ�
    {
        Param param;
        param.baopai = { "m9" };
        auto hule_ = hule(Shoupai("m119p19s19z1234567"), {}, param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ "���m���o", -1 } }), hule_.hupai);
    }

    // �_�v�Z: 20�� 2�| �q �c�� �� 400/700
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��O�����̘a", 1 },
                Hupai{ "���a", 1 } },
            20, 2, 0, 1500,
            std::array<int, 4>{ -700,  1500,  -400,  -400 } }),
            hule(Shoupai("z33m123p456s789m234"), {}, {}));
    }
    // �_�v�Z: 20�� 3�| �e �c�� �� 1300��
    {
        Param param;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��O�����̘a", 1 },
                Hupai{ "���a", 1 },
                Hupai{ "��u��", 1 } },
            20, 3, 0, 3900,
            std::array<int, 4>{ 3900, -1300, -1300, -1300 } }),
            hule(Shoupai("z33m123p456s789m231"), {}, param));
    }
    // �_�v�Z: 20�� 4�| �q �c�� �� 1300/2600
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��O�����̘a", 1 },
                Hupai{ "���a", 1 },
                Hupai{ "�O�F����", 2 } },
            20, 4, 0, 5200,
            std::array<int, 4>{ -2600,  5200, -1300, -1300 } }),
            hule(Shoupai("z33m123p234s234m234"), {}, {}));
    }
    // �_�v�Z: 25�� 2�| �q ���� �� 1600
    {
        Param param;
        param.jicun.lizhibang = 1;
        param.jicun.changbang = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "���Ύq", 2 } },
            25, 2, 0, 1600,
            std::array<int, 4>{ -1900,  2900,     0,     0 } }),
            hule(Shoupai("m1122p3344s5566z7"), "z7-", param));
    }
    // �_�v�Z: 25�� 3�| �e �c�� �� 1600��
    {
        Param param;
        param.menfeng = 0;
        param.jicun.lizhibang = 1;
        param.jicun.changbang = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��O�����̘a", 1 },
                Hupai{ "���Ύq", 2 } },
            25, 3, 0, 4800,
            std::array<int, 4>{ 6100, -1700, -1700, -1700 } }),
            hule(Shoupai("m1122p3344s5566z77"), {}, param));
    }
    // �_�v�Z: 25�� 4�| �q �c�� �� 1600/3200
    {
        Param param;
        param.jicun.lizhibang = 1;
        param.jicun.changbang = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��O�����̘a", 1 },
                Hupai{ "�f���", 1 },
                Hupai{ "���Ύq", 2 } },
            25, 4, 0, 6400,
            std::array<int, 4>{ -3300,  7700, -1700, -1700 } }),
            hule(Shoupai("m2277p3344s556688"), {}, param));
    }
    // �_�v�Z: 30�� 1�| �e ���� �� 1500
    {
        Param param;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�f���", 1 } },
            30, 1, 0, 1500,
            std::array<int, 4>{ 1500,     0, -1500,     0 } }),
            hule(Shoupai("m77234p456s67,m34-5"), "s8=", param));
    }
    // �_�v�Z: 30�� 2�| �q ���� �� 2000
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�f���", 1 },
                Hupai{ "�O�F����", 1 } },
            30, 2, 0, 2000,
            std::array<int, 4>{ -2000,  2000,     0,     0 } }),
            hule(Shoupai("m77234p345s34,m34-5"), "s5-", {}));
    }
    // �_�v�Z: 30�� 3�| �e �c�� �� 2000��
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��O�����̘a", 1 },
                Hupai{ "���� ��", 1 },
                Hupai{ "��u��", 1 } },
            30, 3, 0, 6000,
            std::array<int, 4>{ 6000, -2000, -2000, -2000 } }),
            hule(Shoupai("m22z111p445566s789"), {}, param));
    }
    // �_�v�Z: 30�� 4�| �q �c�� �� 2000/3900
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��O�����̘a", 1 },
                Hupai{ "�ꕗ ��", 1 },
                Hupai{ "���S�ћ��", 2 } },
            30, 4, 0, 7900,
            std::array<int, 4>{ -3900,  7900, -2000, -2000 } }),
            hule(Shoupai("m11z111p123789s789"), {}, {}));
    }
    // �_�v�Z: 40�� 1�| �e ���� �� 2000
    {
        Param param;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��u��", 1 } },
            40, 1, 0, 2000,
            std::array<int, 4>{ 2000,     0, -2000,     0 } }),
            hule(Shoupai("m11234234p456s89"), "s7=", param));
    }
    // �_�v�Z: 40�� 2�| �q ���� �� 2600
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�f���", 1 },
                Hupai{ "��u��", 1 } },
            40, 2, 0, 2600,
            std::array<int, 4>{ -2600,  2600,     0,     0 } }),
            hule(Shoupai("m22334455p456s68"), "s7-", {}));
    }
    // �_�v�Z: 40�� 3�| �e �c�� �� 2600��
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 },
                Hupai{ "�΁X�a", 2 } },
            40, 3, 0, 7800,
            std::array<int, 4>{ 7800, -2600, -2600, -2600 } }),
            hule(Shoupai("z33222m222,s222=,p999+"), {}, param));
    }
    // �_�v�Z: 40�� 4�| �q �c�� �� 2000/4000
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 },
                Hupai{ "���� ��", 1 },
                Hupai{ "�΁X�a", 2 } },
            40, 4, 0, 8000,
            std::array<int, 4>{ -4000,  8000, -2000, -2000 } }),
            hule(Shoupai("z33222m222,s222=,p999+"), {}, param));
    }
    // �_�v�Z: 50�� 1�| �e ���� �� 2400
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 } },
            50, 1, 0, 2400,
            std::array<int, 4>{ 2400,     0, -2400,     0 } }),
            hule(Shoupai("m123p456s789z2227"), "z7=", param));
    }
    // �_�v�Z: 50�� 2�| �q ���� �� 3200
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 },
                Hupai{ "���� ��", 1 } },
            50, 2, 0, 3200,
            std::array<int, 4>{ -3200,  3200,     0,     0 } }),
            hule(Shoupai("m123p456s789z2227"), "z7-", param));
    }
    // �_�v�Z: 50�� 3�| �e �c�� �� 3200��
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 },
                Hupai{ "�O�Í�", 2 } },
            50, 3, 0, 9600,
            std::array<int, 4>{ 9600, -3200, -3200, -3200 } }),
            hule(Shoupai("z33m222z222,p8888,s789-"), {}, param));
    }
    // �_�v�Z: 50�� 4�| �q �c�� �� 2000/4000
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 },
                Hupai{ "���� ��", 1 },
                Hupai{ "�O�Í�", 2 } },
            50, 4, 0, 8000,
            std::array<int, 4>{ -4000,  8000, -2000, -2000 } }),
            hule(Shoupai("z33m222z222,p8888,s789-"), {}, param));
    }
    // �_�v�Z: 60�� 1�| �e ���� �� 2900
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 } },
            60, 1, 0, 2900,
            std::array<int, 4>{ 2900,     0, -2900,     0 } }),
            hule(Shoupai("s789z2227,m2222,p111="), "z7=", param));
    }
    // �_�v�Z: 60�� 2�| �q ���� �� 3900
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 },
                Hupai{ "���� ��", 1 } },
            60, 2, 0, 3900,
            std::array<int, 4>{ -3900,  3900,     0,     0 } }),
            hule(Shoupai("s789z2227,m2222,p111="), "z7-", param));
    }
    // �_�v�Z: 60�� 3�| �e �c�� �� 3900��
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 },
                Hupai{ "����F", 2 } },
            60, 3, 0, 11700,
            std::array<int, 4>{ 11700, -3900, -3900, -3900 } }),
            hule(Shoupai("m11222789,z2222,m444="), {}, param));
    }
    // �_�v�Z: 60�� 4�| �q �c�� �� 2000/4000
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 },
                Hupai{ "���� ��", 1 },
                Hupai{ "����F", 2 } },
            60, 4, 0, 8000,
            std::array<int, 4>{ -4000,  8000, -2000, -2000 } }),
            hule(Shoupai("m11222789,z2222,m444="), {}, param));
    }
    // �_�v�Z: 70�� 1�| �e ���� �� 3400
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 } },
            70, 1, 0, 3400,
            std::array<int, 4>{ 3400,     0, -3400,     0 } }),
            hule(Shoupai("m12377p456s78,z2222"), "s9=", param));
    }
    // �_�v�Z: 70�� 2�| �q ���� �� 4500
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 },
                Hupai{ "���� ��", 1 } },
            70, 2, 0, 4500,
            std::array<int, 4>{ -4500,  4500,     0,     0 } }),
            hule(Shoupai("m12377p456s78,z2222"), "s9-", param));
    }
    // �_�v�Z: 70�� 3�| �e �c�� �� 4000��
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��O�����̘a", 1 },
                Hupai{ "�ꕗ ��", 1 },
                Hupai{ "��u��", 1 } },
            70, 3, 0, 12000,
            std::array<int, 4>{ 12000, -4000, -4000, -4000 } }),
            hule(Shoupai("p77s223344,z2222,m2222"), {}, param));
    }
    // �_�v�Z: 80�� 1�| �e ���� �� 3900
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 } },
            80, 1, 0, 3900,
            std::array<int, 4>{ 3900,     0, -3900,     0 } }),
            hule(Shoupai("m22s888p34,z222+2,z4444"), "p5=", param));
    }
    // �_�v�Z: 80�� 2�| �q ���� �� 5200
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 },
                Hupai{ "���� ��", 1 } },
            80, 2, 0, 5200,
            std::array<int, 4>{ -5200,  5200,     0,     0 } }),
            hule(Shoupai("m22s888p34,z222+2,z4444"), "p5-", param));
    }
    // �_�v�Z: 80�� 3�| �e �c�� �� 4000��
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 },
                Hupai{ "���� ��", 1 },
                Hupai{ "���S�ћ��", 1 } },
            80, 3, 0, 12000,
            std::array<int, 4>{ 12000, -4000, -4000, -4000 } }),
            hule(Shoupai("m11p999s123,z222+2,z1111"), {}, param));
    }
    // �_�v�Z: 90�� 1�| �e ���� �� 4400
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 } },
            90, 1, 0, 4400,
            std::array<int, 4>{ 4400,     0, -4400,     0 } }),
            hule(Shoupai("p88m123s99,s6666,z2222"), "s9=", param));
    }
    // �_�v�Z: 90�� 2�| �q ���� �� 5800
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 },
                Hupai{ "���� ��", 1 } },
            90, 2, 0, 5800,
            std::array<int, 4>{ -5800,  5800,     0,     0 } }),
            hule(Shoupai("p88m123s99,s6666,z2222"), "s9-", param));
    }
    // �_�v�Z: 90�� 3�| �e �c�� �� 4000��
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 },
                Hupai{ "�|�v ��", 1 },
                Hupai{ "�|�v �", 1 } },
            90, 3, 0, 12000,
            std::array<int, 4>{ 12000, -4000, -4000, -4000 } }),
            hule(Shoupai("m22s345,z5555,z2222,z666-"), {}, param));
    }
    // �_�v�Z: 100�� 1�| �e ���� �� 4800
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 } },
            100, 1, 0, 4800,
            std::array<int, 4>{ 4800,     0, -4800,     0 } }),
            hule(Shoupai("m22p345s67,z2222,s9999"), "s8=", param));
    }
    // �_�v�Z: 100�� 2�| �q ���� �� 6400
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 },
                Hupai{ "���� ��", 1 } },
            100, 2, 0, 6400,
            std::array<int, 4>{ -6400,  6400,     0,     0 } }),
            hule(Shoupai("m22p345s67,z2222,s9999"), "s8-", param));
    }
    // �_�v�Z: 100�� 3�| �e �c�� �� 4000��
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��O�����̘a", 1 },
                Hupai{ "�O�Í�", 2 } },
            100, 3, 0, 12000,
            std::array<int, 4>{ 12000, -4000, -4000, -4000 } }),
            hule(Shoupai("z11m999p243,s1111,s9999"), {}, param));
    }
    // �_�v�Z: 110�� 1�| �e ���� �� 5300
    {
        Param param;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�|�v ��", 1 } },
            110, 1, 0, 5300,
            std::array<int, 4>{ 5300,     0, -5300,     0 } }),
            hule(Shoupai("m234z1177,p1111,s9999"), "z7=", param));
    }
    // �_�v�Z: 110�� 2�| �q ���� �� 7100
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�|�v ��", 1 },
                Hupai{ "�|�v ��", 1 } },
            110, 2, 0, 7100,
            std::array<int, 4>{ -7100,  7100,     0,     0 } }),
            hule(Shoupai("m234z2277,p1111,z5555"), "z7-", param));
    }
    // �_�v�Z: 110�� 3�| �e �c�� �� 4000��
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�|�v ��", 1 },
                Hupai{ "�O�Ȏq", 2 } },
            110, 3, 0, 12000,
            std::array<int, 4>{ 12000, -4000, -4000, -4000 } }),
            hule(Shoupai("m243z11,p1111,s9999,z555+5"), {}, param));
    }
    // �_�v�Z: 5�| �e ���� �� 12000
    {
        Param param;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "���a", 1 },
                Hupai{ "�f���", 1 },
                Hupai{ "��u��", 1 },
                Hupai{ "�O�F����", 2 } },
            30, 5, 0, 12000,
            std::array<int, 4>{ 12000,     0,-12000,     0 } }),
            hule(Shoupai("m22456p456s44556"), "s6=", param));
    }
    // �_�v�Z: 6�| �q �c�� �� 3000/6000
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��O�����̘a", 1 },
                Hupai{ "���a", 1 },
                Hupai{ "�f���", 1 },
                Hupai{ "��u��", 1 },
                Hupai{ "�O�F����", 2 } },
            20, 6, 0, 12000,
            std::array<int, 4>{ -6000, 12000, -3000, -3000 } }),
            hule(Shoupai("m22456p456s445566"), {}, {}));
    }
    // �_�v�Z: 7�| �e ���� �� 18000
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 },
                Hupai{ "�΁X�a", 2 },
                Hupai{ "���V��", 2 },
                Hupai{ "����F", 2 } },
            50, 7, 0, 18000,
            std::array<int, 4>{ 18000,     0,-18000,     0 } }),
            hule(Shoupai("m111z3334,z222=,m999-"), "z4=", param));
    }
    // �_�v�Z: 8�| �q �c�� �� 4000/8000
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�ꕗ ��", 1 },
                Hupai{ "���� ��", 1 },
                Hupai{ "�΁X�a", 2 },
                Hupai{ "���V��", 2 },
                Hupai{ "����F", 2 } },
            50, 8, 0, 16000,
            std::array<int, 4>{ -8000, 16000, -4000, -4000 } }),
            hule(Shoupai("m111z333444,z222=,m999-"), {}, param));
    }
    // �_�v�Z: 9�| �e ���� �� 24000
    {
        Param param;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�f���", 1 },
                Hupai{ "�O�Í�", 2 },
                Hupai{ "����F", 6 } },
            50, 9, 0, 24000,
            std::array<int, 4>{ 24000,     0,-24000,     0 } }),
            hule(Shoupai("s2223334455567"), "s8=", param));
    }
    // �_�v�Z: 10�| �q �c�� �� 4000/8000
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��O�����̘a", 1 },
                Hupai{ "�f���", 1 },
                Hupai{ "�O�Í�", 2 },
                Hupai{ "����F", 6 } },
            40, 10, 0, 16000,
            std::array<int, 4>{ -8000, 16000, -4000, -4000 } }),
            hule(Shoupai("s22233344555678"), {}, {}));
    }
    // �_�v�Z: 11�| �e ���� �� 36000
    {
        Param param;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "���a", 1 },
                Hupai{ "�f���", 1 },
                Hupai{ "��u��", 3 },
                Hupai{ "����F", 6 } },
            30, 11, 0, 36000,
            std::array<int, 4>{ 36000,     0,-36000,     0 } }),
            hule(Shoupai("p2233445566778"), "p8=", param));
    }
    // �_�v�Z: 12�| �q �c�� �� 6000/12000
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��O�����̘a", 1 },
                Hupai{ "���a", 1 },
                Hupai{ "�f���", 1 },
                Hupai{ "��u��", 3 },
                Hupai{ "����F", 6 } },
            20, 12, 0, 24000,
            std::array<int, 4>{ -12000, 24000, -6000, -6000 } }),
            hule(Shoupai("p22334455667788"), {}, {}));
    }
    // �_�v�Z: 13�| �e ���� �� 48000
    {
        Param param;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "���a", 1 },
                Hupai{ "���S�ћ��", 3 },
                Hupai{ "��u��", 3 },
                Hupai{ "����F", 6 } },
            30, 13, 0, 48000,
            std::array<int, 4>{ 48000,     0,-48000,     0 } }),
            hule(Shoupai("m1177778888999"), "m9=", param));
    }
    // �_�v�Z: �𖞕��� �q �c�� �� 24000/48000
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��l��", -2 },
                Hupai{ "����F", -1 } },
            0, 0, 3, 96000,
            std::array<int, 4>{ -48000, 96000,-24000,-24000 } }),
            hule(Shoupai("z77111z444,z222+,z333-"), {}, {}));
    }
    // �_�v�Z: �𖞃p�I ���e�҂Ȃ��A�ӔC����
    {
        Param param;
        param.menfeng = 0;
        param.jicun.lizhibang = 1;
        param.jicun.changbang = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��O��", -1, '-' } },
            0, 0, 1, 48000,
            std::array<int, 4>{ 49300,     0,     0,-48300 } }),
            hule(Shoupai("m11p456,z555+,z666=,z777-"), {}, param));
    }
    // �_�v�Z: �𖞃p�I ���e�҂���A���e�҂Ɛܔ�
    {
        Param param;
        param.menfeng = 0;
        param.jicun.lizhibang = 1;
        param.jicun.changbang = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��O��", -1, '-'} },
            0, 0, 1, 48000,
            std::array<int, 4>{ 49300,     0,-24300,-24000 } }),
            hule(Shoupai("m11p45,z555+,z666=,z777-"), "p6=", param));
    }
    // �_�v�Z: �𖞃p�I �p�I�����e�A�S�z�ӔC����
    {
        Param param;
        param.menfeng = 0;
        param.jicun.lizhibang = 1;
        param.jicun.changbang = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��O��", -1, '-' } },
            0, 0, 1, 48000,
            std::array<int, 4>{ 49300,     0,     0,-48300 } }),
            hule(Shoupai("m11p45,z555+,z666=,z777-"), "p6-", param));
    }
    // �_�v�Z: �_�u���𖞃p�I ���e�҂Ȃ��A�֘A�𖞂̂ݐӔC����
    {
        Param param;
        param.jicun.lizhibang = 1;
        param.jicun.changbang = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��l��", -2, '+' },
                Hupai{ "����F", -1 } },
            0, 0, 3, 96000,
            std::array<int, 4>{ -16100, 97300,-72100, -8100 } }),
            hule(Shoupai("z77,z111-,z2222,z333=3,z444+"), {}, param));
    }
    // �_�v�Z: �_�u���𖞃p�I ���e�҂���A�֘A�𖞂̂ݕ��e�҂Ɛܔ�
    {
        Param param;
        param.jicun.lizhibang = 1;
        param.jicun.changbang = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��l��", -2, '+' },
                Hupai{ "����F", -1 } },
            0, 0, 3, 96000,
            std::array<int, 4>{ -64300, 97300,-32000,     0 } }),
            hule(Shoupai("z7,z111-,z2222,z333=3,z444+"), "z7-", param));
    }
    // �_�v�Z: �_�u���𖞃p�I �p�I�����e�A�S�z�ӔC����
    {
        Param param;
        param.jicun.lizhibang = 1;
        param.jicun.changbang = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��l��", -2, '+' },
                Hupai{ "����F", -1 } },
            0, 0, 3, 96000,
            std::array<int, 4>{ 0, 97300,-96300,     0 } }),
            hule(Shoupai("z7,z111-,z2222,z333=3,z444+"), "z7+", param));
    }
    // ���_�@: ���Ύq�Ɠ�u���̑I��
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�f���", 1 },
                Hupai{ "��u��", 3 } },
            40, 4, 0, 8000,
            std::array<int, 4>{ 0,  8000,    0, -8000 } }),
            hule(Shoupai("m223344p556677s8"), "s8=", {}));
    }
    // ���_�@: �������2�̑I��
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�f���", 1 },
                Hupai{ "��u��", 1 },
                Hupai{ "�O�F����", 2 } },
            40, 4, 0, 8000,
            std::array<int, 4>{ 0,  8000,    0, -8000 } }),
            hule(Shoupai("m2234455p234s234"), "m3=", {}));
    }
    // ���_�@: ���q�ƍ��q�̑I��
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��u��", 1 },
                Hupai{ "���S�ћ��", 3 } },
            40, 4, 0, 8000,
            std::array<int, 4>{ 0,  8000,    0, -8000 } }),
            hule(Shoupai("m111222333p8999"), "p7=", {}));
    }
    // ���_�@: �ƒ��҂����ʑ҂��̑I��
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�|�v ��", 1 } },
            50, 1, 0, 1600,
            std::array<int, 4>{ 0,  1600,    0, -1600 } }),
            hule(Shoupai("m12334p567z11z777"), "m2=", {}));
    }
    // ���_�@: ���_�������ꍇ�͖|������������I��
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "���a", 1 },
                Hupai{ "��u��", 1 },
                Hupai{ "���S�ћ��", 3 } },
            30, 5, 0, 8000,
            std::array<int, 4>{ 0,  8000,    0, -8000 } }),
            hule(Shoupai("m111222333p7899"), "p9=", {}));
    }
    // ���_�@: ���_�E�|���������ꍇ�͕�����������I��
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "�O�Í�", 2 },
                Hupai{ "����F", 6 } },
            50, 8, 0, 16000,
            std::array<int, 4>{ 0, 16000,    0,-16000 } }),
            hule(Shoupai("s1112223335578"), "s9=", {}));
    }
    // ���_�@: �𖞂Ɛ����𖞂ł͖𖞂�I��
    {
        Param param;
        param.hupai.lizhi = 1;
        param.hupai.yifa = true;
        param.baopai = { "m2" };
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ "��@��", -1 } },
            0, 0, 1, 32000,
            std::array<int, 4>{ -16000, 32000, -8000, -8000 } }),
            hule(Shoupai("m11123457899996"), {}, param));
    }
    // �a���_�v�Z: 10000�p�^�[��
    {
        std::ifstream ifs("../../tests_cpp/data/hule.json");
        nlohmann::json data;
        ifs >> data;
        int count = 0;
        for (const auto& t : data) {
            Param param;
            const auto& in = t["in"];
            const auto& in_param = in["param"];
            param.zhuangfeng = in_param["zhuangfeng"].get<int>();
            param.menfeng = in_param["menfeng"].get<int>();
            const auto& in_hupai = in_param["hupai"];
            if (in_hupai.contains("lizhi")) param.hupai.lizhi = in_hupai["lizhi"].get<int>();
            if (in_hupai.contains("yifa")) param.hupai.yifa = in_hupai["yifa"].get<bool>();
            if (in_hupai.contains("lingshang")) param.hupai.lingshang = in_hupai["lingshang"].get<bool>();
            if (in_hupai.contains("haidi")) param.hupai.haidi = in_hupai["haidi"].get<int>();
            param.baopai = in_param["baopai"].get<std::vector<std::string>>();
            param.fubaopai = in_param["fubaopai"].get<std::vector<std::string>>();
            const auto& in_jicun = in_param["jicun"];
            if (in_jicun.contains("changbang")) param.jicun.changbang = in_jicun["changbang"].get<int>();
            if (in_jicun.contains("lizhibang")) param.jicun.lizhibang = in_jicun["lizhibang"].get<int>();

            std::string rongpai;
            if (in.contains("rongpai")) rongpai = in["rongpai"].get<std::string>();

            const auto& out = t["out"];
            Defen defen;
            for (const auto& out_hupai : out["hupai"]) {
                auto s = out_hupai["name"].get<std::string>();
                auto b = (s == u8"�f���");
                defen.hupai.emplace_back(out_hupai["name"].get<std::string>(), out_hupai["fanshu"].get<int>());
            }
            defen.fu = out["fu"].get<int>();
            defen.fanshu = out["fanshu"].get<int>();
            defen.damanguan = out["damanguan"].is_null() ? 0 : out["damanguan"].get<int>();
            defen.defen = out["defen"].get<int>();
            defen.fenpei = out["fenpei"].get<std::array<int, 4>>();

            auto hule_ = hule(Shoupai(in["shoupai"].get<std::string>()), rongpai, param);
            EXPECT_EQ(defen, hule_);
            if (count++ > 5) break;
        }
    }
}
