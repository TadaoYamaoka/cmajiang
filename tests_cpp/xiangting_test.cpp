#include "pch.h"
#include "../src_cpp/xiangting.h"

#include <fstream>
#include <filesystem>

TEST(XiangtingTest, xiangting_yiban) {
    // ��̎�v��13����
    EXPECT_EQ(13, xiangting_yiban(Shoupai()));
    // ���v�`
    EXPECT_EQ(0, xiangting_yiban(Shoupai("m123p406s789z1122")));
    //�a���`
    EXPECT_EQ(-1, xiangting_yiban(Shoupai("m123p456s789z11222")));
    // ���I����
    EXPECT_EQ(-1, xiangting_yiban(Shoupai("m123p456s789z11222")));
    // �����Ȃ�
    EXPECT_EQ(1, xiangting_yiban(Shoupai("m12389p456s12789z1")));
    // ���q�ߑ�
    EXPECT_EQ(1, xiangting_yiban(Shoupai("m12389p456s1289z11")));
    // ���q�s��
    EXPECT_EQ(2, xiangting_yiban(Shoupai("m133345568z23677")));
    // ���v: �����Ȃ�4�ʎq
    EXPECT_EQ(1, xiangting_yiban(Shoupai("p234s567,m222=,p0-67")));
    // ���q�{���q
    EXPECT_EQ(4, xiangting_yiban(Shoupai("p222345z1234567")));
    // ���q�{�Ǘ��v�{���q
    EXPECT_EQ(4, xiangting_yiban(Shoupai("p2344456z123456")));
    // �Ύq�{���q�{���q
    EXPECT_EQ(3, xiangting_yiban(Shoupai("p11222345z12345")));
    // �Ύq�{���q�{���q�{�Ύq
    EXPECT_EQ(2, xiangting_yiban(Shoupai("p2234556788z123")));
    // ���I����̔v�p���a���`
    EXPECT_EQ(0, xiangting_yiban(Shoupai("m11122,p123-,s12-3,z111=,")));
    // ��ʎ�: 10000�p�^�[��
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_1.json");
        nlohmann::json data1;
        ifs >> data1;
        for (const auto& data : data1) {
            EXPECT_EQ(data["x"][0].get<int>(), xiangting_yiban(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // �����: 10000�p�^�[��
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_2.json");
        nlohmann::json data2;
        ifs >> data2;
        for (const auto& data : data2) {
            EXPECT_EQ(data["x"][0].get<int>(), xiangting_yiban(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // �����: 10000�p�^�[��
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_3.json");
        nlohmann::json data3;
        ifs >> data3;
        for (const auto& data : data3) {
            EXPECT_EQ(data["x"][0].get<int>(), xiangting_yiban(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // ���m��: 10000�p�^�[��
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_4.json");
        nlohmann::json data4;
        ifs >> data4;
        for (const auto& data : data4) {
            EXPECT_EQ(data["x"][0].get<int>(), xiangting_yiban(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
}

TEST(XiangtingTest, xiangting_guoshi) {
    // ��̎�v��13����
    EXPECT_EQ(13, xiangting_guoshi(Shoupai()));
    // ���v�Ȃ�
    EXPECT_EQ(13, xiangting_guoshi(Shoupai("m23455p345s45678")));
    // �����Ȃ�
    EXPECT_EQ(4, xiangting_guoshi(Shoupai("m189p12s249z12345")));
    // ��������
    EXPECT_EQ(3, xiangting_guoshi(Shoupai("m119p12s299z12345")));
    // ���v�`
    EXPECT_EQ(0, xiangting_guoshi(Shoupai("m11p19s19z1234567")));
    // ���v�`(13�ʒ�)
    EXPECT_EQ(0, xiangting_guoshi(Shoupai("m19p19s19z1234567")));
    // �a���`
    EXPECT_EQ(-1, xiangting_guoshi(Shoupai("m119p19s19z1234567")));
    // ���I����
    EXPECT_EQ(INT_MAX, xiangting_guoshi(Shoupai("m19p19s19z1234,z777=")));
    // ���v
    EXPECT_EQ(-1, xiangting_guoshi(Shoupai("m19p19s19z12345677").zimo("m1", false)));
    // ���v
    EXPECT_EQ(1, xiangting_guoshi(Shoupai("m119p19s19z12345")));
    // ��ʎ�: 10000�p�^�[��
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_1.json");
        nlohmann::json data1;
        ifs >> data1;
        for (const auto& data : data1) {
            EXPECT_EQ(data["x"][1].get<int>(), xiangting_guoshi(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // �����: 10000�p�^�[��
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_2.json");
        nlohmann::json data2;
        ifs >> data2;
        for (const auto& data : data2) {
            EXPECT_EQ(data["x"][1].get<int>(), xiangting_guoshi(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // �����: 10000�p�^�[��
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_3.json");
        nlohmann::json data3;
        ifs >> data3;
        for (const auto& data : data3) {
            EXPECT_EQ(data["x"][1].get<int>(), xiangting_guoshi(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // ���m��: 10000�p�^�[��
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_4.json");
        nlohmann::json data4;
        ifs >> data4;
        for (const auto& data : data4) {
            EXPECT_EQ(data["x"][1].get<int>(), xiangting_guoshi(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
}

TEST(XiangtingTest, xiangting_qidui) {
    // ��̎�v��13����
    EXPECT_EQ(13, xiangting_qidui(Shoupai()));
    // �Ύq�Ȃ�
    EXPECT_EQ(6, xiangting_qidui(Shoupai("m19p19s19z1234567")));
    // �Ȏq����
    EXPECT_EQ(2, xiangting_qidui(Shoupai("m1188p288s05z1111")));
    // �Í�����
    EXPECT_EQ(1, xiangting_qidui(Shoupai("m1188p2388s05z111")));
    // �Í�2��
    EXPECT_EQ(2, xiangting_qidui(Shoupai("m1188p288s055z111")));
    // ���v�`
    EXPECT_EQ(0, xiangting_qidui(Shoupai("m1188p288s05z1177")));
    // �a���`
    EXPECT_EQ(-1, xiangting_qidui(Shoupai("m1188p288s05z1177p2")));
    // ���I����
    EXPECT_EQ(INT_MAX, xiangting_qidui(Shoupai("m1188p288s05z2,z111=")));
    // ���v: 8�Ύq
    EXPECT_EQ(-1, xiangting_qidui(Shoupai("m1188p2288s05z1122").zimo("z7", false).zimo("z7", false)));
    // ���v
    EXPECT_EQ(3, xiangting_qidui(Shoupai("m1188s05z1122")));
    // ��ʎ�: 10000�p�^�[��
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_1.json");
        nlohmann::json data1;
        ifs >> data1;
        for (const auto& data : data1) {
            EXPECT_EQ(data["x"][2].get<int>(), xiangting_qidui(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // �����: 10000�p�^�[��
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_2.json");
        nlohmann::json data2;
        ifs >> data2;
        for (const auto& data : data2) {
            EXPECT_EQ(data["x"][2].get<int>(), xiangting_qidui(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // �����: 10000�p�^�[��
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_3.json");
        nlohmann::json data3;
        ifs >> data3;
        for (const auto& data : data3) {
            EXPECT_EQ(data["x"][2].get<int>(), xiangting_qidui(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // ���m��: 10000�p�^�[��
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_4.json");
        nlohmann::json data4;
        ifs >> data4;
        for (const auto& data : data4) {
            EXPECT_EQ(data["x"][2].get<int>(), xiangting_qidui(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
}

TEST(XiangtingTest, xiangting) {
    // ��ʌ`���v
    EXPECT_EQ(0, xiangting(Shoupai("m123p406s789z1122")));
    // ���m���o�`���v
    EXPECT_EQ(0, xiangting(Shoupai("m19p19s19z1234567")));
    // ���Ύq�`���v
    EXPECT_EQ(0, xiangting(Shoupai("m1188p288s05z1177")));
    // ��ʎ�: 10000�p�^�[��
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_1.json");
        nlohmann::json data1;
        ifs >> data1;
        for (const auto& data : data1) {
            EXPECT_EQ(std::min({ data["x"][0].get<int>(), data["x"][1].get<int>(), data["x"][2].get<int>() }),
                xiangting(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // �����: 10000�p�^�[��
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_2.json");
        nlohmann::json data2;
        ifs >> data2;
        for (const auto& data : data2) {
            EXPECT_EQ(std::min({ data["x"][0].get<int>(), data["x"][1].get<int>(), data["x"][2].get<int>() }),
                xiangting(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // �����: 10000�p�^�[��
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_3.json");
        nlohmann::json data3;
        ifs >> data3;
        for (const auto& data : data3) {
            EXPECT_EQ(std::min({ data["x"][0].get<int>(), data["x"][1].get<int>(), data["x"][2].get<int>() }),
                xiangting(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // ���m��: 10000�p�^�[��
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_4.json");
        nlohmann::json data4;
        ifs >> data4;
        for (const auto& data : data4) {
            EXPECT_EQ(std::min({ data["x"][0].get<int>(), data["x"][1].get<int>(), data["x"][2].get<int>() }),
                xiangting(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
}

TEST(XiangtingTest, tingpai) {
    // �Ŕv�\�ȏ�Ԃ̂Ƃ��A�G���[�ƂȂ邱��
    EXPECT_THROW(tingpai(Shoupai("m123p456s789z12345")), std::runtime_error);
    EXPECT_THROW(tingpai(Shoupai("m123p456z12345,s789-,")), std::runtime_error);
    // ���I�Ȃ�
    EXPECT_EQ((std::vector<std::string>{
        "z1", "z2", "z3", "z4" }),
        tingpai(Shoupai("m123p456s789z1234")));
    // ���I����
    EXPECT_EQ((std::vector<std::string>{
        "z1", "z2", "z3", "z4" }),
        tingpai(Shoupai("m123p456z1234,s789-")));
    // ���m���o13�ʑ҂�
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m9", "p1", "p9", "s1", "s9", "z1", "z2", "z3", "z4", "z5", "z6", "z7" }),
        tingpai(Shoupai("m19p19s19z1234567")));
    // �Ŕv�\�Ȏ�v��4������v�͑҂��v�Ƃ��Ȃ�����
    EXPECT_EQ((std::vector<std::string>{
        "m1" }),
        tingpai(Shoupai("m1234444p456s789")));
    // �Í��̔v�͑҂��v�Ƃł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "m2" }),
        tingpai(Shoupai("m13p456s789z11,m2222")));
    // ���Ύq�Ɩʎq��œ���������
    EXPECT_EQ((std::vector<std::string>{
        "m5", "p2", "p6", "p7", "p8", "p9", "s6", "z1", "z7" }),
        tingpai(Shoupai("m11155p2278s66z17")));
    // �������Z�o���[�`�����w��ł��邱��
    EXPECT_EQ((std::vector<std::string>{
        "p7", "p8", "z1", "z7" }),
        tingpai(Shoupai("m11155p2278s66z17"), xiangting_qidui));
}
