#include "pch.h"
#include "../src_cpp/xiangting.h"

#include <fstream>
#include <filesystem>

TEST(XiangtingTest, xiangting_yiban) {
    // 空の手牌は13向聴
    EXPECT_EQ(13, xiangting_yiban(Shoupai()));
    // 聴牌形
    EXPECT_EQ(0, xiangting_yiban(Shoupai("m123p406s789z1122")));
    //和了形
    EXPECT_EQ(-1, xiangting_yiban(Shoupai("m123p456s789z11222")));
    // 副露あり
    EXPECT_EQ(-1, xiangting_yiban(Shoupai("m123p456s789z11222")));
    // 雀頭なし
    EXPECT_EQ(1, xiangting_yiban(Shoupai("m12389p456s12789z1")));
    // 搭子過多
    EXPECT_EQ(1, xiangting_yiban(Shoupai("m12389p456s1289z11")));
    // 搭子不足
    EXPECT_EQ(2, xiangting_yiban(Shoupai("m133345568z23677")));
    // 少牌: 雀頭なし4面子
    EXPECT_EQ(1, xiangting_yiban(Shoupai("p234s567,m222=,p0-67")));
    // 刻子＋順子
    EXPECT_EQ(4, xiangting_yiban(Shoupai("p222345z1234567")));
    // 順子＋孤立牌＋順子
    EXPECT_EQ(4, xiangting_yiban(Shoupai("p2344456z123456")));
    // 対子＋刻子＋順子
    EXPECT_EQ(3, xiangting_yiban(Shoupai("p11222345z12345")));
    // 対子＋順子＋順子＋対子
    EXPECT_EQ(2, xiangting_yiban(Shoupai("p2234556788z123")));
    // 副露直後の牌姿が和了形
    EXPECT_EQ(0, xiangting_yiban(Shoupai("m11122,p123-,s12-3,z111=,")));
}

TEST(XiangtingTest, xiangting_yiban_json) {
    // 一般手: 10000パターン
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_1.json");
        nlohmann::json data1;
        ifs >> data1;
        for (const auto& data : data1) {
            EXPECT_EQ(data["x"][0].get<int>(), xiangting_yiban(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // 混一手: 10000パターン
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_2.json");
        nlohmann::json data2;
        ifs >> data2;
        for (const auto& data : data2) {
            EXPECT_EQ(data["x"][0].get<int>(), xiangting_yiban(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // 清一手: 10000パターン
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_3.json");
        nlohmann::json data3;
        ifs >> data3;
        for (const auto& data : data3) {
            EXPECT_EQ(data["x"][0].get<int>(), xiangting_yiban(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // 国士手: 10000パターン
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
    // 空の手牌は13向聴
    EXPECT_EQ(13, xiangting_guoshi(Shoupai()));
    // 幺九牌なし
    EXPECT_EQ(13, xiangting_guoshi(Shoupai("m23455p345s45678")));
    // 雀頭なし
    EXPECT_EQ(4, xiangting_guoshi(Shoupai("m189p12s249z12345")));
    // 雀頭あり
    EXPECT_EQ(3, xiangting_guoshi(Shoupai("m119p12s299z12345")));
    // 聴牌形
    EXPECT_EQ(0, xiangting_guoshi(Shoupai("m11p19s19z1234567")));
    // 聴牌形(13面張)
    EXPECT_EQ(0, xiangting_guoshi(Shoupai("m19p19s19z1234567")));
    // 和了形
    EXPECT_EQ(-1, xiangting_guoshi(Shoupai("m119p19s19z1234567")));
    // 副露あり
    EXPECT_EQ(INT_MAX, xiangting_guoshi(Shoupai("m19p19s19z1234,z777=")));
    // 多牌
    EXPECT_EQ(-1, xiangting_guoshi(Shoupai("m19p19s19z12345677").zimo("m1", false)));
    // 少牌
    EXPECT_EQ(1, xiangting_guoshi(Shoupai("m119p19s19z12345")));
}

TEST(XiangtingTest, xiangting_guoshi_json) {
    // 一般手: 10000パターン
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_1.json");
        nlohmann::json data1;
        ifs >> data1;
        for (const auto& data : data1) {
            EXPECT_EQ(data["x"][1].get<int>(), xiangting_guoshi(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // 混一手: 10000パターン
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_2.json");
        nlohmann::json data2;
        ifs >> data2;
        for (const auto& data : data2) {
            EXPECT_EQ(data["x"][1].get<int>(), xiangting_guoshi(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // 清一手: 10000パターン
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_3.json");
        nlohmann::json data3;
        ifs >> data3;
        for (const auto& data : data3) {
            EXPECT_EQ(data["x"][1].get<int>(), xiangting_guoshi(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // 国士手: 10000パターン
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
    // 空の手牌は13向聴
    EXPECT_EQ(13, xiangting_qidui(Shoupai()));
    // 対子なし
    EXPECT_EQ(6, xiangting_qidui(Shoupai("m19p19s19z1234567")));
    // 槓子あり
    EXPECT_EQ(2, xiangting_qidui(Shoupai("m1188p288s05z1111")));
    // 暗刻あり
    EXPECT_EQ(1, xiangting_qidui(Shoupai("m1188p2388s05z111")));
    // 暗刻2つ
    EXPECT_EQ(2, xiangting_qidui(Shoupai("m1188p288s055z111")));
    // 聴牌形
    EXPECT_EQ(0, xiangting_qidui(Shoupai("m1188p288s05z1177")));
    // 和了形
    EXPECT_EQ(-1, xiangting_qidui(Shoupai("m1188p288s05z1177p2")));
    // 副露あり
    EXPECT_EQ(INT_MAX, xiangting_qidui(Shoupai("m1188p288s05z2,z111=")));
    // 多牌: 8対子
    EXPECT_EQ(-1, xiangting_qidui(Shoupai("m1188p2288s05z1122").zimo("z7", false).zimo("z7", false)));
    // 少牌
    EXPECT_EQ(3, xiangting_qidui(Shoupai("m1188s05z1122")));
}

TEST(XiangtingTest, xiangting_qidui_json) {
    // 一般手: 10000パターン
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_1.json");
        nlohmann::json data1;
        ifs >> data1;
        for (const auto& data : data1) {
            EXPECT_EQ(data["x"][2].get<int>(), xiangting_qidui(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // 混一手: 10000パターン
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_2.json");
        nlohmann::json data2;
        ifs >> data2;
        for (const auto& data : data2) {
            EXPECT_EQ(data["x"][2].get<int>(), xiangting_qidui(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // 清一手: 10000パターン
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_3.json");
        nlohmann::json data3;
        ifs >> data3;
        for (const auto& data : data3) {
            EXPECT_EQ(data["x"][2].get<int>(), xiangting_qidui(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // 国士手: 10000パターン
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
    // 一般形聴牌
    EXPECT_EQ(0, xiangting(Shoupai("m123p406s789z1122")));
    // 国士無双形聴牌
    EXPECT_EQ(0, xiangting(Shoupai("m19p19s19z1234567")));
    // 七対子形聴牌
    EXPECT_EQ(0, xiangting(Shoupai("m1188p288s05z1177")));
}

TEST(XiangtingTest, xiangting_json) {
    // 一般手: 10000パターン
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_1.json");
        nlohmann::json data1;
        ifs >> data1;
        for (const auto& data : data1) {
            EXPECT_EQ(std::min({ data["x"][0].get<int>(), data["x"][1].get<int>(), data["x"][2].get<int>() }),
                xiangting(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // 混一手: 10000パターン
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_2.json");
        nlohmann::json data2;
        ifs >> data2;
        for (const auto& data : data2) {
            EXPECT_EQ(std::min({ data["x"][0].get<int>(), data["x"][1].get<int>(), data["x"][2].get<int>() }),
                xiangting(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // 清一手: 10000パターン
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_3.json");
        nlohmann::json data3;
        ifs >> data3;
        for (const auto& data : data3) {
            EXPECT_EQ(std::min({ data["x"][0].get<int>(), data["x"][1].get<int>(), data["x"][2].get<int>() }),
                xiangting(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // 国士手: 10000パターン
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
    // 打牌可能な状態のとき、エラーとなること
    EXPECT_THROW(tingpai(Shoupai("m123p456s789z12345")), std::runtime_error);
    EXPECT_THROW(tingpai(Shoupai("m123p456z12345,s789-,")), std::runtime_error);
    // 副露なし
    EXPECT_EQ((std::vector<std::string>{
        "z1", "z2", "z3", "z4" }),
        tingpai(Shoupai("m123p456s789z1234")));
    // 副露あり
    EXPECT_EQ((std::vector<std::string>{
        "z1", "z2", "z3", "z4" }),
        tingpai(Shoupai("m123p456z1234,s789-")));
    // 国士無双13面待ち
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m9", "p1", "p9", "s1", "s9", "z1", "z2", "z3", "z4", "z5", "z6", "z7" }),
        tingpai(Shoupai("m19p19s19z1234567")));
    // 打牌可能な手牌に4枚ある牌は待ち牌としないこと
    EXPECT_EQ((std::vector<std::string>{
        "m1" }),
        tingpai(Shoupai("m1234444p456s789")));
    // 暗刻の牌は待ち牌とできること
    EXPECT_EQ((std::vector<std::string>{
        "m2" }),
        tingpai(Shoupai("m13p456s789z11,m2222")));
    // 七対子と面子手で同じ向聴数
    EXPECT_EQ((std::vector<std::string>{
        "m5", "p2", "p6", "p7", "p8", "p9", "s6", "z1", "z7" }),
        tingpai(Shoupai("m11155p2278s66z17")));
    // 向聴数算出ルーチンを指定できること
    EXPECT_EQ((std::vector<std::string>{
        "p7", "p8", "z1", "z7" }),
        tingpai(Shoupai("m11155p2278s66z17"), xiangting_qidui));
}
