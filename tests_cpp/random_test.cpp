#include "pch.h"
#include "../src_cpp/random.h"

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
