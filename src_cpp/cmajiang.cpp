#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "xiangting.h"
#include "game.h"
#include "feature.h"
#include "random.h"
#include "paipu.h"
#include "svg.h"

#include <functional>

namespace py = pybind11;

Defen __hule(const Shoupai& shoupai, const std::string& rongpai,
    const Rule& rule,
    const int zhuangfeng,
    const int menfeng,
    const int lizhi,
    const bool yifa,
    const bool qianggang,
    const bool lingshang,
    const int haidi,
    const int tianhu,
    const std::vector<std::string>& baopai,
    const std::vector<std::string>& libaopai,
    const int changbang,
    const int lizhibang
) {
    return hule(shoupai, rongpai,
        Param{
            rule, zhuangfeng, menfeng,
            Param::Hupai{ lizhi, yifa, qianggang, lingshang, haidi, tianhu },
            baopai, libaopai,
            Param::Jicun{ changbang, lizhibang }});
}

std::vector<std::string> __tingpai(const Shoupai& shoupai) {
    return tingpai(shoupai);
}

Rule create_rule(
    const int startingPoints,
    const std::array<float, 4>& rankPoints,
    const bool roundRankPoints,
    const bool doubleWindTileScore,
    const std::array<int, 3>& hongpai,
    const bool canduan,
    const int canChangePermissionLevel,
    const int roundsType,
    const bool abortiveDraw,
    const bool liujumanguan,
    const bool notenDeclaration,
    const bool notenPenalty,
    const int maxSimultaneousWinners,
    const int dealerContinuationType,
    const bool bankruptcyEndAll,
    const bool lastRoundStop,
    const int overtimeType,
    const bool yifa,
    const bool libaopai,
    const bool gangbaopai,
    const bool ganglibaopai,
    const bool gangbaopaiPostAddition,
    const bool lizhiWithoutTsumoBonus,
    const int lizhiPostClosedGangPermissionLevel,
    const bool damanguanCombination,
    const bool doubleDamanguan,
    const bool countedDamanguan,
    const bool damanguanPao,
    const bool roundUpManguan
    ) {
    return Rule{
        startingPoints,
        rankPoints,
        roundRankPoints,
        doubleWindTileScore,
        hongpai,
        canduan,
        canChangePermissionLevel,
        roundsType,
        abortiveDraw,
        liujumanguan,
        notenDeclaration,
        notenPenalty,
        maxSimultaneousWinners,
        dealerContinuationType,
        bankruptcyEndAll,
        lastRoundStop,
        overtimeType,
        yifa,
        libaopai,
        gangbaopai,
        ganglibaopai,
        gangbaopaiPostAddition,
        lizhiWithoutTsumoBonus,
        lizhiPostClosedGangPermissionLevel,
        damanguanCombination,
        doubleDamanguan,
        countedDamanguan,
        damanguanPao,
        roundUpManguan
    };
}

extern std::mt19937_64 engine;
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
Shoupai random_setup(const Rule& rule, const std::function<void(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt)>& setup_func) {
    std::vector<std::string> pai;
    std::vector<std::string> fulou;
    std::map<std::string, int> rest = init_rest(rule);
    setup_func(pai, rest, fulou, rule, engine);
    std::shuffle(pai.begin(), pai.end(), engine);
    return Shoupai{ pai, fulou };
}


PYBIND11_MODULE(_cmajiang, m) {
    m.doc() = "cmajiang module";
    py::class_<Shoupai>(m, "Shoupai")
        .def(py::init<>())
        .def(py::init<const std::vector<std::string>&>())
        .def(py::init<const std::string&>())
        .def("__repr__",
                [](const Shoupai& shoupai) {
                    return shoupai.toString();
                }
            )
        .def("clone", &Shoupai::clone)
        .def("zimo", &Shoupai::zimo, py::arg("p"), py::arg("check") = true)
        .def("dapai", &Shoupai::dapai, py::arg("p"), py::arg("check") = true)
        .def("fulou", &Shoupai::fulou, py::arg("m"), py::arg("check") = true)
        .def("gang", &Shoupai::gang, py::arg("m"), py::arg("check") = true)
        .def_property_readonly("menqian", &Shoupai::menqian)
        .def_property_readonly("lizhi", &Shoupai::lizhi)
        .def("get_dapai", &Shoupai::get_dapai, py::arg("check") = true)
        .def("get_chi_mianzi", &Shoupai::get_chi_mianzi, py::arg("p"), py::arg("check") = true)
        .def("get_peng_mianzi", &Shoupai::get_peng_mianzi)
        .def("get_gang_mianzi", &Shoupai::get_gang_mianzi, py::arg("p") = std::string{})
        .def("bingpai", &Shoupai::bingpai)
        .def_property_readonly("m", &Shoupai::m)
        .def_property_readonly("p", &Shoupai::p)
        .def_property_readonly("s", &Shoupai::s)
        .def_property_readonly("z", &Shoupai::z)
        .def("m_", &Shoupai::m_)
        .def("p_", &Shoupai::p_)
        .def("s_", &Shoupai::s_)
        .def("z_", &Shoupai::z_)
        .def_property_readonly("_fulou", &Shoupai::fulou_)
        .def_property_readonly("_zimo", &Shoupai::zimo_)
        .def("_repr_svg_", &shoupai_to_svg)
        ;
    py::class_<Rule>(m, "Rule")
        .def(py::init<>())
        .def(py::init(&create_rule),
            py::arg("startingPoints") = 25000,
            py::arg("rankPoints") = std::array<float, 4>{ 20, 10, -10, -20 },
            py::arg("roundRankPoints") = false,
            py::arg("doubleWindTileScore") = false,
            py::arg("hongpai") = std::array<int, 3>{ 1, 1, 1 },
            py::arg("canduan") = true,
            py::arg("canChangePermissionLevel") = 0,
            py::arg("roundsType") = 2,
            py::arg("abortiveDraw") = true,
            py::arg("liujumanguan") = true,
            py::arg("notenDeclaration") = false,
            py::arg("notenPenalty") = true,
            py::arg("maxSimultaneousWinners") = 2,
            py::arg("dealerContinuationType") = 2,
            py::arg("bankruptcyEndAll") = true,
            py::arg("lastRoundStop") = true,
            py::arg("overtimeType") = 1,
            py::arg("yifa") = true,
            py::arg("libaopai") = true,
            py::arg("gangbaopai") = true,
            py::arg("ganglibaopai") = true,
            py::arg("gangbaopaiPostAddition") = true,
            py::arg("lizhiWithoutTsumoBonus") = false,
            py::arg("lizhiPostClosedGangPermissionLevel") = 2,
            py::arg("damanguanCombination") = true,
            py::arg("doubleDamanguan") = true,
            py::arg("countedDamanguan") = true,
            py::arg("damanguanPao") = true,
            py::arg("roundUpManguan") = false
        )
        .def_readwrite("startingPoints", &Rule::startingPoints)
        .def_readwrite("rankPoints", &Rule::rankPoints)
        .def_readwrite("doubleWindTileScore", &Rule::doubleWindTileScore)
        .def_readwrite("hongpai", &Rule::hongpai)
        .def("hongpai_", &Rule::hongpai_)
        .def_readwrite("canduan", &Rule::canduan)
        .def_readwrite("canChangePermissionLevel", &Rule::canChangePermissionLevel)
        .def_readwrite("roundsType", &Rule::roundsType)
        .def_readwrite("abortiveDraw", &Rule::abortiveDraw)
        .def_readwrite("liujumanguan", &Rule::liujumanguan)
        .def_readwrite("notenDeclaration", &Rule::notenDeclaration)
        .def_readwrite("notenPenalty", &Rule::notenPenalty)
        .def_readwrite("maxSimultaneousWinners", &Rule::maxSimultaneousWinners)
        .def_readwrite("dealerContinuationType", &Rule::dealerContinuationType)
        .def_readwrite("bankruptcyEndAll", &Rule::bankruptcyEndAll)
        .def_readwrite("lastRoundStop", &Rule::lastRoundStop)
        .def_readwrite("overtimeType", &Rule::overtimeType)
        .def_readwrite("yifa", &Rule::yifa)
        .def_readwrite("libaopai", &Rule::libaopai)
        .def_readwrite("gangbaopai", &Rule::gangbaopai)
        .def_readwrite("ganglibaopai", &Rule::ganglibaopai)
        .def_readwrite("gangbaopaiPostAddition", &Rule::gangbaopaiPostAddition)
        .def_readwrite("lizhiWithoutTsumoBonus", &Rule::lizhiWithoutTsumoBonus)
        .def_readwrite("lizhiPostClosedGangPermissionLevel", &Rule::lizhiPostClosedGangPermissionLevel)
        .def_readwrite("damanguanCombination", &Rule::damanguanCombination)
        .def_readwrite("doubleDamanguan", &Rule::doubleDamanguan)
        .def_readwrite("countedDamanguan", &Rule::countedDamanguan)
        .def_readwrite("damanguanPao", &Rule::damanguanPao)
        .def_readwrite("roundUpManguan", &Rule::roundUpManguan)
        ;
    py::class_<Param::Hupai>(m, "ParamHupai")
        .def(py::init<>())
        .def_readwrite("lizhi", &Param::Hupai::lizhi)
        .def_readwrite("yifa", &Param::Hupai::yifa)
        .def_readwrite("qianggang", &Param::Hupai::qianggang)
        .def_readwrite("lingshang", &Param::Hupai::lingshang)
        .def_readwrite("haidi", &Param::Hupai::haidi)
        .def_readwrite("tianhu", &Param::Hupai::tianhu)
        ;
    py::class_<Param::Jicun>(m, "Jicun")
        .def(py::init<>())
        .def_readwrite("changbang", &Param::Jicun::changbang)
        .def_readwrite("lizhibang", &Param::Jicun::lizhibang)
        ;
    py::class_<Param>(m, "Param")
        .def(py::init<>())
        .def_readwrite("rule", &Param::rule)
        .def_readwrite("zhuangfeng", &Param::zhuangfeng)
        .def_readwrite("menfeng", &Param::menfeng)
        .def_readwrite("hupai", &Param::hupai)
        .def_readwrite("baopai", &Param::baopai)
        .def_readwrite("libaopai", &Param::libaopai)
        .def_readwrite("jicun", &Param::jicun)
        ;
    py::enum_<Hupai::Name>(m, "HupaiName")
        .value("ZHUANGFENG", Hupai::Name::ZHUANGFENG, u8"場風")
        .value("ZHUANGFENGDONG", Hupai::Name::ZHUANGFENGDONG, u8"場風 東")
        .value("ZHUANGFENGNAN", Hupai::Name::ZHUANGFENGNAN, u8"場風 南")
        .value("ZHUANGFENGXI", Hupai::Name::ZHUANGFENGXI, u8"場風 西")
        .value("ZHUANGFENGBEI", Hupai::Name::ZHUANGFENGBEI, u8"場風 北")
        .value("MENFENG=4", Hupai::Name::MENFENG, u8"自風")
        .value("MENFENGDONG=4", Hupai::Name::MENFENGDONG, u8"自風 東")
        .value("MENFENGNAN", Hupai::Name::MENFENGNAN, u8"自風 南")
        .value("MENFENGXI", Hupai::Name::MENFENGXI, u8"自風 西")
        .value("MENFENGBEI", Hupai::Name::MENFENGBEI, u8"自風 北")
        .value("FANPAI=8", Hupai::Name::FANPAI, u8"翻牌")
        .value("FANPAIBAI=8", Hupai::Name::FANPAIBAI, u8"翻牌 白")
        .value("FANPAIFA", Hupai::Name::FANPAIFA, u8"翻牌 發")
        .value("FANPAIZHONG", Hupai::Name::FANPAIZHONG, u8"翻牌 中")
        .value("BAOPAI", Hupai::Name::BAOPAI, u8"ドラ")
        .value("CHIBAOPAI", Hupai::Name::CHIBAOPAI, u8"赤ドラ")
        .value("LIBAOPAI", Hupai::Name::LIBAOPAI, u8"裏ドラ")
        .value("LIZHI", Hupai::Name::LIZHI, u8"立直")
        .value("DABULIZHI", Hupai::Name::DABULIZHI, u8"ダブル立直")
        .value("YIPA", Hupai::Name::YIPA, u8"一発")
        .value("HAIDIMOYUE", Hupai::Name::HAIDIMOYUE, u8"海底摸月")
        .value("HEDILAOYU", Hupai::Name::HEDILAOYU, u8"河底撈魚")
        .value("LINGSHANGKAIHUA", Hupai::Name::LINGSHANGKAIHUA, u8"嶺上開花")
        .value("QIANGGANG", Hupai::Name::QIANGGANG, u8"槍槓")
        .value("MENQIANQINGZIMOHE", Hupai::Name::MENQIANQINGZIMOHE, u8"門前清自摸和")
        .value("PINGHE", Hupai::Name::PINGHE, u8"平和")
        .value("DUANYAOJIU", Hupai::Name::DUANYAOJIU, u8"断幺九")
        .value("YIBEIKOU", Hupai::Name::YIBEIKOU, u8"一盃口")
        .value("SANSETONGSHUN", Hupai::Name::SANSETONGSHUN, u8"三色同順")
        .value("YIQITONGGUAN", Hupai::Name::YIQITONGGUAN, u8"一気通貫")
        .value("HUNQUANDAIYAOJIU", Hupai::Name::HUNQUANDAIYAOJIU, u8"混全帯幺九")
        .value("QIDUIZI", Hupai::Name::QIDUIZI, u8"七対子")
        .value("DUIDUIHU", Hupai::Name::DUIDUIHU, u8"対々和")
        .value("SANANKE", Hupai::Name::SANANKE, u8"三暗刻")
        .value("SANGANGZI", Hupai::Name::SANGANGZI, u8"三槓子")
        .value("SANSETONGKE", Hupai::Name::SANSETONGKE, u8"三色同刻")
        .value("HUNLAOTOU", Hupai::Name::HUNLAOTOU, u8"混老頭")
        .value("XIAOSANYUAN", Hupai::Name::XIAOSANYUAN, u8"小三元")
        .value("HUNYISE", Hupai::Name::HUNYISE, u8"混一色")
        .value("CHUNQUANDAIYAOJIU", Hupai::Name::CHUNQUANDAIYAOJIU, u8"純全帯幺九")
        .value("ERBEIKOU", Hupai::Name::ERBEIKOU, u8"二盃口")
        .value("QINGYISE", Hupai::Name::QINGYISE, u8"清一色")
        .value("GUOSHIWUSHUANG", Hupai::Name::GUOSHIWUSHUANG, u8"国士無双")
        .value("GUOSHIWUSHUANGSHISANMIAN", Hupai::Name::GUOSHIWUSHUANGSHISANMIAN, u8"国士無双十三面")
        .value("SIANKE", Hupai::Name::SIANKE, u8"四暗刻")
        .value("SIANKEDANQI", Hupai::Name::SIANKEDANQI, u8"四暗刻単騎")
        .value("DASANYUAN", Hupai::Name::DASANYUAN, u8"大三元")
        .value("XIAOSIXI", Hupai::Name::XIAOSIXI, u8"小四喜")
        .value("DASIXI", Hupai::Name::DASIXI, u8"大四喜")
        .value("ZIYISE", Hupai::Name::ZIYISE, u8"字一色")
        .value("LVYISE", Hupai::Name::LVYISE, u8"緑一色")
        .value("QINGLAOTOU", Hupai::Name::QINGLAOTOU, u8"清老頭")
        .value("SIGANGZI", Hupai::Name::SIGANGZI, u8"四槓子")
        .value("JIULIANBAODENG", Hupai::Name::JIULIANBAODENG, u8"九蓮宝燈")
        .value("CHUNZHENGJIULIANBAODENG", Hupai::Name::CHUNZHENGJIULIANBAODENG, u8"純正九蓮宝燈")
        .value("TIANHE", Hupai::Name::TIANHE, u8"天和")
        .value("DIHE", Hupai::Name::DIHE, u8"地和")
        .export_values()
        ;
    py::class_<Hupai>(m, "Hupai")
        .def(py::init<const Hupai::Name, const int, const char>())
        .def(py::init<const Hupai::Name, const int>())
        .def_readwrite("name", &Hupai::name)
        .def_readwrite("fanshu", &Hupai::fanshu)
        .def_readwrite("baojia", &Hupai::baojia)
        ;
    py::class_<Defen>(m, "Defen")
        .def(py::init<>())
        .def_readwrite("hupai", &Defen::hupai)
        .def_readwrite("fu", &Defen::fu)
        .def_readwrite("fanshu", &Defen::fanshu)
        .def_readwrite("damanguan", &Defen::damanguan)
        .def_readwrite("defen", &Defen::defen)
        .def_readwrite("fenpei", &Defen::fenpei)
        .def_readwrite("menfeng", &Defen::menfeng)
        .def("__repr__",
                [](const Defen& defen) {
                    constexpr char* name[] = {
                        u8"場風 東", u8"場風 南", u8"場風 西", u8"場風 北", u8"自風 東", u8"自風 南", u8"自風 西", u8"自風 北", u8"翻牌 白", u8"翻牌 發", u8"翻牌 中", u8"ドラ", u8"赤ドラ", u8"裏ドラ", u8"立直", u8"ダブル立直", u8"一発", u8"海底摸月", u8"河底撈魚", u8"嶺上開花", u8"槍槓", u8"門前清自摸和", u8"平和", u8"断幺九", u8"一盃口", u8"三色同順", u8"一気通貫", u8"混全帯幺九", u8"七対子", u8"対々和", u8"三暗刻", u8"三槓子", u8"三色同刻", u8"混老頭", u8"小三元", u8"混一色", u8"純全帯幺九", u8"二盃口", u8"清一色", u8"国士無双", u8"国士無双十三面", u8"四暗刻", u8"四暗刻単騎", u8"大三元", u8"小四喜", u8"大四喜", u8"字一色", u8"緑一色", u8"清老頭", u8"四槓子", u8"九蓮宝燈", u8"純正九蓮宝燈", u8"天和", u8"地和"
                    };
                    std::ostringstream ss;
                    for (const auto& hupai : defen.hupai) {
                        ss << name[(int)hupai.name];
                        if (hupai.fanshu > 0)
                            ss <<  "\t" << hupai.fanshu << u8"翻";
                        ss << "\n";
                    }
                    if (defen.damanguan == 0) {
                        ss << defen.fu << u8"符 " << defen.fanshu << u8"翻 ";
                    }
                    const double manguan = defen.defen / (defen.menfeng == 0 ? 6.0 : 4.0) / 2000.0;
                    if (manguan >= 4 * 6)      ss << u8"六倍役満 ";
                    else if (manguan >= 4 * 5) ss << u8"五倍役満 ";
                    else if (manguan >= 4 * 4) ss << u8"四倍役満 ";
                    else if (manguan >= 4 * 3) ss << u8"トリプル役満 ";
                    else if (manguan >= 4 * 2) ss << u8"ダブル役満 ";
                    else if (manguan >= 4)     ss << u8"役満 ";
                    else if (manguan >= 3)     ss << u8"三倍満 ";
                    else if (manguan >= 2)     ss << u8"倍満 ";
                    else if (manguan >= 1.5)   ss << u8"跳満 ";
                    else if (manguan >= 1)     ss << u8"満貫 ";

                    ss << defen.defen << u8"点";
                    return ss.str();
                }
            )
        ;
    m.def("hule", &__hule, py::arg("shoupai"), py::arg("rongpai") = std::string{},
        py::arg("rule") = Rule{},
        py::arg("zhuangfeng") = 0,
        py::arg("menfeng") = 1,
        py::arg("lizhi") = 0,
        py::arg("yifa") = false,
        py::arg("qianggang") = false,
        py::arg("lingshang") = false,
        py::arg("haidi") = 0,
        py::arg("tianhu") = 0,
        py::arg("baopai") = std::vector<std::string>{},
        py::arg("libaopai") = std::vector<std::string>{},
        py::arg("changbang") = 0,
        py::arg("lizhibang") = 0
    );
    m.def("xiangting", &xiangting);
    m.def("tingpai", &__tingpai);
    py::class_<Shan>(m, "Shan")
        .def(py::init<>())
        .def(py::init<const Rule&>())
        .def("set", &Shan::set)
        .def_property("pai", [](Shan& shan) { return shan.pai(); }, [](const Shan& shan) { return shan.pai(); })
        .def_property_readonly("paishu", &Shan::paishu)
        .def_property_readonly("baopai", &Shan::baopai)
        .def_property_readonly("libaopai", &Shan::libaopai)
        .def_property_readonly("_libaopai", &Shan::libaopai_)
        .def("zimo", &Shan::zimo)
        .def("gangzimo", &Shan::gangzimo)
        .def("kaigang", &Shan::kaigang)
        .def("close", &Shan::close)
        .def_property_readonly("rule", &Shan::rule)
        ;
    py::class_<He>(m, "He")
        .def(py::init<>())
        .def("set", &He::set)
        .def("dapai", &He::dapai)
        .def("fulou", &He::fulou)
        .def("find", &He::find)
        .def_property_readonly("pai", &He::pai)
        ;
    py::enum_<Game::Pingju::Name>(m, "PingjuName")
        .value("NONE", Game::Pingju::Name::NONE)
        .value("HUANGPAI", Game::Pingju::Name::HUANGPAI, u8"荒牌平局")
        .value("MANGUAN", Game::Pingju::Name::MANGUAN, u8"流し満貫")
        .value("YAO9", Game::Pingju::Name::YAO9, u8"九種九牌")
        .value("HULE3", Game::Pingju::Name::HULE3, u8"三家和了")
        .value("LIZHI4", Game::Pingju::Name::LIZHI4, u8"四家立直")
        .value("FENG4", Game::Pingju::Name::FENG4, u8"四風連打")
        .value("GANG4", Game::Pingju::Name::GANG4, u8"四槓散了")
        ;
    py::class_<Game::Model>(m, "Model")
        .def(py::init<>())
        .def_readwrite("qijia", &Game::Model::qijia)
        .def_readwrite("zhuangfeng", &Game::Model::zhuangfeng)
        .def_readwrite("jushu", &Game::Model::jushu)
        .def_readwrite("changbang", &Game::Model::changbang)
        .def_readwrite("lizhibang", &Game::Model::lizhibang)
        .def_readwrite("defen", &Game::Model::defen)
        .def_readwrite("shan", &Game::Model::shan)
        .def_readwrite("shoupai", &Game::Model::shoupai)
        .def_readwrite("he", &Game::Model::he)
        .def_readwrite("player_id", &Game::Model::player_id)
        .def_readwrite("lunban", &Game::Model::lunban)
        ;
    py::enum_<Game::Message>(m, "Message")
        .value("NONE", Game::Message::NONE)
        .value("DAOPAI", Game::Message::DAOPAI, u8"倒牌")
        .value("FULOU", Game::Message::FULOU, u8"副露")
        .value("HULE", Game::Message::HULE, u8"和了")
        .value("GANG", Game::Message::GANG, u8"槓")
        .value("DAPAI", Game::Message::DAPAI, u8"打牌")
        ;
    py::enum_<Game::Status>(m, "Status")
        .value("NONE", Game::Status::NONE)
        .value("KAIJU", Game::Status::KAIJU, u8"開始")
        .value("QIPAI", Game::Status::QIPAI, u8"配牌")
        .value("ZIMO", Game::Status::ZIMO, u8"自摸")
        .value("DAPAI", Game::Status::DAPAI, u8"打牌")
        .value("FULOU", Game::Status::FULOU, u8"副露")
        .value("GANG", Game::Status::GANG, u8"槓")
        .value("GANGZIMO", Game::Status::GANGZIMO, u8"槓自摸")
        .value("HULE", Game::Status::HULE, u8"和了")
        .value("PINGJU", Game::Status::PINGJU, u8"流局")
        .value("JIEJI", Game::Status::JIEJI, u8"終局")
        ;
    py::class_<Game::Reply>(m, "Reply")
        .def(py::init<>())
        .def_readwrite("msg", &Game::Reply::msg)
        .def_readwrite("arg", &Game::Reply::arg)
        ;
    py::class_<Game::Paipu::Round>(m, "Round")
        .def(py::init<>())
        .def_readwrite("model", &Game::Paipu::Round::model)
        .def_readwrite("moves", &Game::Paipu::Round::moves)
        ;
    py::class_<Game::Paipu>(m, "Paipu")
        .def(py::init<>())
        .def(py::init<const std::string&>())
        .def_readwrite("rule", &Game::Paipu::rule)
        .def_readwrite("rounds", &Game::Paipu::rounds)
        .def("__repr__", [](const Game::Paipu& paipu) {
            std::stringstream ss;
            ss << paipu;
            return ss.str();
        })
        ;
    py::class_<Game>(m, "Game")
        .def(py::init<>())
        .def(py::init<const Rule&>())
        .def("reply", &Game::reply, py::arg("id"), py::arg("msg"), py::arg("arg") = std::string{})
        .def("next", &Game::next)
        .def_property_readonly("lunban_player_id", &Game::lunban_player_id)
        .def("player_lunban", &Game::player_lunban)
        .def("kaiju", &Game::kaiju, py::arg("qijia") = -1)
        .def("qipai", &Game::qipai)
        .def("qipai_", &Game::qipai_)
        .def("zimo", &Game::zimo)
        .def("dapai", &Game::dapai)
        .def("fulou", &Game::fulou)
        .def("gang", &Game::gang)
        .def("gangzimo", &Game::gangzimo)
        .def("kaigang", &Game::kaigang)
        .def("hule", &Game::hule)
        .def("pingju", &Game::pingju)
        .def("last", &Game::last)
        .def("jieju", &Game::jieju)
        .def("get_reply", &Game::get_reply)
        .def("reply_kaiju", &Game::reply_kaiju)
        .def("reply_qipai", &Game::reply_qipai)
        .def("reply_zimo", &Game::reply_zimo)
        .def("reply_dapai", &Game::reply_dapai)
        .def("reply_fulou", &Game::reply_fulou)
        .def("reply_gang", &Game::reply_gang)
        .def("reply_hule", &Game::reply_hule)
        .def("reply_pingju", &Game::reply_pingju)
        .def("get_dapai", &Game::get_dapai)
        .def("get_chi_mianzi", &Game::get_chi_mianzi)
        .def("get_peng_mianzi", &Game::get_peng_mianzi)
        .def("get_gang_mianzi", &Game::get_gang_mianzi, py::arg("l") = -1)
        .def("allow_lizhi", &Game::allow_lizhi, py::arg("p") = std::string{})
        .def("allow_hule", &Game::allow_hule, py::arg("l") = -1)
        .def("allow_pingju", &Game::allow_pingju)
        .def_property_readonly("qijia", &Game::qijia)
        .def_property_readonly("zhuangfeng", &Game::zhuangfeng)
        .def_property_readonly("jushu", &Game::jushu)
        .def_property_readonly("changbang", &Game::changbang)
        .def_property_readonly("lizhibang", &Game::lizhibang)
        .def_property_readonly("defen", &Game::defen)
        .def_property_readonly("shan", &Game::shan)
        .def_property_readonly("shoupai", &Game::shoupai)
        .def("shoupai_", &Game::shoupai_)
        .def_property_readonly("he", &Game::he)
        .def("he_", &Game::he_)
        .def_property_readonly("player_id", &Game::player_id)
        .def_property_readonly("lunban", &Game::lunban)
        .def_property_readonly("max_jushu", &Game::max_jushu)
        .def_property_readonly("diyizimo", &Game::diyizimo)
        .def_property_readonly("fengpai", &Game::fengpai)
        .def_property_readonly("_dapai", &Game::dapai_)
        .def_property_readonly("_dapai", &Game::dapai_)
        .def_property_readonly("lizhi", &Game::lizhi)
        .def_property_readonly("yifa", &Game::yifa)
        .def_property_readonly("neng_rong", &Game::neng_rong)
        .def_property_readonly("_fulou", &Game::fulou_)
        .def_property_readonly("_gang_", &Game::gang)
        .def_property_readonly("_hule", &Game::hule_)
        .def_property_readonly("_defen", &Game::defen_)
        .def_property_readonly("rank", &Game::rank)
        .def_property_readonly("point", &Game::point)
        .def_property_readonly("_pingju", &Game::pingju_)
        .def_property_readonly("fenpei", &Game::fenpei)
        .def_property_readonly("lianzhuang", &Game::lianzhuang)
        .def_property_readonly("no_game", &Game::no_game)
        .def_property_readonly("model", [](const Game& game) { return game.model(); })
        .def_property_readonly("rule", &Game::rule)
        .def_property_readonly("status", &Game::status)
        .def_property_readonly("paipu", [](const Game& game) { return game.paipu(); })
        ;
    py::class_<PaipuReplay>(m, "PaipuReplay")
        .def(py::init<const Game::Paipu&>())
        .def("next", &PaipuReplay::next)
        .def_property_readonly("game", &PaipuReplay::game)
        .def_property_readonly("round", &PaipuReplay::round)
        .def_property_readonly("ply", &PaipuReplay::ply)
        .def_property_readonly("status", &PaipuReplay::status)
        ;
    m.attr("N_CHANNELS_STATUS") = N_CHANNELS_STATUS;
    m.attr("N_CHANNELS_SHOUPAI") = N_CHANNELS_SHOUPAI;
    m.attr("N_CHANNELS_FULOU") = N_CHANNELS_FULOU;
    m.attr("N_CHANNELS_PAI") = N_CHANNELS_PAI;
    m.attr("N_CHANNELS_PAI") = N_CHANNELS_PAI;
    m.attr("N_CHANNELS_TINGPAI") = N_CHANNELS_TINGPAI;
    m.attr("N_CHANNELS_LIZHI") = N_CHANNELS_LIZHI;
    m.attr("N_CHANNELS_HE") = N_CHANNELS_HE;
    m.attr("N_CHANNELS_TAJIADAPAI") = N_CHANNELS_TAJIADAPAI;
    m.attr("N_CHANNELS_BAOPAI") = N_CHANNELS_BAOPAI;
    m.attr("N_CHANNELS_MENFENG") = N_CHANNELS_MENFENG;
    m.attr("N_CHANNELS_ZHUANGFENG") = N_CHANNELS_ZHUANGFENG;
    m.attr("N_CHANNELS_PAISHU") = N_CHANNELS_PAISHU;
    m.attr("N_CHANNELS_SHAN") = N_CHANNELS_SHAN;
    m.attr("N_CHANNELS_PUBLIC") = N_CHANNELS_PUBLIC;
    m.attr("N_CHANNELS_PRIVATE") = N_CHANNELS_PRIVATE;
    m.attr("N_ACTIONS") = N_ACTIONS;
    m.def("status_featuers", [](const Game& game, const int lunban, py::array_t<float> ndarray) {
        auto data = static_cast<float*>(ndarray.request().ptr);
        status_featuers(game, lunban, reinterpret_cast<float(*)[9][4]>(data));
    });
    m.def("shoupai_features", [](const Shoupai& shoupai, py::array_t<float> ndarray) {
        auto data = static_cast<float*>(ndarray.request().ptr);
        shoupai_features(shoupai, reinterpret_cast<float(*)[9][4]>(data));
    });
    m.def("fulou_features", [](const Shoupai& shoupai, py::array_t<float> ndarray) {
        auto data = static_cast<float*>(ndarray.request().ptr);
        fulou_features(shoupai, reinterpret_cast<float(*)[9][4]>(data));
    });
    m.def("pai_features", [](const std::string& pai, py::array_t<float> ndarray) {
        auto data = static_cast<float*>(ndarray.request().ptr);
        pai_features(pai, reinterpret_cast<float(*)[9][4]>(data));
    });
    m.def("he_features", [](const He& he, py::array_t<float> ndarray) {
        auto data = static_cast<float*>(ndarray.request().ptr);
        he_features(he, reinterpret_cast<float(*)[9][4]>(data));
    });
    m.def("tajiadapai_features", [](const Game& game, const int lunban, py::array_t<float> ndarray) {
        auto data = static_cast<float*>(ndarray.request().ptr);
        tajiadapai_features(game, lunban, reinterpret_cast<float(*)[9][4]>(data));
    });
    m.def("baopai_features", [](const std::vector<std::string>& baopai, py::array_t<float> ndarray) {
        auto data = static_cast<float*>(ndarray.request().ptr);
        baopai_features(baopai, reinterpret_cast<float(*)[9][4]>(data));
    });
    m.def("shan_features", [](const Shan& shan, py::array_t<float> ndarray) {
        auto data = static_cast<float*>(ndarray.request().ptr);
        shan_features(shan, reinterpret_cast<float(*)[9][4]>(data));
    });
    m.def("public_features", [](const Game& game, const int lunban, py::array_t<float> ndarray) {
        auto data = static_cast<float*>(ndarray.request().ptr);
        public_features(game, lunban, reinterpret_cast<float(*)[9][4]>(data));
    });
    m.def("private_features", [](const Game& game, const int lunban, py::array_t<float> ndarray) {
        auto data = static_cast<float*>(ndarray.request().ptr);
        private_features(game, lunban, reinterpret_cast<float(*)[9][4]>(data));
    });
    m.def("set_seed", &set_seed);
    m.def("random_zhuangfeng", [](const int zhuangfeng, const Rule& rule) {
        return random_setup(rule, [zhuangfeng](std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
            setup_zhuangfeng(pai, rest, fulou, zhuangfeng, rule, mt);
        });
    }, py::arg("zhuangfeng") = 0, py::arg("rule") = Rule{}
    );
    m.def("random_menfeng", [](const int menfeng, const Rule& rule) {
        return random_setup(rule, [menfeng](std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
            setup_menfeng(pai, rest, fulou, menfeng, rule, mt);
        });
    }, py::arg("menfeng") = 0, py::arg("rule") = Rule{}
    );
    m.def("random_fanpai", [](const Rule& rule) {
        return random_setup(rule, setup_fanpai);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_pinghe", [](const int zhuangfeng, const int menfeng, const Rule& rule) {
        return random_setup(rule, [zhuangfeng, menfeng](std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
            setup_pinghe(pai, rest, zhuangfeng, menfeng, rule, mt);
        });
    }, py::arg("zhuangfeng") = 0, py::arg("menfeng") = 0, py::arg("rule") = Rule{}
    );
    m.def("random_duanyaojiu", [](const Rule& rule) {
        return random_setup(rule, [](std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
            setup_duanyaojiu(pai, rest, rule, mt);
        });
    }, py::arg("rule") = Rule{}
    );
    m.def("random_yibeikou", [](const Rule& rule) {
        return random_setup(rule, [](std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
            setup_yibeikou(pai, rest, rule, mt);
        });
    }, py::arg("rule") = Rule{}
    );
    m.def("random_sansetongshun", [](const Rule& rule) {
        return random_setup(rule, setup_sansetongshun);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_yiqitongguan", [](const Rule& rule) {
        return random_setup(rule, setup_yiqitongguan);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_hunquandaiyaojiu", [](const Rule& rule) {
        return random_setup(rule, setup_hunquandaiyaojiu);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_qiduizi", [](const Rule& rule) {
        return random_setup(rule, [](std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
            setup_qiduizi(pai, rest, rule, mt);
        });
    }, py::arg("rule") = Rule{}
    );
    m.def("random_duiduihu", [](const Rule& rule) {
        return random_setup(rule, setup_duiduihu);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_sananke", [](const Rule& rule) {
        return random_setup(rule, setup_sananke);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_sangangzi", [](const Rule& rule) {
        return random_setup(rule, setup_sangangzi);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_sansetongke", [](const Rule& rule) {
        return random_setup(rule, setup_sansetongke);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_hunlaotou", [](const Rule& rule) {
        return random_setup(rule, setup_hunlaotou);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_xiaosanyuan", [](const Rule& rule) {
        return random_setup(rule, setup_xiaosanyuan);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_hunyise", [](const Rule& rule) {
        return random_setup(rule, setup_hunyise);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_chunquandaiyaojiu", [](const Rule& rule) {
        return random_setup(rule, setup_chunquandaiyaojiu);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_erbeikou", [](const Rule& rule) {
        return random_setup(rule, setup_erbeikou);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_qingyise", [](const Rule& rule) {
        return random_setup(rule, setup_qingyise);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_guoshiwushuang", [](const Rule& rule) {
        return random_setup(rule, [](std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
            setup_guoshiwushuang(pai, rest, rule, mt);
        });
    }, py::arg("rule") = Rule{}
    );
    m.def("random_sianke", [](const Rule& rule) {
        return random_setup(rule, [](std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
            setup_sianke(pai, rest, rule, mt);
        });
    }, py::arg("rule") = Rule{}
    );
    m.def("random_dasanyuan", [](const Rule& rule) {
        return random_setup(rule, setup_dasanyuan);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_xiaosixi", [](const Rule& rule) {
        return random_setup(rule, setup_xiaosixi);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_dasixi", [](const Rule& rule) {
        return random_setup(rule, setup_dasixi);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_ziyise", [](const Rule& rule) {
        return random_setup(rule, setup_ziyise);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_lvyise", [](const Rule& rule) {
        return random_setup(rule, setup_lvyise);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_qinglaotou", [](const Rule& rule) {
        return random_setup(rule, setup_qinglaotou);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_sigangzi", [](const Rule& rule) {
        return random_setup(rule, setup_sigangzi);
    }, py::arg("rule") = Rule{}
    );
    m.def("random_jiulianbaodeng", [](const Rule& rule) {
        return random_setup(rule, [](std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt) {
            setup_jiulianbaodeng(pai, rest, rule, mt);
        });
    }, py::arg("rule") = Rule{}
    );
    m.def("random_game_state", [](Game& game, const int n_xiangting) {
        random_game_state(game, n_xiangting, engine);
    }, py::arg("game"), py::arg("n_xiangting") = 1
    );
}
