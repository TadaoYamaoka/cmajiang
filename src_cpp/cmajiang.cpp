#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "xiangting.h"
#include "game.h"

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
    const std::array<std::string, 4>& rankPoints,
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
		;
	py::class_<Rule>(m, "Rule")
        .def(py::init<>())
        .def(py::init(&create_rule),
            py::arg("startingPoints") = 25000,
            py::arg("rankPoints") = std::array<std::string, 4>{ "20.0", "10.0", "-10.0", "-20.0" },
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
        .value("CHANGFENG", Hupai::Name::CHANGFENG, u8"場風")
        .value("CHANGFENGDONG", Hupai::Name::CHANGFENGDONG, u8"場風 東")
        .value("CHANGFENGNAN", Hupai::Name::CHANGFENGNAN, u8"場風 南")
        .value("CHANGFENGXI", Hupai::Name::CHANGFENGXI, u8"場風 西")
        .value("CHANGFENGBEI", Hupai::Name::CHANGFENGBEI, u8"場風 北")
        .value("ZIFENG=4", Hupai::Name::ZIFENG, u8"自風")
        .value("ZIFENGDONG=4", Hupai::Name::ZIFENGDONG, u8"自風 東")
        .value("ZIFENGNAN", Hupai::Name::ZIFENGNAN, u8"自風 南")
        .value("ZIFENGXI", Hupai::Name::ZIFENGXI, u8"自風 西")
        .value("ZIFENGBEI", Hupai::Name::ZIFENGBEI, u8"自風 北")
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

                    ss << defen.defen << u8"点\n";
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
        .def_property("pai", &Shan::pai, &Shan::set_pai)
        .def_property_readonly("paishu", &Shan::paishu)
        .def_property_readonly("baopai", &Shan::baopai)
        .def_property_readonly("libaopai", &Shan::libaopai)
        .def("zimo", &Shan::zimo)
        .def("gangzimo", &Shan::gangzimo)
        .def("kaigang", &Shan::kaigang)
        .def("close", &Shan::close)
        ;
	py::class_<He>(m, "He")
		.def(py::init<>())
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
	py::class_<Game>(m, "Game")
		.def(py::init<>())
        .def(py::init<const Rule&>())
        .def("reply", &Game::reply)
        .def("next", &Game::next)
        .def_property_readonly("lunban_player_id", &Game::lunban_player_id)
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
        .def_property_readonly("model", &Game::model)
        .def_property_readonly("rule", &Game::rule)
        .def_property_readonly("status", &Game::status)
        ;
}
