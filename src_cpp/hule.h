#pragma once

#include "shoupai.h"
#include "rule.h"
#include "shan.h"

#include <array>

template<typename T>
void concat(T& m1, T& m2) {
    m1.insert(m1.end(), std::make_move_iterator(m2.begin()), std::make_move_iterator(m2.end()));
}
template<typename T>
void concat(T& m1, const T& m2) {
    m1.insert(m1.end(), m2.begin(), m2.end());
}

struct Param {
    Rule rule;
    int zhuangfeng = 0;
    int menfeng = 1;
    // 胡牌(役)
    struct Hupai {
        int lizhi = 0;
        bool yifa = false;
        bool qianggang = false;
        bool lingshang = false;
        int haidi = 0;
        int tianhu = 0;
    } hupai;
    std::vector<std::string> baopai;
    std::vector<std::string> libaopai;
    struct Jicun {
        int changbang = 0;
        int lizhibang = 0;
    } jicun;
};

struct Hupai {
    enum Name {
        ZHUANGFENG = 0,           // 場風
        ZHUANGFENGDONG = 0,       // 場風 東
        ZHUANGFENGNAN,            // 場風 南
        ZHUANGFENGXI,             // 場風 西
        ZHUANGFENGBEI,            // 場風 北
        MENFENG = 4,              // 自風
        MENFENGDONG = 4,          // 自風 東
        MENFENGNAN,               // 自風 南
        MENFENGXI,                // 自風 西
        MENFENGBEI,               // 自風 北
        FANPAI = 8,               // 翻牌
        FANPAIBAI = 8,            // 翻牌 白
        FANPAIFA,                 // 翻牌 發
        FANPAIZHONG,              // 翻牌 中
        BAOPAI,                   // ドラ
        CHIBAOPAI,                // 赤ドラ
        LIBAOPAI,                 // 裏ドラ
        LIZHI,                    // 立直
        DABULIZHI,                // ダブル立直
        YIPA,                     // 一発
        HAIDIMOYUE,               // 海底摸月
        HEDILAOYU,                // 河底撈魚
        LINGSHANGKAIHUA,          // 嶺上開花
        QIANGGANG,                // 槍槓
        MENQIANQINGZIMOHE,        // 門前清自摸和
        PINGHE,                   // 平和
        DUANYAOJIU,               // 断幺九
        YIBEIKOU,                 // 一盃口
        SANSETONGSHUN,            // 三色同順
        YIQITONGGUAN,             // 一気通貫
        HUNQUANDAIYAOJIU,         // 混全帯幺九
        QIDUIZI,                  // 七対子
        DUIDUIHU,                 // 対々和
        SANANKE,                  // 三暗刻
        SANGANGZI,                // 三槓子
        SANSETONGKE,              // 三色同刻
        HUNLAOTOU,                // 混老頭
        XIAOSANYUAN,              // 小三元
        HUNYISE,                  // 混一色
        CHUNQUANDAIYAOJIU,        // 純全帯幺九
        ERBEIKOU,                 // 二盃口
        QINGYISE,                 // 清一色
        GUOSHIWUSHUANG,           // 国士無双
        GUOSHIWUSHUANGSHISANMIAN, // 国士無双十三面
        SIANKE,                   // 四暗刻
        SIANKEDANQI,              // 四暗刻単騎
        DASANYUAN,                // 大三元
        XIAOSIXI,                 // 小四喜
        DASIXI,                   // 大四喜
        ZIYISE,                   // 字一色
        LVYISE,                   // 緑一色
        QINGLAOTOU,               // 清老頭
        SIGANGZI,                 // 四槓子
        JIULIANBAODENG,           // 九蓮宝燈
        CHUNZHENGJIULIANBAODENG,  // 純正九蓮宝燈
        TIANHE,                   // 天和
        DIHE,                     // 地和
    } name;
    int fanshu;
    char baojia; // 包家(放銃者)

    Hupai(const Name name, const int fanshu, const char baojia) : name{ name }, fanshu{ fanshu }, baojia{ baojia } {}
    Hupai(const Name name, const int fanshu) : name{ name }, fanshu{ fanshu }, baojia{} {}
    bool operator==(const Hupai& other) const {
        return name == other.name &&
            fanshu == other.fanshu &&
            baojia == other.baojia;
    }
};

// 得分(得点)
struct Defen {
    std::vector<Hupai> hupai;
    int fu;
    int fanshu;
    int damanguan;
    int defen;
    std::array<int, 4> fenpei;
    int menfeng;

    bool operator==(const Defen& other) const {
        return hupai == other.hupai &&
            fu == other.fu &&
            fanshu == other.fanshu &&
            damanguan == other.damanguan &&
            defen == other.defen &&
            fenpei == other.fenpei;
    }
};

std::vector<std::vector<std::string>> hule_mianzi_yiban(Shoupai& shoupai, const std::string& hulepai);
std::vector<std::vector<std::string>> hule_mianzi(const Shoupai& shoupai, const std::string& rongpai = {});
Defen hule(const Shoupai& shoupai, std::string rongpai = {}, const Param& param = {});
