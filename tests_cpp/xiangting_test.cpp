#include "pch.h"
#include "../src_cpp/xiangting.h"

#include <fstream>
#include <filesystem>

TEST(XiangtingTest, xiangting_yiban) {
    // ‹ó‚Ìè”v‚Í13Œü’®
    EXPECT_EQ(13, xiangting_yiban(Shoupai()));
    // ’®”vŒ`
    EXPECT_EQ(0, xiangting_yiban(Shoupai("m123p406s789z1122")));
    //˜a—¹Œ`
    EXPECT_EQ(-1, xiangting_yiban(Shoupai("m123p456s789z11222")));
    // •›˜I‚ ‚è
    EXPECT_EQ(-1, xiangting_yiban(Shoupai("m123p456s789z11222")));
    // “ª‚È‚µ
    EXPECT_EQ(1, xiangting_yiban(Shoupai("m12389p456s12789z1")));
    // “‹q‰ß‘½
    EXPECT_EQ(1, xiangting_yiban(Shoupai("m12389p456s1289z11")));
    // “‹q•s‘«
    EXPECT_EQ(2, xiangting_yiban(Shoupai("m133345568z23677")));
    // ­”v: “ª‚È‚µ4–Êq
    EXPECT_EQ(1, xiangting_yiban(Shoupai("p234s567,m222=,p0-67")));
    // q{‡q
    EXPECT_EQ(4, xiangting_yiban(Shoupai("p222345z1234567")));
    // ‡q{ŒÇ—§”v{‡q
    EXPECT_EQ(4, xiangting_yiban(Shoupai("p2344456z123456")));
    // ‘Îq{q{‡q
    EXPECT_EQ(3, xiangting_yiban(Shoupai("p11222345z12345")));
    // ‘Îq{‡q{‡q{‘Îq
    EXPECT_EQ(2, xiangting_yiban(Shoupai("p2234556788z123")));
    // •›˜I’¼Œã‚Ì”vp‚ª˜a—¹Œ`
    EXPECT_EQ(0, xiangting_yiban(Shoupai("m11122,p123-,s12-3,z111=,")));
    // ˆê”Êè: 10000ƒpƒ^[ƒ“
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_1.json");
        nlohmann::json data1;
        ifs >> data1;
        for (const auto& data : data1) {
            EXPECT_EQ(data["x"][0].get<int>(), xiangting_yiban(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // ¬ˆêè: 10000ƒpƒ^[ƒ“
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_2.json");
        nlohmann::json data2;
        ifs >> data2;
        for (const auto& data : data2) {
            EXPECT_EQ(data["x"][0].get<int>(), xiangting_yiban(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // ´ˆêè: 10000ƒpƒ^[ƒ“
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_3.json");
        nlohmann::json data3;
        ifs >> data3;
        for (const auto& data : data3) {
            EXPECT_EQ(data["x"][0].get<int>(), xiangting_yiban(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // ‘mè: 10000ƒpƒ^[ƒ“
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
    // ‹ó‚Ìè”v‚Í13Œü’®
    EXPECT_EQ(13, xiangting_guoshi(Shoupai()));
    // ›ô‹ã”v‚È‚µ
    EXPECT_EQ(13, xiangting_guoshi(Shoupai("m23455p345s45678")));
    // “ª‚È‚µ
    EXPECT_EQ(4, xiangting_guoshi(Shoupai("m189p12s249z12345")));
    // “ª‚ ‚è
    EXPECT_EQ(3, xiangting_guoshi(Shoupai("m119p12s299z12345")));
    // ’®”vŒ`
    EXPECT_EQ(0, xiangting_guoshi(Shoupai("m11p19s19z1234567")));
    // ’®”vŒ`(13–Ê’£)
    EXPECT_EQ(0, xiangting_guoshi(Shoupai("m19p19s19z1234567")));
    // ˜a—¹Œ`
    EXPECT_EQ(-1, xiangting_guoshi(Shoupai("m119p19s19z1234567")));
    // •›˜I‚ ‚è
    EXPECT_EQ(INT_MAX, xiangting_guoshi(Shoupai("m19p19s19z1234,z777=")));
    // ‘½”v
    EXPECT_EQ(-1, xiangting_guoshi(Shoupai("m19p19s19z12345677").zimo("m1", false)));
    // ­”v
    EXPECT_EQ(1, xiangting_guoshi(Shoupai("m119p19s19z12345")));
    // ˆê”Êè: 10000ƒpƒ^[ƒ“
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_1.json");
        nlohmann::json data1;
        ifs >> data1;
        for (const auto& data : data1) {
            EXPECT_EQ(data["x"][1].get<int>(), xiangting_guoshi(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // ¬ˆêè: 10000ƒpƒ^[ƒ“
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_2.json");
        nlohmann::json data2;
        ifs >> data2;
        for (const auto& data : data2) {
            EXPECT_EQ(data["x"][1].get<int>(), xiangting_guoshi(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // ´ˆêè: 10000ƒpƒ^[ƒ“
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_3.json");
        nlohmann::json data3;
        ifs >> data3;
        for (const auto& data : data3) {
            EXPECT_EQ(data["x"][1].get<int>(), xiangting_guoshi(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // ‘mè: 10000ƒpƒ^[ƒ“
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
    // ‹ó‚Ìè”v‚Í13Œü’®
    EXPECT_EQ(13, xiangting_qidui(Shoupai()));
    // ‘Îq‚È‚µ
    EXPECT_EQ(6, xiangting_qidui(Shoupai("m19p19s19z1234567")));
    // Èq‚ ‚è
    EXPECT_EQ(2, xiangting_qidui(Shoupai("m1188p288s05z1111")));
    // ˆÃ‚ ‚è
    EXPECT_EQ(1, xiangting_qidui(Shoupai("m1188p2388s05z111")));
    // ˆÃ2‚Â
    EXPECT_EQ(2, xiangting_qidui(Shoupai("m1188p288s055z111")));
    // ’®”vŒ`
    EXPECT_EQ(0, xiangting_qidui(Shoupai("m1188p288s05z1177")));
    // ˜a—¹Œ`
    EXPECT_EQ(-1, xiangting_qidui(Shoupai("m1188p288s05z1177p2")));
    // •›˜I‚ ‚è
    EXPECT_EQ(INT_MAX, xiangting_qidui(Shoupai("m1188p288s05z2,z111=")));
    // ‘½”v: 8‘Îq
    EXPECT_EQ(-1, xiangting_qidui(Shoupai("m1188p2288s05z1122").zimo("z7", false).zimo("z7", false)));
    // ­”v
    EXPECT_EQ(3, xiangting_qidui(Shoupai("m1188s05z1122")));
    // ˆê”Êè: 10000ƒpƒ^[ƒ“
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_1.json");
        nlohmann::json data1;
        ifs >> data1;
        for (const auto& data : data1) {
            EXPECT_EQ(data["x"][2].get<int>(), xiangting_qidui(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // ¬ˆêè: 10000ƒpƒ^[ƒ“
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_2.json");
        nlohmann::json data2;
        ifs >> data2;
        for (const auto& data : data2) {
            EXPECT_EQ(data["x"][2].get<int>(), xiangting_qidui(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // ´ˆêè: 10000ƒpƒ^[ƒ“
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_3.json");
        nlohmann::json data3;
        ifs >> data3;
        for (const auto& data : data3) {
            EXPECT_EQ(data["x"][2].get<int>(), xiangting_qidui(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // ‘mè: 10000ƒpƒ^[ƒ“
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
    // ˆê”ÊŒ`’®”v
    EXPECT_EQ(0, xiangting(Shoupai("m123p406s789z1122")));
    // ‘m–³‘oŒ`’®”v
    EXPECT_EQ(0, xiangting(Shoupai("m19p19s19z1234567")));
    // µ‘ÎqŒ`’®”v
    EXPECT_EQ(0, xiangting(Shoupai("m1188p288s05z1177")));
    // ˆê”Êè: 10000ƒpƒ^[ƒ“
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_1.json");
        nlohmann::json data1;
        ifs >> data1;
        for (const auto& data : data1) {
            EXPECT_EQ(std::min({ data["x"][0].get<int>(), data["x"][1].get<int>(), data["x"][2].get<int>() }),
                xiangting(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // ¬ˆêè: 10000ƒpƒ^[ƒ“
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_2.json");
        nlohmann::json data2;
        ifs >> data2;
        for (const auto& data : data2) {
            EXPECT_EQ(std::min({ data["x"][0].get<int>(), data["x"][1].get<int>(), data["x"][2].get<int>() }),
                xiangting(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // ´ˆêè: 10000ƒpƒ^[ƒ“
    {
        std::ifstream ifs("../../tests_cpp/data/xiangting_3.json");
        nlohmann::json data3;
        ifs >> data3;
        for (const auto& data : data3) {
            EXPECT_EQ(std::min({ data["x"][0].get<int>(), data["x"][1].get<int>(), data["x"][2].get<int>() }),
                xiangting(Shoupai(data["q"].get<std::vector<std::string>>())));
        }
    }
    // ‘mè: 10000ƒpƒ^[ƒ“
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
    // ‘Å”v‰Â”\‚Èó‘Ô‚Ì‚Æ‚«AƒGƒ‰[‚Æ‚È‚é‚±‚Æ
    EXPECT_THROW(tingpai(Shoupai("m123p456s789z12345")), std::runtime_error);
    EXPECT_THROW(tingpai(Shoupai("m123p456z12345,s789-,")), std::runtime_error);
    // •›˜I‚È‚µ
    EXPECT_EQ((std::vector<std::string>{
        "z1", "z2", "z3", "z4" }),
        tingpai(Shoupai("m123p456s789z1234")));
    // •›˜I‚ ‚è
    EXPECT_EQ((std::vector<std::string>{
        "z1", "z2", "z3", "z4" }),
        tingpai(Shoupai("m123p456z1234,s789-")));
    // ‘m–³‘o13–Ê‘Ò‚¿
    EXPECT_EQ((std::vector<std::string>{
        "m1", "m9", "p1", "p9", "s1", "s9", "z1", "z2", "z3", "z4", "z5", "z6", "z7" }),
        tingpai(Shoupai("m19p19s19z1234567")));
    // ‘Å”v‰Â”\‚Èè”v‚É4–‡‚ ‚é”v‚Í‘Ò‚¿”v‚Æ‚µ‚È‚¢‚±‚Æ
    EXPECT_EQ((std::vector<std::string>{
        "m1" }),
        tingpai(Shoupai("m1234444p456s789")));
    // ˆÃ‚Ì”v‚Í‘Ò‚¿”v‚Æ‚Å‚«‚é‚±‚Æ
    EXPECT_EQ((std::vector<std::string>{
        "m2" }),
        tingpai(Shoupai("m13p456s789z11,m2222")));
    // µ‘Îq‚Æ–Êqè‚Å“¯‚¶Œü’®”
    EXPECT_EQ((std::vector<std::string>{
        "m5", "p2", "p6", "p7", "p8", "p9", "s6", "z1", "z7" }),
        tingpai(Shoupai("m11155p2278s66z17")));
    // Œü’®”Zoƒ‹[ƒ`ƒ“‚ğw’è‚Å‚«‚é‚±‚Æ
    EXPECT_EQ((std::vector<std::string>{
        "p7", "p8", "z1", "z7" }),
        tingpai(Shoupai("m11155p2278s66z17"), xiangting_qidui));
}
