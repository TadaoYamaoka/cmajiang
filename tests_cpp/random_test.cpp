#include "pch.h"
#include "../src_cpp/random.h"
#include "../src_cpp/xiangting.h"

std::map<std::string, int> init_rest(const Rule& rule) {
    std::map<std::string, int> rest{
        { "m1", 4 }, { "m2", 4 }, { "m3", 4 }, { "m4", 4 }, { "m5", 4 }, { "m6", 4 }, { "m7", 4 }, { "m8", 4 }, { "m9", 4 },
        { "p1", 4 }, { "p2", 4 }, { "p3", 4 }, { "p4", 4 }, { "p5", 4 }, { "p6", 4 }, { "p7", 4 }, { "p8", 4 }, { "p9", 4 },
        { "s1", 4 }, { "s2", 4 }, { "s3", 4 }, { "s4", 4 }, { "s5", 4 }, { "s6", 4 }, { "s7", 4 }, { "s8", 4 }, { "s9", 4 },
        { "z1", 4 }, { "z2", 4 }, { "z3", 4 }, { "z4", 4 }, { "z5", 4 }, { "z6", 4 }, { "z7", 4 }
    };
    for (const auto s : { 'm', 'p', 's' }) {
        for (int i = 0; i < rule.hongpai_(s); i++) {
            rest[to_string(s, 5)]--;
            rest[to_string(s, 0)]++;
        }
    }
    return rest;
}

std::map<std::string, int> count_pai(const std::vector<std::string>& pai, const std::vector<std::string>& fulou, const std::map<std::string, int>& rest) {
    std::map<std::string, int> count;

    for (const auto& p : pai)
        count[p]++;

    for (const auto& m : fulou) {
        for (std::sregex_iterator it(m.begin() + 1, m.end(), re_digit), end; it != end; ++it) {
            const auto p = m[0] + it->str();
            count[p]++;
        }
    }

    for (auto itr = rest.begin(); itr != rest.end(); ++itr) {
        count[itr->first] += itr->second;
    }

    return count;
}


// 場風
TEST(RandomTest, setup_zhuangfeng) {
    const Rule rule;
    {
        // 東
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        Param param;
        param.zhuangfeng = 0;
        std::mt19937_64 mt{ 0 };

        setup_zhuangfeng(pai, rest, fulou, param.zhuangfeng, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai, {}, param);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::ZHUANGFENGDONG; }));
    }
    {
        // 南
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        Param param;
        param.zhuangfeng = 1;
        std::mt19937_64 mt{ 0 };

        setup_zhuangfeng(pai, rest, fulou, param.zhuangfeng, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai, {}, param);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::ZHUANGFENGNAN; }));
    }

}

// 自風
TEST(RandomTest, setup_menfeng) {
    const Rule rule;
    {
        // 東
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        Param param;
        param.menfeng = 0;
        std::mt19937_64 mt{ 0 };

        setup_menfeng(pai, rest, fulou, param.menfeng, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai, {}, param);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::MENFENGDONG; }));
    }
    {
        // 西
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        Param param;
        param.menfeng = 2;
        std::mt19937_64 mt{ 0 };

        setup_menfeng(pai, rest, fulou, param.menfeng, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai, {}, param);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::MENFENGXI; }));
    }

}

// 翻牌
TEST(RandomTest, setup_fanpai) {
    const Rule rule;
    {
        // 白
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_fanpai(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::FANPAIBAI; }));
    }
    {
        // 發
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 4 };

        setup_fanpai(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::FANPAIZHONG; }));
    }
    {
        // 中
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 2 };

        setup_fanpai(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::FANPAIZHONG; }));
    }
}

// 平和
TEST(RandomTest, setup_pinghe) {
    const Rule rule;
    {
        // 赤牌なし
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        Param param;
        param.zhuangfeng = 0;
        param.menfeng = 1;
        std::mt19937_64 mt{ 8 };

        setup_pinghe(pai, rest, param.zhuangfeng, param.menfeng, rule, mt);
        std::swap(pai[0], pai.back());
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai, {}, param);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::PINGHE; }));
        EXPECT_FALSE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::CHIBAOPAI; }));
    }
    {
        // 赤牌
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        Param param;
        param.zhuangfeng = 0;
        param.menfeng = 1;
        std::mt19937_64 mt{ 1 };

        setup_pinghe(pai, rest, param.zhuangfeng, param.menfeng, rule, mt);
        std::swap(pai[0], pai.back());
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai, {}, param);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::PINGHE; }));
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::CHIBAOPAI; }));
    }
    {
        // 雀頭が赤牌
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        Param param;
        param.zhuangfeng = 0;
        param.menfeng = 1;
        std::mt19937_64 mt{ 85 };

        setup_pinghe(pai, rest, param.zhuangfeng, param.menfeng, rule, mt);
        std::swap(pai[0], pai.back());
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai, {}, param);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::PINGHE; }));
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::CHIBAOPAI; }));
    }
}

// 断幺九
TEST(RandomTest, setup_duanyaojiu) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_duanyaojiu(pai, rest, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::DUANYAOJIU; }));
    }
}

// 一盃口
TEST(RandomTest, setup_yibeikou) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_yibeikou(pai, rest, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::YIBEIKOU; }));
        EXPECT_FALSE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::CHIBAOPAI; }));
    }
    {
        // 赤牌
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 1 };

        setup_yibeikou(pai, rest, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::YIBEIKOU; }));
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::CHIBAOPAI; }));
    }
}

// 三色同順
TEST(RandomTest, setup_sansetongshun) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_sansetongshun(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::SANSETONGSHUN; }));
        EXPECT_FALSE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::CHIBAOPAI; }));
    }
    {
        // 赤牌
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 14 };

        setup_sansetongshun(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::SANSETONGSHUN; }));
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::CHIBAOPAI; }));
    }
}

// 一気通貫
TEST(RandomTest, setup_yiqitongguan) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_yiqitongguan(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::YIQITONGGUAN; }));
        EXPECT_FALSE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::CHIBAOPAI; }));
    }
    {
        // 赤牌
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 1 };

        setup_yiqitongguan(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::YIQITONGGUAN; }));
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::CHIBAOPAI; }));
    }
}

// 混全帯幺九
TEST(RandomTest, setup_hunquandaiyaojiu) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_hunquandaiyaojiu(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::HUNQUANDAIYAOJIU; }));
    }
}

// 七対子
TEST(RandomTest, setup_qiduizi) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_qiduizi(pai, rest, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::QIDUIZI; }));
    }
}

// 対々和
TEST(RandomTest, setup_duiduihu) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_duiduihu(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::DUIDUIHU; }));
        EXPECT_FALSE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::CHIBAOPAI; }));
    }
    {
        // 赤牌
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 1 };

        setup_duiduihu(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::DUIDUIHU; }));
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::CHIBAOPAI; }));
    }
    {
        // 槓子
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 101 };

        setup_duiduihu(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::DUIDUIHU; }));
        EXPECT_TRUE(find_if(fulou, [](const auto& m) { return std::regex_match(m, re_gang); }));
    }
}

// 三暗刻
TEST(RandomTest, setup_sananke) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_sananke(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::SANANKE; }));
    }
    {
        // 副露
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 1 };

        setup_sananke(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::SANANKE; }));
        EXPECT_EQ(1, fulou.size());
    }
}

// 三槓子
TEST(RandomTest, setup_sangangzi) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_sangangzi(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::SANGANGZI; }));
    }
    {
        // 暗槓
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 2 };

        setup_sangangzi(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::SANGANGZI; }));
        EXPECT_TRUE(find_if(fulou, [](const auto& m) { return std::regex_match(m, re_angang); }));
    }
    {
        // 4枚目が赤牌
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 50 };

        setup_sangangzi(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::SANGANGZI; }));
        EXPECT_TRUE(find_if(fulou, [](const auto& m) { return std::regex_match(m, re_gang) && (m.back() == '0' || m.back() != '5' && m[4] == '0'); }));
    }
}

// 三色同刻
TEST(RandomTest, setup_sansetongke) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_sansetongke(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::SANSETONGKE; }));
        EXPECT_FALSE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::CHIBAOPAI; }));
    }
    {
        // 赤牌
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 88 };

        setup_sansetongke(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::SANSETONGKE; }));
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::CHIBAOPAI; }));
    }
}

// 混老頭
TEST(RandomTest, setup_hunlaotou) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_hunlaotou(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::HUNLAOTOU; }));
    }
}

// 小三元
TEST(RandomTest, setup_xiaosanyuan) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_xiaosanyuan(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::XIAOSANYUAN; }));
    }
}

// 混一色
TEST(RandomTest, setup_hunyise) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_hunyise(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::HUNYISE; }));
    }
}

// 純全帯幺九
TEST(RandomTest, setup_chunquandaiyaojiu) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_chunquandaiyaojiu(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::CHUNQUANDAIYAOJIU; }));
    }
}

// 二盃口
TEST(RandomTest, setup_erbeikou) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_erbeikou(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::ERBEIKOU; }));
    }
}

// 清一色
TEST(RandomTest, setup_qingyise) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 2 };

        setup_qingyise(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::QINGYISE; }));
        EXPECT_FALSE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::CHIBAOPAI; }));
    }
    {
        // 赤牌
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_qingyise(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::QINGYISE; }));
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::CHIBAOPAI; }));
    }
}

// 国士無双
TEST(RandomTest, setup_guoshiwushuang) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_guoshiwushuang(pai, rest, rule, mt);
        std::swap(pai[0], pai.back());
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::GUOSHIWUSHUANG; }));
    }
    {
        // 国士無双十三面
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 1 };

        setup_guoshiwushuang(pai, rest, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::GUOSHIWUSHUANGSHISANMIAN; }));
    }
}

// 四暗刻
TEST(RandomTest, setup_sianke) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_sianke(pai, rest, rule, mt);
        std::swap(pai[0], pai.back());
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::SIANKE; }));
    }
    {
        // 四暗刻単騎
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 1 };

        setup_sianke(pai, rest, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::SIANKEDANQI; }));
    }
}

// 大三元
TEST(RandomTest, setup_dasanyuan) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_dasanyuan(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::DASANYUAN; }));
    }
}

// 小四喜
TEST(RandomTest, setup_xiaosixi) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_xiaosixi(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::XIAOSIXI; }));
    }
}

// 大四喜
TEST(RandomTest, setup_dasixi) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_dasixi(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::DASIXI; }));
    }
}

// 字一色
TEST(RandomTest, setup_ziyise) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_ziyise(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::ZIYISE; }));
    }
}

// 緑一色
TEST(RandomTest, setup_lvyise) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_lvyise(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::LVYISE; }));
    }
}

// 清老頭
TEST(RandomTest, setup_qinglaotou) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_qinglaotou(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::QINGLAOTOU; }));
    }
}

// 四槓子
TEST(RandomTest, setup_sigangzi) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_sigangzi(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::SIGANGZI; }));
    }
    {
        // 赤牌
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 1 };

        setup_sigangzi(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::SIGANGZI; }));
        EXPECT_TRUE(find_if(fulou, [](const auto& m) { return std::regex_search(m, re_ling); }));
    }
    {
        // 4枚目が赤牌
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 12 };

        setup_sigangzi(pai, rest, fulou, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::SIGANGZI; }));
        EXPECT_TRUE(find_if(fulou, [](const auto& m) { return m.back() == '0' || m.back() != '5' && m[4] == '0'; }));
    }
}

// 九蓮宝燈
TEST(RandomTest, setup_jiulianbaodeng) {
    const Rule rule;
    {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_jiulianbaodeng(pai, rest, rule, mt);
        std::swap(pai[0], pai.back());
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::JIULIANBAODENG; }));
    }
    {
        // 純正九蓮宝燈
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 1 };

        setup_jiulianbaodeng(pai, rest, rule, mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        EXPECT_TRUE(find_if(defen.hupai, [](const auto& h) { return h.name == Hupai::CHUNZHENGJIULIANBAODENG; }));
    }
}

TEST(RandomTest, make_hule) {
    const Rule rule;
    Param param;
    param.zhuangfeng = 0;
    param.menfeng = 1;
    std::mt19937_64 mt{ 0 };

    std::map<Hupai::Name, int> hupai_count;
    for (int i = 0; i < 1000; i++) {
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        make_hule(pai, rest, fulou, param.zhuangfeng, param.menfeng, rule, mt);
        std::shuffle(pai.begin(), pai.end(), mt);
        Shoupai shoupai{ pai, fulou };
        Defen defen = hule(shoupai);
        for (const auto& h : defen.hupai)
            hupai_count[h.name]++;
    }
    EXPECT_GT(hupai_count[Hupai::ZHUANGFENGDONG], 20);
    EXPECT_GT(hupai_count[Hupai::MENFENGNAN], 20);
    EXPECT_GT(hupai_count[Hupai::FANPAIZHONG], 20);
    EXPECT_GT(hupai_count[Hupai::PINGHE], 20);
    EXPECT_GT(hupai_count[Hupai::CHIBAOPAI], 20);
    EXPECT_GT(hupai_count[Hupai::DUANYAOJIU], 20);
    EXPECT_GT(hupai_count[Hupai::YIBEIKOU], 20);
    EXPECT_GT(hupai_count[Hupai::SANSETONGSHUN], 20);
    EXPECT_GT(hupai_count[Hupai::YIQITONGGUAN], 20);
    EXPECT_GT(hupai_count[Hupai::HUNQUANDAIYAOJIU], 20);
    EXPECT_GT(hupai_count[Hupai::QIDUIZI], 20);
    EXPECT_GT(hupai_count[Hupai::DUIDUIHU], 20);
    EXPECT_GT(hupai_count[Hupai::SANANKE], 20);
    EXPECT_GT(hupai_count[Hupai::SANGANGZI], 20);
    EXPECT_GT(hupai_count[Hupai::SANSETONGKE], 20);
    EXPECT_GT(hupai_count[Hupai::HUNLAOTOU], 20);
    EXPECT_GT(hupai_count[Hupai::XIAOSANYUAN], 20);
    EXPECT_GT(hupai_count[Hupai::HUNYISE], 20);
    EXPECT_GT(hupai_count[Hupai::CHUNQUANDAIYAOJIU], 20);
    EXPECT_GT(hupai_count[Hupai::ERBEIKOU], 20);
    EXPECT_GT(hupai_count[Hupai::QINGYISE], 20);
    EXPECT_GT(hupai_count[Hupai::GUOSHIWUSHUANG], 20);
    EXPECT_GT(hupai_count[Hupai::GUOSHIWUSHUANGSHISANMIAN], 2);
    EXPECT_GT(hupai_count[Hupai::SIANKE], 20);
    EXPECT_GT(hupai_count[Hupai::SIANKEDANQI], 4);
    EXPECT_GT(hupai_count[Hupai::DASANYUAN], 20);
    EXPECT_GT(hupai_count[Hupai::XIAOSIXI], 20);
    EXPECT_GT(hupai_count[Hupai::DASIXI], 20);
    EXPECT_GT(hupai_count[Hupai::ZIYISE], 20);
    EXPECT_GT(hupai_count[Hupai::LVYISE], 16);
    EXPECT_GT(hupai_count[Hupai::QINGLAOTOU], 20);
    EXPECT_GT(hupai_count[Hupai::SIGANGZI], 20);
    EXPECT_GT(hupai_count[Hupai::JIULIANBAODENG], 13);
    EXPECT_GT(hupai_count[Hupai::CHUNZHENGJIULIANBAODENG], 3);
}

TEST(RandomTest, beta_distribution) {
    constexpr int N = 10000;
    {
        int n_xiangting = 1;
        std::mt19937_64 mt{ 0 };
        BetaDistribution xiangting_dist{ n_xiangting, 12 };
        int sum = 0;
        int min = INT_MAX;
        int max = 0;
        for (int i = 0; i < N; i++) {
            int n = xiangting_dist(mt);
            min = std::min(n, min);
            max = std::max(n, max);
            sum += n;
        }
        EXPECT_EQ(0, min);
        EXPECT_GT(max, 6);
        EXPECT_EQ(n_xiangting - 1, sum / N);
    }
    {
        int n_xiangting = 2;
        std::mt19937_64 mt{ 0 };
        BetaDistribution xiangting_dist{ n_xiangting, 12 };
        int sum = 0;
        int min = INT_MAX;
        int max = 0;
        for (int i = 0; i < N; i++) {
            int n = xiangting_dist(mt);
            min = std::min(n, min);
            max = std::max(n, max);
            sum += n;
        }
        EXPECT_EQ(0, min);
        EXPECT_GT(max, 7);
        EXPECT_EQ(n_xiangting - 1, sum / N);
    }
    {
        int n_xiangting = 3;
        std::mt19937_64 mt{ 0 };
        BetaDistribution xiangting_dist{ n_xiangting, 12 };
        int sum = 0;
        int min = INT_MAX;
        int max = 0;
        for (int i = 0; i < N; i++) {
            int n = xiangting_dist(mt);
            min = std::min(n, min);
            max = std::max(n, max);
            sum += n;
        }
        EXPECT_EQ(0, min);
        EXPECT_GT(max, 8);
        EXPECT_EQ(n_xiangting - 1, sum / N);
    }
    {
        int n_xiangting = 10;
        std::mt19937_64 mt{ 0 };
        BetaDistribution xiangting_dist{ n_xiangting, 12 };
        int sum = 0;
        int min = INT_MAX;
        int max = 0;
        for (int i = 0; i < N; i++) {
            int n = xiangting_dist(mt);
            min = std::min(n, min);
            max = std::max(n, max);
            sum += n;
        }
        EXPECT_LT(min, 4);
        EXPECT_EQ(11, max);
        EXPECT_EQ(n_xiangting - 1, sum / N);
    }
    {
        int n_xiangting = 11;
        std::mt19937_64 mt{ 0 };
        BetaDistribution xiangting_dist{ n_xiangting, 12 };
        int sum = 0;
        int min = INT_MAX;
        int max = 0;
        for (int i = 0; i < N; i++) {
            int n = xiangting_dist(mt);
            min = std::min(n, min);
            max = std::max(n, max);
            sum += n;
        }
        EXPECT_LT(min, 6);
        EXPECT_EQ(11, max);
        EXPECT_EQ(n_xiangting - 1, sum / N);
    }
}

TEST(RandomTest, discard_one) {
    const Rule rule;
    {
        // 副露なし
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };

        setup_duanyaojiu(pai, rest, rule, mt);
        discard_one(pai, rest, fulou, mt);
        Shoupai shoupai{ pai, fulou };
        EXPECT_EQ(0, xiangting(shoupai));
        EXPECT_EQ(init_rest(rule), count_pai(pai, fulou, rest));
    }
    {
        // 副露あり
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 2 };

        setup_duiduihu(pai, rest, fulou, rule, mt);
        EXPECT_EQ(1, fulou.size());
        discard_one(pai, rest, fulou, mt);
        Shoupai shoupai{ pai, fulou };
        EXPECT_EQ(0, fulou.size());
        EXPECT_EQ(0, xiangting(shoupai));
        EXPECT_EQ(init_rest(rule), count_pai(pai, fulou, rest));
    }
    {
        // 槓あり
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 4 };

        setup_sangangzi(pai, rest, fulou, rule, mt);
        EXPECT_EQ(3, fulou.size());
        discard_one(pai, rest, fulou, mt);
        Shoupai shoupai{ pai, fulou };
        EXPECT_EQ(2, fulou.size());
        EXPECT_EQ(0, xiangting(shoupai));
        EXPECT_EQ(init_rest(rule), count_pai(pai, fulou, rest));
    }
}

TEST(RandomTest, make_n_xiangting) {
    const Rule rule;
    {
        // 副露なし
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 0 };
        BetaDistribution xiangting_dist{ 3, 12 };

        setup_duanyaojiu(pai, rest, rule, mt);
        discard_one(pai, rest, fulou, mt);
        make_n_xiangting(pai, rest, fulou, xiangting_dist, mt);
        Shoupai shoupai{ pai, fulou };
        EXPECT_EQ(2, xiangting(shoupai));
        EXPECT_EQ(init_rest(rule), count_pai(pai, fulou, rest));
    }
    {
        // 副露あり
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 1 };
        BetaDistribution xiangting_dist{ 4, 12 };

        setup_duiduihu(pai, rest, fulou, rule, mt);
        discard_one(pai, rest, fulou, mt);
        EXPECT_EQ(2, fulou.size());
        make_n_xiangting(pai, rest, fulou, xiangting_dist, mt);
        Shoupai shoupai{ pai, fulou };
        EXPECT_EQ(0, fulou.size());
        EXPECT_EQ(2, xiangting(shoupai));
        EXPECT_EQ(init_rest(rule), count_pai(pai, fulou, rest));
    }
    {
        // 槓あり
        std::vector<std::string> pai;
        std::vector<std::string> fulou;
        std::map<std::string, int> rest = init_rest(rule);
        std::mt19937_64 mt{ 4 };
        BetaDistribution xiangting_dist{ 7, 12 };

        setup_sangangzi(pai, rest, fulou, rule, mt);
        discard_one(pai, rest, fulou, mt);
        EXPECT_EQ(2, fulou.size());
        make_n_xiangting(pai, rest, fulou, xiangting_dist, mt);
        Shoupai shoupai{ pai, fulou };
        EXPECT_EQ(1, fulou.size());
        EXPECT_EQ(1, xiangting(shoupai));
        EXPECT_EQ(init_rest(rule), count_pai(pai, fulou, rest));
    }
}

TEST(RandomTest, random_game_state) {
    Rule rule;
    {
        int n_xiangting = 1;
        int zhuangfeng = 0;
        std::mt19937_64 mt{ 0 };
        Game game = random_game_state(n_xiangting, zhuangfeng, rule, mt);
        EXPECT_EQ(Game::Status::DAPAI, game.status());
        EXPECT_EQ(3, game.lunban());
        EXPECT_EQ(1, xiangting(game.shoupai_(0)));
        EXPECT_EQ(0, xiangting(game.shoupai_(1)));
        EXPECT_EQ(1, xiangting(game.shoupai_(2)));
        EXPECT_EQ(0, xiangting(game.shoupai_(3)));
        EXPECT_FALSE(game.shoupai_(0).lizhi());
        EXPECT_FALSE(game.shoupai_(1).lizhi());
        EXPECT_FALSE(game.shoupai_(2).lizhi());
        EXPECT_TRUE(game.shoupai_(3).lizhi());
        EXPECT_EQ(16, game.he_(0).pai().size());
        EXPECT_EQ(16, game.he_(1).pai().size());
        EXPECT_EQ(15, game.he_(2).pai().size());
        EXPECT_EQ(15, game.he_(3).pai().size());
        EXPECT_EQ(11, game.shan().paishu());
        EXPECT_NO_THROW(game.zimo());
    }
    {
        int n_xiangting = 3;
        int zhuangfeng = 0;
        std::mt19937_64 mt{ 1 };
        Game game = random_game_state(n_xiangting, zhuangfeng, rule, mt);
        EXPECT_EQ(Game::Status::DAPAI, game.status());
        EXPECT_EQ(3, game.lunban());
        EXPECT_EQ(0, xiangting(game.shoupai_(0)));
        EXPECT_EQ(2, xiangting(game.shoupai_(1)));
        EXPECT_EQ(1, xiangting(game.shoupai_(2)));
        EXPECT_EQ(2, xiangting(game.shoupai_(3)));
        EXPECT_FALSE(game.shoupai_(0).lizhi());
        EXPECT_FALSE(game.shoupai_(1).lizhi());
        EXPECT_FALSE(game.shoupai_(2).lizhi());
        EXPECT_FALSE(game.shoupai_(3).lizhi());
        EXPECT_EQ(13, game.he_(0).pai().size());
        EXPECT_EQ(13, game.he_(1).pai().size());
        EXPECT_EQ(13, game.he_(2).pai().size());
        EXPECT_EQ(13, game.he_(3).pai().size());
        EXPECT_EQ(23, game.shan().paishu());
        EXPECT_NO_THROW(game.zimo());
    }
    {
        int n_xiangting = 5;
        int zhuangfeng = 0;
        std::mt19937_64 mt{ 2 };
        Game game = random_game_state(n_xiangting, zhuangfeng, rule, mt);
        EXPECT_EQ(Game::Status::DAPAI, game.status());
        EXPECT_EQ(2, game.lunban());
        EXPECT_EQ(3, xiangting(game.shoupai_(0)));
        EXPECT_EQ(2, xiangting(game.shoupai_(1)));
        EXPECT_EQ(3, xiangting(game.shoupai_(2)));
        EXPECT_EQ(4, xiangting(game.shoupai_(3)));
        EXPECT_FALSE(game.shoupai_(0).lizhi());
        EXPECT_FALSE(game.shoupai_(1).lizhi());
        EXPECT_FALSE(game.shoupai_(2).lizhi());
        EXPECT_FALSE(game.shoupai_(3).lizhi());
        EXPECT_EQ(5, game.he_(0).pai().size());
        EXPECT_EQ(5, game.he_(1).pai().size());
        EXPECT_EQ(5, game.he_(2).pai().size());
        EXPECT_EQ(4, game.he_(3).pai().size());
        EXPECT_EQ(51, game.shan().paishu());
        EXPECT_NO_THROW(game.zimo());
    }
    {
        int n_xiangting = 11;
        int zhuangfeng = 0;
        std::mt19937_64 mt{ 3 };
        Game game = random_game_state(n_xiangting, zhuangfeng, rule, mt);
        EXPECT_EQ(Game::Status::DAPAI, game.status());
        EXPECT_EQ(1, game.lunban());
        EXPECT_EQ(3, xiangting(game.shoupai_(0)));
        EXPECT_EQ(5, xiangting(game.shoupai_(1)));
        EXPECT_EQ(3, xiangting(game.shoupai_(2)));
        EXPECT_EQ(3, xiangting(game.shoupai_(3)));
        EXPECT_FALSE(game.shoupai_(0).lizhi());
        EXPECT_FALSE(game.shoupai_(1).lizhi());
        EXPECT_FALSE(game.shoupai_(2).lizhi());
        EXPECT_FALSE(game.shoupai_(3).lizhi());
        EXPECT_EQ(2, game.he_(0).pai().size());
        EXPECT_EQ(2, game.he_(1).pai().size());
        EXPECT_EQ(1, game.he_(2).pai().size());
        EXPECT_EQ(1, game.he_(3).pai().size());
        EXPECT_EQ(63, game.shan().paishu());
        EXPECT_NO_THROW(game.zimo());
    }
}
