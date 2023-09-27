#include "pch.h"
#include "../src_cpp/game.h"

#include <sstream>

Game init_game(
	const Rule& rule = {},
	const int qijia = -1,
	const int lizhibang = 0,
	const int changbang = 0,
	const std::vector<std::string> shoupai = {},
	const std::vector<std::string> zimo = {},
	const std::vector<std::string> gangzimo = {},
	const std::string baopai = {},
	const std::vector<int> defen = {}) {
    Game game(rule);

    if (qijia != -1) game.kaiju(qijia);
    else             game.kaiju();

    if (lizhibang) {
        game.model().lizhibang = lizhibang;
    }
    if (changbang) {
        game.model().changbang = changbang;
    }

    game.qipai();

    if (!shoupai.empty()) {
        for (int l = 0; l < 4; l++) {
            if (shoupai[l].empty()) continue;
            auto paistr = shoupai[l];
            if (paistr == "_") paistr = std::string(13, '_');
            game.model().shoupai[l] = Shoupai(paistr);
        }
    }
    if (!zimo.empty()) {
        auto& pai = game.model().shan.pai();
        for (int i = 0; i < zimo.size(); i++) {
            pai[pai.size() - 1 - i] = zimo[i];
        }
    }
    if (!gangzimo.empty()) {
        auto& pai = game.model().shan.pai();
        for (int i = 0; i < gangzimo.size(); i++) {
            pai[i] = gangzimo[i];
        }
    }
    if (!baopai.empty()) {
		game.model().shan.set_baopai({ baopai });
    }
    if (!defen.empty()) {
        for (int l = 0; l < 4; l++) {
            const auto id = game.model().player_id[l];
            game.model().defen[id] = defen[l];
        }
    }

    return game;
}

void set_reply(Game& game, const int l, const std::string& msg, const std::string& arg = {}) {
	game.reply(game.model().player_id[l], msg, arg);
}

TEST(GameTest, kaiju) {
	Game game;

	// 起家が設定されること
	{
		game.kaiju(0);
		EXPECT_EQ(game.qijia(), 0);
	}
	// 通知が伝わること
	EXPECT_EQ("kaiju", game.status());
	// 起家を乱数で設定できること
	{
		game.kaiju();
		EXPECT_TRUE(game.qijia() == 0 ||
			game.qijia() == 1 ||
			game.qijia() == 2 ||
			game.qijia() == 3);
	}
}

TEST(GameTest, qipai) {
	Game game;
	game.kaiju();

	// 牌山が生成されること
	{
		game.qipai();
		const auto& shan = game.shan();
		EXPECT_EQ(70, shan.paishu());
		EXPECT_EQ(1, shan.baopai().size());
		EXPECT_TRUE(shan.libaopai().empty());
	}
	// 配牌されること
	{
		std::regex regex(R"([mpsz])");
		for (int l = 0; l < 4; l++) {
			EXPECT_EQ(13, std::regex_replace(game.shoupai(l).toString(), regex, "").size());
		}
	}
	// 河が初期化されること
	{
		for (int l = 0; l < 4; l++) {
			EXPECT_EQ(0, game.he(l).pai().size());
		}
	}
	// 第一ツモ巡であること
	EXPECT_TRUE(game.diyizimo());
	// 四風連打中であること
	EXPECT_TRUE(game.fengpai());
	// 通知が伝わること
	EXPECT_EQ("qipai", game.status());

	// 使用する牌山を指定できること
	{
		auto game = init_game();
		Shan shan(game.rule());
		std::vector<std::string> qipai;
		std::copy(shan.pai().end() - 13, shan.pai().end(), std::back_inserter(qipai));
		Shoupai shoupai(qipai);
		game.qipai(shan);
		EXPECT_EQ(shoupai.toString(), game.model().shoupai[0].toString());
	}
	// 途中流局なしの場合、最初から四風連打中でないこと
	{
		Rule rule;
		rule.abortiveDraw/*途中流局あり*/ = false;
		auto game = init_game(rule);
		game.qipai();
		EXPECT_FALSE(game.fengpai());
	}
}

TEST(GameTest, zimo) {
	auto game = init_game();

	// 手番が更新されること
	{
		game.zimo();
		EXPECT_EQ(0, game.model().lunban);
	}
	// 牌山からツモられること
	EXPECT_EQ(69, game.model().shan.paishu());
	// 手牌にツモ牌が加えられること
	EXPECT_TRUE(game.model().shoupai[0].get_dapai().size());
	// 通知が伝わること
	EXPECT_EQ("zimo", game.status());
}

TEST(GameTest, dapai) {
	auto game = init_game();
	std::string dapai;

	// 手牌から打牌が切り出されること
	{
		game.zimo();
		dapai = game.model().shoupai[0].get_dapai()[0];
		game.dapai(dapai);
		EXPECT_THROW(game.model().shoupai[0].get_dapai(), std::runtime_error);
	}
	// 河に打牌されること
	EXPECT_EQ(dapai, game.model().he[0].pai()[0]);
	// 通知が伝わること
	EXPECT_EQ("dapai", game.status());

	// 風牌以外の打牌で四風連打中でなくなること
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "" });
		EXPECT_TRUE(game.fengpai());
		game.zimo();
		game.dapai("m1");
		EXPECT_FALSE(game.fengpai());
	}
	// 異なる風牌の打牌で四風連打中でなくなること
	{
		auto game = init_game({}, -1, 0, 0, { "_", "_", "", "" });
		EXPECT_TRUE(game.fengpai());
		game.zimo();
		game.dapai("z1");
		game.zimo();
		game.dapai("z2");
		EXPECT_FALSE(game.fengpai());
	}
	// 第一ツモ巡終了で四風連打中でなくなること
	{
		auto game = init_game({}, -1, 0, 0, { "_", "_", "", "" });
		EXPECT_TRUE(game.fengpai());
		game.zimo();
		game.dapai("z1");
		game.zimo();
		game.set_diyizimo(false);
		game.dapai("z1");
		EXPECT_FALSE(game.fengpai());
	}
	// ダブルリーチ
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "" });
		game.zimo();
		game.dapai("m1*");
		EXPECT_EQ(2, game.lizhi(game.model().lunban));
		EXPECT_TRUE(game.yifa(game.model().lunban));
	}
	// リーチ後の打牌で一発の権利を失うこと
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "" });
		game.set_yifa(0, true);
		game.zimo();
		game.dapai("m1");
		EXPECT_FALSE(game.yifa(game.model().lunban));
	}
	// テンパイ時に和了牌が河にある場合、フリテンとなること
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z11122", "", "", "" });
		game.model().lunban = 0;
		EXPECT_TRUE(game.neng_rong(game.model().lunban));
		game.dapai("m1");
		EXPECT_FALSE(game.neng_rong(game.model().lunban));
	}
	// リーチ後はフリテンが解除されないこと
	{
		auto game = init_game({}, -1, 0, 0, { "_____________*", "", "", "" });
		game.set_neng_rong(0, false);
		game.zimo();
		auto dapai = game.model().shoupai[0].zimo();
		game.dapai(dapai);
		EXPECT_FALSE(game.neng_rong(game.model().lunban));
	}
	// 加槓後の打牌で開槓されること
	{
		auto game = init_game({}, -1, 0, 0, { "__________,s333=", "", "", "" });
		game.zimo();
		game.gang("s333=3");
		game.gangzimo();
		game.dapai("p1");
		EXPECT_EQ(2, game.model().shan.baopai().size());
	}
}

TEST(GameTest, fulou) {
	auto game = init_game({}, -1, 0, 0, { "_", "_", "", "" });

	// 河から副露牌が拾われること
	{
		game.zimo();
		game.dapai("m2_");
		game.fulou("m12-3");
		EXPECT_EQ("m2_-", game.model().he[0].pai()[0]);
	}
	// 手番が更新されること(上家からのチー)
	EXPECT_EQ(1, game.model().lunban);
	// 手牌が副露されること
	EXPECT_EQ("m12-3", game.model().shoupai[1].fulou()[0]);
	// 通知が伝わること
	EXPECT_EQ("fulou", game.status());

	// 大明槓が副露されること
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "_" });
		game.zimo();
		game.dapai("m2");
		game.fulou("m2222+");
		EXPECT_EQ("m2222+", game.model().shoupai[3].fulou()[0]);
	}
	// 第一ツモ巡でなくなること
	{
		auto game = init_game({}, -1, 0, 0, { "_", "_", "", "_" });
		game.zimo();
		game.dapai("m3");
		game.fulou("m123-");
		EXPECT_FALSE(game.diyizimo());
	}
	// 一発がなくなること
	{
		auto game = init_game({}, -1, 0, 0, { "_", "_", "", "_" });
		game.zimo();
		game.dapai("m3*");
		game.fulou("m123-");
		EXPECT_FALSE(find(game.yifa(), true));
	}
}

TEST(GameTest, gang) {
	auto game = init_game({}, -1, 0, 0, { "__________,s555+", "", "", "" });

	// 加槓が副露されること
	{
		game.zimo();
		game.gang("s555+0");
		EXPECT_EQ("s555+0", game.model().shoupai[0].fulou()[0]);
	}
	// 通知が伝わること
	EXPECT_EQ("gang", game.status());

	// 暗槓が副露されること
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "" });
		game.zimo();
		game.gang("s5550");
		EXPECT_EQ("s5550", game.model().shoupai[0].fulou()[0]);
	}
	// 後乗せの槓が開槓されること
	{
		auto game = init_game({}, -1, 0, 0, { "_______,s222+,z111=", "", "", "" });
		game.zimo();
		game.gang("z111=1");
		game.gangzimo();
		game.gang("s222+2");
		EXPECT_EQ(2, game.model().shan.baopai().size());
	}
}

TEST(GameTest, gangzimo) {
	auto game = init_game({}, -1, 0, 0, { "_", "", "", "" });

	// 牌山からツモられること
	{
		game.zimo();
		game.gang("m5550");
		game.gangzimo();
		EXPECT_EQ(68, game.model().shan.paishu());
	}
	// 手牌にツモ牌が加えられること
	EXPECT_TRUE(game.model().shoupai[0].get_dapai().size());
	// 通知が伝わること
	EXPECT_EQ("gangzimo", game.status());

	// 第一ツモ巡でなくなること
	{
		auto game = init_game({}, -1, 0, 0, { "_", "_", "", "_" });
		game.zimo();
		game.gang("m3333");
		game.gangzimo();
		EXPECT_FALSE(game.diyizimo());
	}
	// 一発がなくなること
	{
		auto game = init_game({}, -1, 0, 0, { "_", "_", "", "_" });
		game.zimo();
		game.dapai("m3*");
		game.zimo();
		game.gang("m4444");
		game.gangzimo();
		EXPECT_FALSE(find(game.yifa(), true));
	}
	// 加槓の場合、即座には開槓されないこと
	{
		auto game = init_game({}, -1, 0, 0, { "__________,s333=", "", "", "" });
		game.zimo();
		game.gang("s333=3");
		game.gangzimo();
		EXPECT_EQ(1, game.model().shan.baopai().size());
	}
	// カンドラ後乗せではない場合、加槓も即座に開槓されること
	{
		Rule rule;
		rule.gangbaopaiPostAddition/*カンドラ後乗せ*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "__________,s333=", "", "", "" });
		game.zimo();
		game.gang("s333=3");
		game.gangzimo();
		EXPECT_EQ(2, game.model().shan.baopai().size());
	}
	// 大明槓の場合、即座には開槓されないこと
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "_", "" });
		game.zimo();
		game.dapai("s3");
		game.fulou("s3333=");
		game.gangzimo();
		EXPECT_EQ(1, game.model().shan.baopai().size());
	}
	// カンドラ後乗せではない場合、大明槓も即座に開槓されること
	{
		Rule rule;
		rule.gangbaopaiPostAddition/*カンドラ後乗せ*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "_", "", "_", "" });
		game.zimo();
		game.dapai("s3");
		game.fulou("s3333=");
		game.gangzimo();
		EXPECT_EQ(2, game.model().shan.baopai().size());
	}
}

TEST(GameTest, kakigang) {
	auto game = init_game({}, -1, 0, 0, { "__________,s555+", "", "", "" });

	// 槓ドラが増えること
	{
		game.zimo();
		game.gang("s555+0");
		game.gangzimo();
		game.kaigang();
		EXPECT_EQ(2, game.model().shan.baopai().size());
		EXPECT_TRUE(game.gang().empty());
	}
	// 通知が伝わること
	EXPECT_EQ("gangzimo", game.status());

	// カンドラなしの場合、開槓しないこと
	{
		Rule rule;
		rule.gangbaopai/*カンドラあり*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "_", "", "", "" });
		game.zimo();
		game.gang("m1111");
		game.gangzimo();
		EXPECT_EQ(1, game.model().shan.baopai().size());
		EXPECT_TRUE(game.gang().empty());
	}
}

TEST(GameTest, hule) {
	auto game = init_game({}, -1, 0, 0, { "_", "", "m123p456s789z1122", "" });

	// 和了が記録されること
	{
		game.zimo();
		game.dapai("z1");
		game.set_hule({ 2 });
		game.hule();
		EXPECT_TRUE(game.defen().hupai.size());
	}
	// 通知が伝わること
	EXPECT_EQ("hule", game.status());

	// 立直・一発
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z1122", "_", "", "" });
		game.set_diyizimo(false);
		game.zimo();
		game.dapai(game.model().shoupai[0].zimo() + "_*");
		game.zimo();
		game.dapai("z1");
		game.set_hule({ 0 });
		game.hule();
		EXPECT_TRUE(find_if(game.defen().hupai, [](const auto& h) { return h.name == Hupai::LIZHI; }));
		EXPECT_TRUE(find_if(game.defen().hupai, [](const auto& h) { return h.name == Hupai::YIPA; }));
	}
	// ダブル立直
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z1122", "_", "", "" });
		game.zimo();
		game.dapai(game.model().shoupai[0].zimo() + "_*");
		game.zimo();
		game.dapai("z1");
		game.set_hule({ 0 });
		game.hule();
		EXPECT_TRUE(find_if(game.defen().hupai, [](const auto& h) { return h.name == Hupai::DABULIZHI; }));
	}
	// 槍槓
	{
		auto game = init_game({}, -1, 0, 0, { "_________m1,m111=", "_", "m23p456s789z11222", "" });
		game.zimo();
		game.gang("m111=1");
		game.set_hule({ 2 });
		game.hule();
		EXPECT_TRUE(find_if(game.defen().hupai, [](const auto& h) { return h.name == Hupai::QIANGGANG; }));
	}
	// 嶺上開花
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s78z11,m111=", "", "", "" }, { "m4" }, { "s9" });
		game.zimo();
		game.gang("m111=1");
		game.gangzimo();
		game.hule();
		EXPECT_TRUE(find_if(game.defen().hupai, [](const auto& h) { return h.name == Hupai::LINGSHANGKAIHUA; }));
	}
	// 最終牌で嶺上開花
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s78z11,m111=", "", "", "" }, { "m4" }, { "s9" });
		game.set_diyizimo(false);
		game.zimo();
		game.gang("m111=1");
		while (game.model().shan.paishu() > 1) game.model().shan.zimo();
		game.gangzimo();
		game.hule();
		EXPECT_TRUE(find_if(game.defen().hupai, [](const auto& h) { return h.name == Hupai::LINGSHANGKAIHUA; }));
		EXPECT_FALSE(find_if(game.defen().hupai, [](const auto& h) { return h.name == Hupai::HAIDIMOYUE; }));
	}
	// 海底摸月
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z1122", "", "", "" }, { "z2" });
		game.set_diyizimo(false);
		game.zimo();
		while (game.model().shan.paishu() > 0) game.model().shan.zimo();
		game.hule();
		EXPECT_TRUE(find_if(game.defen().hupai, [](const auto& h) { return h.name == Hupai::HAIDIMOYUE; }));
	}
	// 河底撈魚
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "m123p456s789z1122", "" });
		game.set_diyizimo(false);
		game.zimo();
		while (game.model().shan.paishu() > 0) game.model().shan.zimo();
		game.dapai("z2");
		game.set_hule({ 2 });
		game.hule();
		EXPECT_TRUE(find_if(game.defen().hupai, [](const auto& h) { return h.name == Hupai::HEDILAOYU; }));
	}
	// 天和
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z1122", "", "", "" }, { "z2" });
		game.zimo();
		game.hule();
		EXPECT_TRUE(find_if(game.defen().hupai, [](const auto& h) { return h.name == Hupai::TIANHE; }));
	}
	// 地和
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m123p456s789z1122", "", "" }, { "m1", "z2" });
		game.zimo();
		game.dapai("m1_");
		game.zimo();
		game.hule();
		EXPECT_TRUE(find_if(game.defen().hupai, [](const auto& h) { return h.name == Hupai::DIHE; }));
	}
	// 槍槓でダブロン
	{
		auto game = init_game({}, -1, 0, 0, { "__________,m111=", "m23p456s789z11122", "m23p789s456z33344", "" });
		game.zimo();
		game.gang("m111=1");
		game.set_hule({1, 2});
		game.hule();
		game.hule();
		EXPECT_TRUE(find_if(game.defen().hupai, [](const auto& h) { return h.name == Hupai::QIANGGANG; }));
	}
	// 子の和了は輪荘
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m123p456s789z1122", "", "" });
		game.zimo();
		game.dapai("z1");
		game.set_hule({ 1 });
		game.hule();
		EXPECT_FALSE(game.lianzhuang());
	}
	// 親の和了は連荘
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z1122", "", "", "" }, { "z1" });
		game.zimo();
		game.hule();
		EXPECT_TRUE(game.lianzhuang());
	}
	// ダブロンは親の和了があれば連荘
	{
		auto game = init_game({}, -1, 0, 0, { "m23p456s789z11122", "", "m23p789s546z33344", "" }, { "m2", "m1" });
		game.zimo();
		game.dapai("m2");
		game.zimo();
		game.dapai("m1");
		game.set_hule({ 2, 0 });
		game.hule();
		game.hule();
		EXPECT_TRUE(game.lianzhuang());
	}
	// 連荘なしの場合は親の和了があっても輪荘
	{
		Rule rule;
		rule.dealerContinuationType/*連荘方式*/ = 0;
		auto game = init_game(rule, -1, 0, 0, { "m123p456s789z1122", "", "", "" }, { "z1" });
		game.zimo();
		game.hule();
		EXPECT_FALSE(game.lianzhuang());
	}
	// 一局戦の場合は親の和了でも輪荘
	{
		Rule rule;
		rule.roundsType/*場数*/ = 0;
		auto game = init_game(rule, -1, 0, 0, { "m123p456s789z1122", "", "", "" }, { "z1" });
		game.zimo();
		game.hule();
		EXPECT_FALSE(game.lianzhuang());
	}
}

TEST(GameTest, pingju) {
	auto game = init_game();

	// 途中流局
	{
		game.pingju("yao9"); // 九種九牌
		EXPECT_TRUE(game.no_game());
		EXPECT_TRUE(game.lianzhuang());
	}
	// 通知が伝わること
	EXPECT_EQ("pingju", game.status());

	// 全員テンパイ
	{
		Rule rule;
		rule.liujumanguan/*流し満貫あり*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m22p12366s406789", "m55p40s123,z111-,p678-", "m67p678s22,s56-7,p444-", "m12345p33s333,m406-" });
		game.pingju();
		EXPECT_EQ("", game.get_pingju().name);
		EXPECT_EQ(4, count_if(game.get_pingju().shoupai, [](const auto& s) { return !s.empty(); }));
		EXPECT_EQ((std::array<int, 4>{}), game.fenpei());
	}
	// 全員ノーテン
	{
		Rule rule;
		rule.liujumanguan/*流し満貫あり*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m40789p4667s8z577", "m99p12306z277,m345-", "m3p1234689z55,s7-89", "m2233467p234555" });
		game.pingju();
		EXPECT_EQ("", game.get_pingju().name);
		EXPECT_EQ(0, count_if(game.get_pingju().shoupai, [](const auto& s) { return !s.empty(); }));
		EXPECT_EQ((std::array<int, 4>{}), game.fenpei());
	}
	// 2人テンパイ
	{
		Rule rule;
		rule.liujumanguan/*流し満貫あり*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m22p12366s406789", "m99p12306z277,m345-", "m3p1234689z55,s7-89", "m12345p33s333,m406-" });
		game.pingju();
		EXPECT_EQ("", game.get_pingju().name);
		EXPECT_EQ(2, count_if(game.get_pingju().shoupai, [](const auto& s) { return !s.empty(); }));
		EXPECT_EQ((std::array<int, 4>{ 1500, -1500, -1500, 1500 }), game.fenpei());
	}
	// 形式テンパイとならない牌姿
	{
		Rule rule;
		rule.liujumanguan/*流し満貫あり*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m123p456s789z1111", "m99p12306z277,m345-", "m3p1234689z55,s7-89", "m12345p33s333,m406-" });
		game.pingju();
		EXPECT_EQ("", game.get_pingju().name);
		EXPECT_EQ(1, count_if(game.get_pingju().shoupai, [](const auto& s) { return !s.empty(); }));
		EXPECT_EQ((std::array<int, 4>{ -1000, -1000, -1000, 3000 }), game.fenpei());
	}
	// ノーテン宣言ありの場合、宣言なしをノーテンとみなすこと
	{
		Rule rule;
		rule.liujumanguan/*流し満貫あり*/ = false;
		rule.notenDeclaration/*ノーテン宣言あり*/ = true;
		auto game = init_game(rule, -1, 0, 0, { "m22p12366s406789", "m55p40s123,z111-,p678-", "m67p678s22,s56-7,p444-", "m12345p33s333,m406-" });
		game.pingju("", { "", "_", "_", "_" });
		EXPECT_EQ("", game.get_pingju().name);
		EXPECT_EQ(3, count_if(game.get_pingju().shoupai, [](const auto& s) { return !s.empty(); }));
		EXPECT_EQ((std::array<int, 4>{ -3000, 1000, 1000, 1000 }), game.fenpei());
	}
	// ノーテン宣言であってもリーチ者の手牌は公開すること
	{
		Rule rule;
		rule.liujumanguan/*流し満貫あり*/ = false;
		rule.notenDeclaration/*ノーテン宣言あり*/ = true;
		auto game = init_game(rule, -1, 0, 0, { "m22p12366s406789*", "", "", "" });
		game.pingju();
		EXPECT_EQ("", game.get_pingju().name);
		EXPECT_FALSE(game.get_pingju().shoupai[0].empty());
	}
	// ノーテン罰なし
	{
		Rule rule;
		rule.liujumanguan/*流し満貫あり*/ = false;
		rule.notenPenalty/*ノーテン罰あり*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m22p12366s406789", "m99p12306z277,m345-", "m3p1234689z55,s7-89", "m12345p33s333,m406-" });
		game.pingju();
		EXPECT_EQ("", game.get_pingju().name);
		EXPECT_EQ(1, count_if(game.get_pingju().shoupai, [](const auto& s) { return !s.empty(); }));
		EXPECT_EQ((std::array<int, 4>{ 0, 0, 0, 0 }), game.fenpei());
	}
	// テンパイ連荘
	{
		Rule rule;
		rule.liujumanguan/*流し満貫あり*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m22p12366s406789", "m99p12306z277,m345-", "m3p1234689z55,s7-89", "m12345p33s333,m406-" });
		game.pingju();
		EXPECT_TRUE(game.lianzhuang());
	}
	// ノーテン親流れ
	{
		Rule rule;
		rule.liujumanguan/*流し満貫あり*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m40789p4667s8z577", "m99p12306z277,m345-", "m3p1234689z55,s7-89", "m12345p33s333,m406-" });
		game.pingju();
		EXPECT_FALSE(game.lianzhuang());
	}
	// 和了連荘の場合、親のテンパイでも輪荘すること
	{
		Rule rule;
		rule.liujumanguan/*流し満貫あり*/ = false;
		rule.dealerContinuationType/*連荘方式*/ = 1;
		auto game = init_game(rule, -1, 0, 0, { "m22p12366s406789", "m99p12306z277,m345-", "m3p1234689z55,s7-89", "m12345p33s333,m406-" });
		game.pingju();
		EXPECT_FALSE(game.lianzhuang());
	}
	// ノーテン連荘の場合、親がノーテンでも連荘すること
	{
		Rule rule;
		rule.liujumanguan/*流し満貫あり*/ = false;
		rule.dealerContinuationType/*連荘方式*/ = 3;
		auto game = init_game(rule, -1, 0, 0, { "m40789p4667s8z577", "m99p12306z277,m345-", "m3p1234689z55,s7-89", "m12345p33s333,m406-" });
		game.pingju();
		EXPECT_TRUE(game.lianzhuang());
	}
	// 一局戦の場合、親がノーテンでも連荘すること
	{
		Rule rule;
		rule.liujumanguan/*流し満貫あり*/ = false;
		rule.roundsType/*場数*/ = 0;
		auto game = init_game(rule, -1, 0, 0, { "m40789p4667s8z577", "m99p12306z277,m345-", "m3p1234689z55,s7-89", "m12345p33s333,m406-" });
		game.pingju();
		EXPECT_TRUE(game.lianzhuang());
	}
	// 流し満貫
	{
		auto game = init_game({}, -1, 0, 0, { "_", "_", "_", "_" });
		game.zimo(); game.dapai("z1");
		game.zimo(); game.dapai("m2");
		game.zimo(); game.dapai("p2");
		game.zimo(); game.dapai("s2");
		game.pingju();
		EXPECT_EQ("nm", game.get_pingju().name);
		EXPECT_EQ((std::array<int, 4>{ 12000, -4000, -4000, -4000 }), game.fenpei());
	}
	// 鳴かれた場合、流し満貫は成立しない
	{
		auto game = init_game({}, -1, 0, 0, { "_", "_", "_", "_" });
		game.zimo(); game.dapai("z1");
		game.fulou("z111-"); game.dapai("m2");
		game.zimo(); game.dapai("p2");
		game.zimo(); game.dapai("s2");
		game.pingju();
		EXPECT_EQ("", game.get_pingju().name);
	}
	// 2人が流し満貫
	{
		auto game = init_game({}, -1, 0, 0, { "_", "_", "_", "_" });
		game.zimo(); game.dapai("z1");
		game.zimo(); game.dapai("m1");
		game.zimo(); game.dapai("p2");
		game.zimo(); game.dapai("s2");
		game.pingju();
		EXPECT_EQ("nm", game.get_pingju().name);
		EXPECT_EQ((std::array<int, 4>{ 8000, 4000, -6000, -6000 }), game.fenpei());
	}
	// ノーテン罰なしのルールの場合、リーチ者と親以外は手牌を開かないこと
	{
		Rule rule;
		rule.liujumanguan/*流し満貫あり*/ = false;
		rule.notenPenalty/*ノーテン罰あり*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m567999s4466777", "m05p123s56z333*,s8888", "m11p789s06,z555-,p406-", "" });
		game.pingju();
		EXPECT_EQ((std::array<std::string, 4>{ "m567999s4466777", "m05p123s56z333*,s8888", "", "" }),
			game.get_pingju().shoupai);
	}
	// ノーテン罰なしで和了連荘のルールの場合、リーチ者以外は手牌を開かないこと
	{
		Rule rule;
		rule.liujumanguan/*流し満貫あり*/ = false;
		rule.notenPenalty/*ノーテン罰あり*/ = false;
		rule.dealerContinuationType/*連荘方式*/ = 1;
		auto game = init_game(rule, -1, 0, 0, { "m567999s4466777", "m05p123s56z333*,s8888", "m11p789s06,z555-,p406-", "" });
		game.pingju();
		EXPECT_EQ((std::array<std::string, 4>{ "", "m05p123s56z333*,s8888", "", "" }),
			game.get_pingju().shoupai);
	}
}

TEST(GameTest, last) {
	// 連荘時に局が進まないこと
	{
		auto game = init_game();
		game.set_lianzhuang(true);
		game.last();
		EXPECT_EQ(0, game.model().zhuangfeng);
		EXPECT_EQ(0, game.model().jushu);
	}
	// 輪荘時に局が進むこと
	{
		auto game = init_game();
		game.model().zhuangfeng = 0;
		game.model().jushu = 3;
		game.last();
		EXPECT_EQ(1, game.model().zhuangfeng);
		EXPECT_EQ(0, game.model().jushu);
	}
	// 東風戦は東四局で終局すること
	{
		Rule rule;
		rule.roundsType/*場数*/ = 1;
		auto game = init_game(rule, -1, 0, 0, {}, {}, {}, {}, { 10000, 20000, 30000, 40000 });
		game.model().zhuangfeng = 0;
		game.model().jushu = 3;
		game.last();
		EXPECT_EQ("jieju", game.status());
	}
	// 東南戦は南四局で終局すること
	{
		Rule rule;
		rule.roundsType/*場数*/ = 2;
		auto game = init_game(rule, -1, 0, 0, {}, {}, {}, {}, { 10000, 20000, 30000, 40000 });
		game.model().zhuangfeng = 1;
		game.model().jushu = 3;
		game.last();
		EXPECT_EQ("jieju", game.status());
	}
	// 一荘戦は北四局で終局すること
	{
		Rule rule;
		rule.roundsType/*場数*/ = 4;
		auto game = init_game(rule, -1, 0, 0, {}, {}, {}, {}, { 10000, 20000, 30000, 40000 });
		game.model().zhuangfeng = 3;
		game.model().jushu = 3;
		game.last();
		EXPECT_EQ("jieju", game.status());
	}
	// 連荘中でもトビ終了すること
	{
		Rule rule;
		rule.roundsType/*場数*/ = 4;
		auto game = init_game(rule, -1, 0, 0, {}, {}, {}, {}, { 50100, 30000, 20000, -100 });
		game.set_lianzhuang(true);
		game.last();
		EXPECT_EQ("jieju", game.status());
	}
	// トビ終了なし
	{
		Rule rule;
		rule.bankruptcyEndAll/*トビ終了あり*/ = false;
		auto game = init_game(rule, -1, 0, 0, {}, {}, {}, {}, { 50100, 30000, 20000, -100 });
		game.last();
		EXPECT_EQ("qipai", game.status());
	}
	// オーラス止め(東風戦)
	{
		Rule rule;
		rule.roundsType/*場数*/ = 1;
		auto game = init_game(rule, -1, 0, 0, {}, {}, {}, {}, { 40000, 30000, 20000, 10000 });
		game.model().zhuangfeng = 0;
		game.model().jushu = 3;
		game.set_lianzhuang(true);
		game.last();
		EXPECT_EQ("jieju", game.status());
	}
	// オーラス止め(東南戦)
	{
		auto game = init_game({}, -1, 0, 0, {}, {}, {}, {}, { 40000, 30000, 20000, 10000 });
		game.model().zhuangfeng = 1;
		game.model().jushu = 3;
		game.set_lianzhuang(true);
		game.last();
		EXPECT_EQ("jieju", game.status());
	}
	// 途中流局ではオーラス止めしないこと
	{
		auto game = init_game({}, -1, 0, 0, {}, {}, {}, {}, { 40000, 30000, 20000, 10000 });
		game.model().zhuangfeng = 1;
		game.model().jushu = 3;
		game.set_lianzhuang(true);
		game.set_no_game(true);
		game.last();
		EXPECT_EQ("qipai", game.status());
	}
	// オーラス止めなし
	{
		Rule rule;
		rule.lastRoundStop/*オーラス止めあり*/ = false;
		auto game = init_game(rule, -1, 0, 0, {}, {}, {}, {}, { 40000, 30000, 20000, 10000 });
		game.model().zhuangfeng = 1;
		game.model().jushu = 3;
		game.set_lianzhuang(true);
		game.last();
		EXPECT_EQ("qipai", game.status());
	}
	// 一荘戦では延長戦がないこと
	{
		Rule rule;
		rule.roundsType/*場数*/ = 4;
		auto game = init_game(rule);
		game.model().zhuangfeng = 3;
		game.model().jushu = 3;
		game.last();
		EXPECT_EQ("jieju", game.status());
	}
	// 延長戦なし
	{
		Rule rule;
		rule.overtimeType/*延長戦方式*/ = 0;
		auto game = init_game(rule);
		game.model().zhuangfeng = 1;
		game.model().jushu = 3;
		game.last();
		EXPECT_EQ("jieju", game.status());
	}
	// 延長戦突入
	{
		auto game = init_game();
		game.model().zhuangfeng = 1;
		game.model().jushu = 3;
		game.last();
		EXPECT_EQ("qipai", game.status());
	}
	// 延長戦サドンデス
	{
		auto game = init_game({}, -1, 0, 0, {}, {}, {}, {}, { 10000, 20000, 30000, 40000 });
		game.model().zhuangfeng = 2;
		game.model().jushu = 0;
		game.last();
		EXPECT_EQ(7, game.max_jushu());
		EXPECT_EQ("jieju", game.status());
	}
	// 連荘優先サドンデス
	{
		Rule rule;
		rule.overtimeType/*延長戦方式*/ = 2;
		auto game = init_game(rule);
		game.model().zhuangfeng = 1;
		game.model().jushu = 3;
		game.last();
		EXPECT_EQ(8, game.max_jushu());
		EXPECT_EQ("qipai", game.status());
	}
	// 4局固定延長戦オーラス止め
	{
		Rule rule;
		rule.overtimeType/*延長戦方式*/ = 3;
		auto game = init_game(rule);
		game.model().zhuangfeng = 1;
		game.model().jushu = 3;
		game.last();
		EXPECT_EQ(11, game.max_jushu());
		EXPECT_EQ("qipai", game.status());
	}
	// 延長戦は最大四局で終了すること
	{
		auto game = init_game();
		game.model().zhuangfeng = 2;
		game.model().jushu = 3;
		game.last();
		EXPECT_EQ("jieju", game.status());
	}
	// 一局戦には延長戦はない
	{
		Rule rule;
		rule.roundsType/*場数*/ = 0;
		auto game = init_game(rule);
		game.model().zhuangfeng = 0;
		game.model().jushu = 0;
		game.last();
		EXPECT_EQ("jieju", game.status());
	}
}

TEST(GameTest, jieju) {
	auto game = init_game({}, 1, 0, 0, {}, {}, {}, {}, { 20400, 28500, 20500, 30600 });

	// 牌譜が記録されること
	{
		game.jieju();
		EXPECT_EQ((std::array<int, 4>{ 30600, 20400, 28500, 20500 }), game.model().defen);
		EXPECT_EQ((std::array<int, 4>{ 1, 4, 2, 3 }), game.rank());
		EXPECT_EQ((std::array<float, 4>{ 40.6f, -29.6f, 8.5f, -19.5f }), game.point());
	}
	// 通知が伝わること
	EXPECT_EQ("jieju", game.status());

	// 同点の場合は起家に近い方を上位とする
	{
		auto game = init_game({}, 2);
		game.jieju();
		EXPECT_EQ((std::array<int, 4>{ 3, 4, 1, 2 }), game.rank());
		EXPECT_EQ((std::array<float, 4>{ -15.0f, - 25.0f, 35.0f, 5.0f }), game.point());
	}
	// 終局時に残った供託リーチ棒はトップの得点に加算
	{
		auto game = init_game({}, 3, 3, 0, {}, {}, {}, {}, { 9000, 19000, 29000, 40000 });
		game.jieju();
		EXPECT_EQ((std::array<int, 4>{ 19000, 29000, 43000, 9000 }), game.model().defen);
	}
	// 1000点未満のポイントは四捨五入する
	{
		Rule rule;
		rule.rankPoints/*順位点*/ = { "20","10","-10","-20" };
		auto game = init_game(rule, 0, 0, 0, {}, {}, {}, {}, { 20400, 28500, 20500, 30600 });
		game.jieju();
		EXPECT_EQ((std::array<float, 4>{ -30, 9, -19, 40 }), game.point());
	}
	// "1000点未満のポイントは四捨五入する(負のケース)
	{
		Rule rule;
		rule.rankPoints/*順位点*/ = { "20","10","-10","-20" };
		auto game = init_game(rule, 0, 0, 0, {}, {}, {}, {}, { -1500, 83800, 6000, 11700 });
		game.jieju();
		EXPECT_EQ((std::array<float, 4>{ -51, 93, -34, -8 }), game.point());
	}
	// 順位点を変更できること
	{
		Rule rule;
		rule.rankPoints/*順位点*/ = { "30","10","-10","-30" };
		auto game = init_game(rule, 2);
		game.jieju();
		EXPECT_EQ((std::array<int, 4>{ 3, 4, 1, 2 }), game.rank());
		EXPECT_EQ((std::array<float, 4>{ -15, -35, 45, 5 }), game.point());
	}
}

TEST(GameTest, reply_kaiju) {
	// 配牌に遷移すること
	{
		Game game;
		game.kaiju();
		game.next();
		EXPECT_EQ("qipai", game.status());
	}
}

TEST(GameTest, reply_qipai) {
	// ツモに遷移すること
	{
		auto game = init_game();
		game.qipai();
		game.next();
		EXPECT_EQ("zimo", game.status());
	}
}

TEST(GameTest, reply_zimo) {
	// 打牌
	{
		auto game = init_game({}, -1, 0, 0, {}, { "m1" });
		game.zimo();
		set_reply(game, 0, "dapai", "m1_");
		game.next();
		EXPECT_EQ("dapai", game.status());
		EXPECT_EQ("m1_", game.dapai());
	}
	// リーチ
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z1122", "", "", ""  }, { "m1" });
		game.zimo();
		set_reply(game, 0, "dapai", "m1_*");
		game.next();
		EXPECT_EQ("dapai", game.status());
		EXPECT_EQ("m1_*", game.dapai());
	}
	// 打牌(不正応答)
	{
		auto game = init_game({}, -1, 0, 0, {}, { "m1" });
		game.zimo();
		set_reply(game, 0, "dapai", "m2_");
		game.next();
		EXPECT_EQ("dapai", game.status());
		EXPECT_NE("m2_", game.dapai());
	}
	// 九種九牌
	{
		auto game = init_game({}, -1, 0, 0, { "m123459z1234567", "", "", "" });
		game.zimo();
		set_reply(game, 0, "daopai", "-");
		game.next();
		EXPECT_EQ("yao9", game.get_pingju().name);
	}
	// 九種九牌(不正応答)
	{
		auto game = init_game({}, -1, 0, 0, { "m234567z1234567", "", "", "" });
		game.zimo();
		set_reply(game, 0, "daopai", "-");
		game.next();
		EXPECT_EQ("dapai", game.status());
	}
	// 途中流局なしの場合は九種九牌にできないこと
	{
		Rule rule;
		rule.abortiveDraw/*途中流局あり*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m123459z1234567", "", "", "" });
		game.zimo();
		set_reply(game, 0, "daopai", "-");
		game.next();
		EXPECT_EQ("dapai", game.status());
	}
	// ツモ和了
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z1122", "", "", "" }, { "z1" });
		game.zimo();
		set_reply(game, 0, "hule", "-");
		game.next();
		EXPECT_EQ("hule", game.status());
	}
	// ツモ和了(不正応答)
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z1122", "", "", "" }, { "z3" });
		game.zimo();
		set_reply(game, 0, "hule", "-");
		game.next();
		EXPECT_EQ("dapai", game.status());
	}
	// カン
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456z1122,s888+", "", "", "" }, { "s8" });
		game.zimo();
		set_reply(game, 0, "gang", "s888+8");
		game.next();
		EXPECT_EQ("gang", game.status());
		EXPECT_EQ("s888+8", game.gang());
	}
	// カン(不正応答)
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456z1122,s888+", "", "", "" }, { "s7" });
		game.zimo();
		set_reply(game, 0, "gang", "s888+8");
		game.next();
		EXPECT_EQ("dapai", game.status());
	}
	// 5つめのカンができないこと
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456z1122,s888+", "", "", "" }, { "s8" });
		game.set_n_gang({ 0, 0, 0, 4 });
		game.zimo();
		set_reply(game, 0, "gang", "s888+8");
		game.next();
		EXPECT_EQ("dapai", game.status());
	}
	// 無応答のときにツモ切りすること
	{
		auto game = init_game({}, -1, 0, 0, {}, { "m1" });
		game.zimo();
		set_reply(game, 0, "gang", "s888+8");
		game.next();
		EXPECT_EQ("dapai", game.status());
		EXPECT_EQ("m1_", game.dapai());
	}
	// 槓ツモ
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "" });
		game.zimo();
		game.gang("m1111");
		game.gangzimo();
		game.next();
		EXPECT_EQ("dapai", game.status());
	}
}

TEST(GameTest, reply_dapai) {
	// 応答なし
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "" });
		game.zimo();
		game.dapai("m1");
		game.next();
		EXPECT_EQ("zimo", game.status());
	}
	// ロン和了
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m123p456s789z1122", "", "" });
		game.zimo();
		game.dapai("z1");
		set_reply(game, 1, "hule", "-");
		game.next();
		EXPECT_EQ("hule", game.status());
		EXPECT_EQ(1, max_index(game.fenpei()));
	}
	// 和了見逃しでフリテンになること
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m123p456s789z1122", "", "" });
		game.zimo();
		game.dapai("z1");
		game.next();
		EXPECT_FALSE(game.neng_rong(1));
	}
	// ダブロン
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m23446p45688s345", "m34s33,s444-,s666+,p406-", "" });
		game.zimo();
		game.dapai("m5*");
		set_reply(game, 1, "hule", "-");
		set_reply(game, 2, "hule", "-");
		game.next();
		EXPECT_EQ(1, max_index(game.fenpei()));
		EXPECT_EQ((std::vector<int>{2}), game.get_hule());
	}
	// ダブロンを頭ハネに変更できること
	{
		Rule rule;
		rule.maxSimultaneousWinners/*最大同時和了数*/ = 1;
		auto game = init_game(rule, -1, 0, 0, { "_", "m23446p45688s345", "m34s33,s444-,s666+,p406-", "" });
		game.zimo();
		game.dapai("m5*");
		set_reply(game, 1, "hule", "-");
		set_reply(game, 2, "hule", "-");
		game.next();
		EXPECT_EQ(1, max_index(game.fenpei()));
		EXPECT_EQ((std::vector<int>{}), game.get_hule());
	}
	// 三家和
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m23446p45688s345", "m34s33,s444-,s666+,p406-", "m23467s88,s222+,z666=" });
		game.zimo();
		game.dapai("m5*");
		set_reply(game, 1, "hule", "-");
		set_reply(game, 2, "hule", "-");
		set_reply(game, 3, "hule", "-");
		game.next();
		EXPECT_EQ("ron3", game.get_pingju().name);
		EXPECT_EQ((std::array<std::string, 4>{ "", "m23446p45688s345", "m34s33,s444-,s666+,p406-", "m23467s88,s222+,z666=" }), game.get_pingju().shoupai);
	}
	// トリロン可に変更できること
	{
		Rule rule;
		rule.maxSimultaneousWinners/*最大同時和了数*/ = 3;
		auto game = init_game(rule, -1, 0, 0, { "_", "m23446p45688s345", "m34s33,s444-,s666+,p406-", "m23467s88,s222+,z666=" });
		game.zimo();
		game.dapai("m5*");
		set_reply(game, 1, "hule", "-");
		set_reply(game, 2, "hule", "-");
		set_reply(game, 3, "hule", "-");
		game.next();
		EXPECT_EQ(1, max_index(game.fenpei()));
		EXPECT_EQ((std::vector<int>{ 2, 3 }), game.get_hule());
	}
	// リーチ成立
	{
		auto game = init_game({}, 0, 0, 0, { "m55688p234567s06", "", "", "" }, { "s7" });
		game.zimo();
		game.dapai("m5*");
		game.next();
		EXPECT_EQ(24000, game.model().defen[0]);
		EXPECT_EQ(1, game.model().lizhibang);
		EXPECT_EQ("zimo", game.status());
	}
	// リーチ不成立
	{
		auto game = init_game({}, 0, 0, 0, { "m55688p234567s06", "m23446p45688s345", "", "" }, { "s7" });
		game.zimo();
		game.dapai("m5*");
		set_reply(game, 1, "hule", "-");
		game.next();
		EXPECT_EQ(25000, game.model().defen[0]);
		EXPECT_EQ(0, game.model().lizhibang);
		EXPECT_EQ("hule", game.status());
	}
	// 四家立直
	{
		auto game = init_game({}, 0, 0, 0, { "m11156p5688s2346", "m2227p11340s2356", "m2346789p345699", "m34056p4678s3456" }, { "p4","s1","m7","s6" });
		for (const auto& p : { "s6*", "m7*", "p6*", "p4*" }) {
			game.zimo();
			game.dapai(p);
		}
		game.next();
		EXPECT_EQ("riichi4", game.get_pingju().name);
		EXPECT_EQ((std::array<std::string, 4>{ "m11156p45688s234*", "m222p11340s12356*", "m23467789p34599*", "m34056p678s34566*" }), game.get_pingju().shoupai);
	}
	// 途中流局なしの場合は四家立直でも続行すること
	{
		Rule rule;
		rule.abortiveDraw/*途中流局あり*/ = false;
		auto game = init_game(rule, 0, 0, 0, { "m11156p5688s2346", "m2227p11340s2356", "m2346789p345699", "m34056p4678s3456" }, { "p4","s1","m7","s6" });
		for (const auto& p : { "s6*", "m7*", "p6*", "p4*" }) {
			game.zimo();
			game.dapai(p);
		}
		game.next();
		EXPECT_EQ("zimo", game.status());
	}
	// 四風連打
	{
		auto game = init_game({}, -1, 0, 0, { "_", "_", "_", "_" });
		for (int l = 0; l < 4; l++) {
			game.zimo();
			game.dapai("z1");
		}
		game.next();
		EXPECT_EQ("kaze4", game.get_pingju().name);
		EXPECT_EQ((std::array<std::string, 4>{ "", "", "", "" }), game.get_pingju().shoupai);
	}
	// 途中流局なしの場合は四風連打とならず、第一ツモ巡が終了すること
	{
		Rule rule;
		rule.abortiveDraw/*途中流局あり*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "_", "_", "_", "_" });
		for (int l = 0; l < 4; l++) {
			game.zimo();
			game.dapai("z1");
		}
		game.next();
		EXPECT_FALSE(game.diyizimo());
		EXPECT_EQ("zimo", game.status());
	}
	 // 四開槓
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m111p22279s57,s333=", "m123p456s222789z2", "" }, { "m1" }, { "p2","s3","s2","z7" });
		game.zimo();
		game.dapai("m1_");
		game.fulou("m1111-");
		game.gangzimo();
		game.gang("p2222");
		game.gangzimo();
		game.gang("s333=3");
		game.gangzimo();
		game.dapai("s2");
		game.fulou("s2222-");
		game.gangzimo();
		game.dapai("z7_");
		game.next();
		EXPECT_EQ("kan4", game.get_pingju().name);
		EXPECT_EQ((std::array<std::string, 4>{ "", "", "", "" }), game.get_pingju().shoupai);
	}
	// 1人で四開槓
	{
		auto game = init_game({}, -1, 0, 0, { "m1112,p111+,s111=,z111-", "", "", "" }, { "m1" }, { "p1","s1","z1","z7" });
		game.zimo();
		game.gang("m1111");
		game.gangzimo();
		game.gang("p111+1");
		game.gangzimo();
		game.gang("s111=1");
		game.gangzimo();
		game.gang("z111-1");
		game.gangzimo();
		game.dapai("z7");
		game.next();
		EXPECT_EQ("zimo", game.status());
	}
	// 途中流局なしでは四開槓とならない
	{
		Rule rule;
		rule.abortiveDraw/*途中流局あり*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "_", "m111p22279s57,s333=", "m123p456s222789z2", "" }, { "m1" }, { "p2","s3","s2","z7" });
		game.zimo();
		game.dapai("m1_");
		game.fulou("m1111-");
		game.gangzimo();
		game.gang("p2222");
		game.gangzimo();
		game.gang("s333=3");
		game.gangzimo();
		game.dapai("s2");
		game.fulou("s2222-");
		game.gangzimo();
		game.dapai("z7_");
		game.next();
		EXPECT_EQ("zimo", game.status());
	}
	// 流局
	{
		Rule rule;
		rule.liujumanguan/*流し満貫あり*/ = false;
		rule.notenDeclaration/*ノーテン宣言あり*/ = true;
		auto game = init_game(rule, -1, 0, 0, { "_", "m222p11340s12356", "m23467789p34599", "_" });
		game.zimo();
		while (game.model().shan.paishu()) game.model().shan.zimo();
		game.dapai(game.model().shoupai[0].get_dapai()[0]);
		set_reply(game, 1, "daopai", "-");
		set_reply(game, 2, "daopai", "-");
		game.next();
		EXPECT_EQ("", game.get_pingju().name);
		EXPECT_EQ((std::array<std::string, 4>{ "", "m222p11340s12356", "m23467789p34599", "" }), game.get_pingju().shoupai);
	}
	// カン
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "m111234p567s3378" });
		game.zimo();
		game.dapai("m1");
		set_reply(game, 3, "fulou", "m1111+");
		game.next();
		EXPECT_EQ("fulou", game.status());
		EXPECT_EQ("m1111+", game.fulou());
	}
	// カン(不正応答)
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "m111234p567s3378" });
		game.zimo();
		game.dapai("m2");
		set_reply(game, 3, "fulou", "m1111+");
		game.next();
		EXPECT_EQ("zimo", game.status());
	}
	// 5つめのカンができないこと
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "m111234p567s3378" });
		game.set_n_gang({ 4, 0, 0, 0 });
		game.zimo();
		game.dapai("m2");
		set_reply(game, 3, "fulou", "m1111+");
		game.next();
		EXPECT_EQ("zimo", game.status());
	}
	// ポン
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "m112345p567s3378", "" });
		game.set_n_gang({ 4, 0, 0, 0 });
		game.zimo();
		game.dapai("m1");
		set_reply(game, 2, "fulou", "m111=");
		game.next();
		EXPECT_EQ("fulou", game.status());
		EXPECT_EQ("m111=", game.fulou());
	}
	// ポン(不正応答)
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "m112345p567s3378", "" });
		game.zimo();
		game.dapai("m2");
		set_reply(game, 2, "fulou", "m111=");
		game.next();
		EXPECT_EQ("zimo", game.status());
	}
	// チー
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m112345p567s3378", "", "" });
		game.zimo();
		game.dapai("m6");
		set_reply(game, 1, "fulou", "m456-");
		game.next();
		EXPECT_EQ("fulou", game.status());
		EXPECT_EQ("m456-", game.fulou());
	}
	// チー(不正応答)
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m112345p567s3378", "", "" });
		game.zimo();
		game.dapai("m5");
		set_reply(game, 1, "fulou", "m456-");
		game.next();
		EXPECT_EQ("zimo", game.status());
	}
	// ポンとチーの競合はポンを優先
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m23567p456s889z11", "m11789p123s11289", "" });
		game.zimo();
		game.dapai("m1");
		set_reply(game, 1, "fulou", "m1-23");
		set_reply(game, 2, "fulou", "m111=");
		game.next();
		EXPECT_EQ("fulou", game.status());
		EXPECT_EQ("m111=", game.fulou());
	}
}

TEST(GameTest, reply_fulou) {
	// 大明槓
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m1112356p456s889", "", "" });
		game.zimo();
		game.dapai("m1");
		game.fulou("m1111-");
		game.next();
		EXPECT_EQ("gangzimo", game.status());
	}
	// チー・ポン → 打牌
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m23567p456s889z11", "", "" });
		game.zimo();
		game.dapai("m1");
		game.fulou("m1-23");
		set_reply(game, 1, "dapai", "s9");
		game.next();
		EXPECT_EQ("dapai", game.status());
		EXPECT_EQ("s9", game.dapai());
	}
	// チー・ポン → 打牌(不正応答)
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m23456p456s889z11", "", "" });
		game.zimo();
		game.dapai("m1");
		game.fulou("m1-23");
		set_reply(game, 1, "dapai", "m4");
		game.next();
		EXPECT_EQ("dapai", game.status());
		EXPECT_EQ("z1", game.dapai());
	}
	// 無応答のときに右端の牌を切ること
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m23567p456s889z11", "", "" });
		game.zimo();
		game.dapai("m1");
		game.fulou("m1-23");
		game.next();
		EXPECT_EQ("dapai", game.status());
		EXPECT_EQ("z1", game.dapai());
	}
}

TEST(GameTest, reply_gang) {
	// 応答なし
	{
		auto game = init_game({}, -1, 0, 0, { "m45p456s11789,m111+", "", "", "" }, { "m1" });
		game.zimo();
		game.gang("m111+1");
		game.next();
		EXPECT_EQ("gangzimo", game.status());
	}
	// ロン和了(槍槓)
	{
		auto game = init_game({}, -1, 0, 0, { "m45p456s11789,m111+", "", "", "m23456p123s67899" }, { "m1" });
		game.zimo();
		game.gang("m111+1");
		set_reply(game, 3, "hule", "-");
		game.next();
		EXPECT_EQ("hule", game.status());
		EXPECT_EQ(3, max_index(game.fenpei()));
	}
	// ロン和了(不正応答)
	{
		auto game = init_game({}, -1, 0, 0, { "m45p456s11789,m111+", "", "", "m33456p123s67899" }, { "m1" });
		game.zimo();
		game.gang("m111+1");
		set_reply(game, 3, "hule", "-");
		game.next();
		EXPECT_EQ("gangzimo", game.status());
	}
	// 暗槓は槍槓できない
	{
		auto game = init_game({}, -1, 0, 0, { "m11145p456s11789", "", "", "m23456p123s67899" }, { "m1" });
		game.zimo();
		game.gang("m1111");
		set_reply(game, 3, "hule", "-");
		game.next();
		EXPECT_EQ("gangzimo", game.status());
	}
	// 和了見逃しでフリテンになること
	{
		auto game = init_game({}, -1, 0, 0, { "m45p456s11789,m111+", "", "", "m23456p123s67899" }, { "m1" });
		game.zimo();
		game.gang("m111+1");
		game.next();
		EXPECT_FALSE(game.neng_rong(3));
	}
	// ダブロン
	{
		auto game = init_game({}, -1, 0, 0, { "m11p222s88,z666=,m505-", "m23446p45688s345", "m34s33,s444-,s666+,p406-", "" }, { "m5" });
		game.zimo();
		game.gang("m505-5");
		set_reply(game, 1, "hule", "-");
		set_reply(game, 2, "hule", "-");
		game.next();
		EXPECT_EQ("hule", game.status());
		EXPECT_EQ(1, max_index(game.fenpei()));
		EXPECT_EQ((std::vector<int>{2}), game.get_hule());
	}
	// ダブロンを頭ハネに変更できること
	{
		Rule rule;
		rule.maxSimultaneousWinners/*最大同時和了数*/ = 1;
		auto game = init_game(rule, -1, 0, 0, { "m11p222s88,z666=,m505-", "m23446p45688s345", "m34s33,s444-,s666+,p406-", "" }, { "m5" });
		game.zimo();
		game.gang("m505-5");
		set_reply(game, 1, "hule", "-");
		set_reply(game, 2, "hule", "-");
		game.next();
		EXPECT_EQ("hule", game.status());
		EXPECT_EQ(1, max_index(game.fenpei()));
		EXPECT_EQ((std::vector<int>{}), game.get_hule());
	}
}

TEST(GameTest, reply_hule) {
	// 親のツモ和了
	{
		auto game = init_game({}, 0, 1, 1, { "m345567p111s3368", "", "", "" }, { "s7" }, {}, { "p2" }, { 25000, 25000, 25000, 24000 });
		game.set_diyizimo(false);
		game.zimo();
		game.set_hule({ 0 });
		game.hule();
		game.next();
		EXPECT_EQ((std::array<int, 4>{ 28400, 24200, 24200, 23200 }), game.model().defen);
		EXPECT_EQ(2, game.model().changbang);
		EXPECT_EQ(0, game.model().lizhibang);
		EXPECT_EQ("qipai", game.status());
	}
	// 子のロン和了
	{
		auto game = init_game({}, 0, 1, 1, { "_", "m345567p111s66z11", "", "" }, { "s7" }, {}, { "p2" }, { 25000, 25000, 25000, 24000 });
		game.zimo();
		game.dapai("z1");
		game.set_hule({ 1 });
		game.hule();
		game.next();
		EXPECT_EQ((std::array<int, 4>{ 23100, 27900, 25000, 24000 }), game.model().defen);
		EXPECT_EQ(0, game.model().changbang);
		EXPECT_EQ(0, game.model().lizhibang);
		EXPECT_EQ("qipai", game.status());
	}
	// ダブロンで連荘
	{
		auto game = init_game({}, 0, 1, 1, { "m23p456s789z11122", "_", "m23p789s546z33344", "" }, { "m2", "m1"}, {}, {"s9"}, { 25000, 25000, 25000, 24000 });
		game.zimo();
		game.dapai("m2");
		game.zimo();
		game.dapai("m1");
		game.set_hule({ 2, 0 });
		game.hule();
		game.next();
		EXPECT_EQ((std::array<int, 4>{ 25000, 23400, 27600, 24000 }), game.model().defen);
		EXPECT_EQ(0, game.model().changbang);
		EXPECT_EQ(0, game.model().lizhibang);
		EXPECT_EQ("hule", game.status());
		game.next();
		EXPECT_EQ((std::array<int, 4>{ 28900, 19500, 27600, 24000 }), game.model().defen);
		EXPECT_EQ(2, game.model().changbang);
		EXPECT_EQ(0, game.model().lizhibang);
		EXPECT_EQ("qipai", game.status());
	}
}

TEST(GameTest, reply_pingju) {
	// 流局
	{
		auto game = init_game({}, 0, 1, 1, { "m123p456s789z1122", "_", "_", "_" }, { "m2", "m3", "m4", "m5" }, {}, {}, { 25000, 25000, 25000, 24000 });
		for (const auto& p : { "m2", "m3", "m4", "m5" }) {
			game.zimo();
			game.dapai(p);
		}
		game.pingju();
		game.next();
		EXPECT_EQ((std::array<int, 4>{ 28000, 24000, 24000, 23000}), game.model().defen);
		EXPECT_EQ(2, game.model().changbang);
		EXPECT_EQ(1, game.model().lizhibang);
		EXPECT_EQ("qipai", game.status());
	}
}

TEST(GameTest, get_dapai) {
	// 現在の手番の可能な打牌を返すこと
	{
		auto game = init_game({}, -1, 0, 0, { "m123,z111+,z222=,z333-", "", "", "" }, { "m1" });
		game.zimo();
		EXPECT_EQ((std::vector<std::string>{ "m1", "m2", "m3", "m1_" }), game.get_dapai());
	}
	// 現物喰い替えありに変更できること
	{
		Rule rule;
		rule.canChangePermissionLevel/*喰い替え許可レベル*/ = 2;
		auto game = init_game(rule, -1, 0, 0, { "_", "m1234p567,z111=,s789-", "", "" });
		game.zimo();
		game.dapai("m1");
		game.fulou("m1-23");
		EXPECT_EQ((std::vector<std::string>{ "m1", "m4", "p5", "p6", "p7" }), game.get_dapai());
	}
}

TEST(GameTest, get_chi_mianzi) {
	// 現在打たれた牌でチーできる面子を返すこと
	{
		auto game = init_game({}, -1, 0, 0, { "", "_", "m1234p456s789z111", "" });
		game.zimo();
		game.dapai(game.get_dapai()[0]);
		game.zimo();
		game.dapai("m2");
		EXPECT_EQ((std::vector<std::string>{ "m12-3", "m2-34" }), game.get_chi_mianzi(2));
	}
	// 自身はチーできないこと
	{
		auto game = init_game({}, -1, 0, 0, { "m1234p456s789z111", "", "", "" });
		game.zimo();
		game.dapai(game.get_dapai().back());
		EXPECT_THROW(game.get_chi_mianzi(0), std::invalid_argument);
	}
	// 対面はチーできないこと
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "m1234p456s789z111", "" });
		game.zimo();
		game.dapai("m2");
		EXPECT_EQ((std::vector<std::string>{}), game.get_chi_mianzi(2));
	}
	// ハイテイ牌はチーできないこと
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m1234p456s789z111", "", "" });
		game.zimo();
		while (game.model().shan.paishu()) game.model().shan.zimo();
		game.dapai("m2");
		EXPECT_EQ((std::vector<std::string>{}), game.get_chi_mianzi(1));
	}
	// 現物喰い替えありに変更できること
	{
		Rule rule;
		rule.canChangePermissionLevel/*喰い替え許可レベル*/ = 2;
		auto game = init_game(rule, -1, 0, 0, { "_", "m1123,p456-,z111=,s789-", "", "" });
		game.zimo();
		game.dapai("m1");
		EXPECT_EQ((std::vector<std::string>{ "m1-23" }), game.get_chi_mianzi(1));
	}
}

TEST(GameTest, get_peng_mianzi) {
	// 現在打たれた牌でポンできる面子を返すこと
	{
		auto game = init_game({}, -1, 0, 0, { "", "_", "", "m1123p456s789z111" });
		game.zimo();
		game.dapai(game.get_dapai()[0]);
		game.zimo();
		game.dapai("m1");
		EXPECT_EQ((std::vector<std::string>{ "m111=" }), game.get_peng_mianzi(3));
	}
	// 自身はポンできないこと
	{
		auto game = init_game({}, -1, 0, 0, { "m1123p456s789z111", "", "", "" });
		game.zimo();
		game.dapai(game.get_dapai().back());
		EXPECT_THROW(game.get_peng_mianzi(0), std::invalid_argument);
	}
	// ハイテイ牌はポンできないこと
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m1123p456s789z111", "", "" });
		game.zimo();
		while (game.model().shan.paishu()) game.model().shan.zimo();
		game.dapai("m1");
		EXPECT_EQ((std::vector<std::string>{}), game.get_peng_mianzi(2));
	}
}

TEST(GameTest, get_gang_mianzi) {
	// 現在打たれた牌で大明槓できる面子を返すこと
	{
		auto game = init_game({}, -1, 0, 0, { "", "_", "", "m1112p456s789z111" });
		game.zimo();
		game.dapai(game.get_dapai()[0]);
		game.zimo();
		game.dapai("m1");
		EXPECT_EQ((std::vector<std::string>{ "m1111=" }), game.get_gang_mianzi(3));
	}
	// 自身は大明槓できないこと
	{
		auto game = init_game({}, -1, 0, 0, { "m1112p456s789z111", "", "", "" });
		game.zimo();
		game.dapai(game.get_dapai().back());
		EXPECT_THROW(game.get_gang_mianzi(0), std::invalid_argument);
	}
	// ハイテイ牌は大明槓できないこと
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m1112p456s789z111", "", "" });
		game.zimo();
		while (game.model().shan.paishu()) game.model().shan.zimo();
		game.dapai("m1");
		EXPECT_EQ((std::vector<std::string>{}), game.get_gang_mianzi(2));
	}
	// 現在の手番が暗槓もしくは加槓できる面子を返すこと
	{
		auto game = init_game({}, -1, 0, 0, { "m1111p4569s78,z111=", "", "", "" }, {"z1"});
		game.zimo();
		EXPECT_EQ((std::vector<std::string>{ "m1111", "z111=1" }), game.get_gang_mianzi());
	}
	// ハイテイ牌は暗槓もしくは加槓できないこと
	{
		auto game = init_game({}, -1, 0, 0, { "m1111p4567s78,z111=", "", "", "" }, { "z1" });
		game.zimo();
		while (game.model().shan.paishu()) game.model().shan.zimo();
		game.dapai("m1");
		EXPECT_EQ((std::vector<std::string>{}), game.get_gang_mianzi(2));
	}
	// リーチ後の暗槓なしに変更できること
	{
		Rule rule;
		rule.lizhiPostClosedGangPermissionLevel/*リーチ後暗槓許可レベル*/ = 0;
		auto game = init_game(rule, -1, 0, 0, { "m111p456s789z1122*", "", "", "" }, { "m1" });
		game.zimo();
		EXPECT_EQ((std::vector<std::string>{}), game.get_gang_mianzi());
	}
}

TEST(GameTest, allow_lizhi) {
	// 指定された打牌でリーチ可能な場合、真を返す
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z1123", "", "", "" }, { "z2" });
		game.zimo();
		EXPECT_TRUE(game.allow_lizhi("z3*"));
	}
	// ツモ番がない場合、リーチできない
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z1123", "", "", "" }, { "z2" });
		game.zimo();
		while (game.model().shan.paishu() >= 4) game.model().shan.zimo();
		EXPECT_FALSE(game.allow_lizhi("z3*"));
	}
	// 持ち点が1000点に満たない場合、リーチできない
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z1123", "", "", "" }, { "z2" }, {}, {}, { 900, 19100, 45000, 35000 });
		game.zimo();
		EXPECT_FALSE(game.allow_lizhi("z3*"));
	}
	// ツモ番なしでもリーチできるように変更できること
	{
		Rule rule;
		rule.lizhiWithoutTsumoBonus/*ツモ番なしリーチあり*/ = true;
		auto game = init_game(rule, -1, 0, 0, { "m123p456s789z1123", "", "", "" }, { "z2" });
		game.zimo();
		while (game.model().shan.paishu() >= 4) game.model().shan.zimo();
		EXPECT_TRUE(game.allow_lizhi("z3*"));
	}
	// 持ち点が1000点に満たなくてもリーチできるように変更できること
	{
		Rule rule;
		rule.bankruptcyEndAll/*トビ終了あり*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m123p456s789z1123", "", "", "" }, { "z2" }, {}, {}, { 900, 19100, 45000, 35000 });
		game.zimo();
		EXPECT_TRUE(game.allow_lizhi("z3*"));
	}
}

TEST(GameTest, allow_hule) {
	// ツモ和了
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z3344", "", "", "" }, { "z4" });
		game.zimo();
		EXPECT_TRUE(game.allow_hule());
	}
	// リーチ・ツモ
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z4*,z333=", "", "", "" }, { "z4" });
		game.zimo();
		EXPECT_TRUE(game.allow_hule());
	}
	// 嶺上開花
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "m123p456s789z3334", "" }, {}, { "z4" });
		game.zimo();
		game.dapai("z3");
		game.fulou("z3333=");
		game.gangzimo();
		EXPECT_TRUE(game.allow_hule());
	}
	// ハイテイ・ツモ
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z4,z333=", "", "", "" }, { "z4" });
		game.zimo();
		while (game.model().shan.paishu()) game.model().shan.zimo();
		EXPECT_TRUE(game.allow_hule());
	}
	// 場風のみ・ツモ
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m123p456s789z4,z111=", "", "" }, { "m1", "z4" });
		game.zimo();
		game.dapai("m1");
		game.zimo();
		EXPECT_TRUE(game.allow_hule());
	}
	// 自風のみ・ツモ
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m123p456s789z4,z222=", "", "" }, { "m1", "z4" });
		game.zimo();
		game.dapai("m1");
		game.zimo();
		EXPECT_TRUE(game.allow_hule());
	}
	// リーチ・ロン
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m123p456s789z3334*", "", "" });
		game.zimo();
		game.dapai("z4");
		EXPECT_TRUE(game.allow_hule(1));
	}
	// 槍槓
	{
		auto game = init_game({}, -1, 0, 0, { "m1234p567s789,m111=", "", "m23p123567s12377", "" });
		game.zimo();
		game.gang("m111=1");
		EXPECT_TRUE(game.allow_hule(2));
	}
	// ハイテイ・ロン
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "m123p456s789z4,z333=" });
		game.zimo();
		while (game.model().shan.paishu()) game.model().shan.zimo();
		game.dapai("z4");
		EXPECT_TRUE(game.allow_hule(3));
	}
	// 場風のみ・ロン
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m123p456s789z4,z111=", "", "" });
		game.zimo();
		game.dapai("z4");
		EXPECT_TRUE(game.allow_hule(1));
	}
	// 自風のみ・ロン
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m123p456s789z4,z222=", "", "" });
		game.zimo();
		game.dapai("z4");
		EXPECT_TRUE(game.allow_hule(1));
	}
	// フリテンはロン和了できないこと
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z3344", "", "", "" }, { "z4", "z3" });
		game.zimo();
		game.dapai("z4");
		game.zimo();
		game.dapai("z3");
		EXPECT_FALSE(game.allow_hule(0));
	}
	// クイタンなしにできること
	{
		Rule rule;
		rule.canduan/*クイタンあり*/ = false;
		auto game = init_game({}, -1, 0, 0, { "_", "m234p567s2244,m888-", "", "" });
		game.zimo();
		game.dapai("s4");
		EXPECT_TRUE(game.allow_hule(1));
	}
}

TEST(GameTest, allow_pingju) {
	// 九種九牌
	{
		auto game = init_game({}, -1, 0, 0, { "m123459z1234567", "", "", "" });
		game.zimo();
		EXPECT_TRUE(game.allow_pingju());
	}
	// 第一ツモ以降は九種九牌にならない
	{
		auto game = init_game({}, -1, 0, 0, { "_", "_", "m123459z1234567", "" });
		game.zimo();
		game.dapai("s2");
		game.fulou("s2-34");
		game.dapai("z3");
		game.zimo();
		EXPECT_FALSE(game.allow_pingju());
	}
	// 途中流局なしの場合は九種九牌にできない
	{
		Rule rule;
		rule.abortiveDraw/*途中流局あり*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m123459z1234567", "", "", "" });
		game.zimo();
		EXPECT_FALSE(game.allow_pingju());
	}
}

TEST(GameTest, static_get_dapai) {
	Shoupai shoupai("m5678p567,z111=,s789-");
	shoupai.fulou("m0-67");

	// 喰い替えなし
	{
		Rule rule;
		rule.canChangePermissionLevel/*喰い替え許可レベル*/ = 0;
		EXPECT_EQ((std::vector<std::string>{ "p5", "p6", "p7" }), Game::get_dapai(rule, shoupai));
	}
	// 現物以外の喰い替えあり
	{
		Rule rule;
		rule.canChangePermissionLevel/*喰い替え許可レベル*/ = 1;
		EXPECT_EQ((std::vector<std::string>{ "m8", "p5", "p6", "p7" }), Game::get_dapai(rule, shoupai));
	}
	// 現物喰い替えもあり
	{
		Rule rule;
		rule.canChangePermissionLevel/*喰い替え許可レベル*/ = 2;
		EXPECT_EQ((std::vector<std::string>{ "m5", "m8", "p5", "p6", "p7" }), Game::get_dapai(rule, shoupai));
	}
}

TEST(GameTest, static_get_chi_mianzi) {
	Shoupai shoupai1("m1234,p456-,z111=,s789-");
	Shoupai shoupai2("m1123,p456-,z111=,s789-");

	// 喰い替えなし
	{
		Rule rule;
		rule.canChangePermissionLevel/*喰い替え許可レベル*/ = 0;
		EXPECT_EQ((std::vector<std::string>{}), Game::get_chi_mianzi(rule, shoupai1, "m1-", 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_chi_mianzi(rule, shoupai2, "m1-", 1));
	}
	// 現物以外の喰い替えあり
	{
		Rule rule;
		rule.canChangePermissionLevel/*喰い替え許可レベル*/ = 1;
		EXPECT_EQ((std::vector<std::string>{ "m1-23" }), Game::get_chi_mianzi(rule, shoupai1, "m1-", 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_chi_mianzi(rule, shoupai2, "m1-", 1));
	}
	// 現物喰い替えもあり
	{
		Rule rule;
		rule.canChangePermissionLevel/*喰い替え許可レベル*/ = 2;
		EXPECT_EQ((std::vector<std::string>{ "m1-23" }), Game::get_chi_mianzi(rule, shoupai1, "m1-", 1));
		EXPECT_EQ((std::vector<std::string>{ "m1-23" }), Game::get_chi_mianzi(rule, shoupai2, "m1-", 1));
	}
	// ハイテイは鳴けない
	{
		Rule rule;
		rule.canChangePermissionLevel/*喰い替え許可レベル*/ = 2;
		EXPECT_EQ((std::vector<std::string>{}), Game::get_chi_mianzi(rule, shoupai1, "m1-", 0));
	}
	// ツモした状態でチーできない
	{
		EXPECT_EQ((std::vector<std::string>{}), Game::get_chi_mianzi({}, shoupai1, "s3-", 1));
	}
}

TEST(GameTest, static_get_peng_mianzi) {
	Shoupai shoupai("m1112,p456-,z111=,s789-");

	// 喰い替えのためにポンできないケースはない
	{
		Rule rule;
		rule.canChangePermissionLevel/*喰い替え許可レベル*/ = 0;
		EXPECT_EQ((std::vector<std::string>{ "m111+" }), Game::get_peng_mianzi(rule, shoupai, "m1+", 1));
	}
	// ハイテイは鳴けない
	{
		Rule rule;
		rule.canChangePermissionLevel/*喰い替え許可レベル*/ = 0;
		EXPECT_EQ((std::vector<std::string>{}), Game::get_peng_mianzi(rule, shoupai, "m1+", 0));
	}
	// ツモした状態でポンできない
	{
		EXPECT_THROW(Game::get_peng_mianzi({}, Shoupai("m123p456s11789z123"), "s1-", 1), std::runtime_error);
	}
}

TEST(GameTest, static_get_gang_mianzi) {
	Shoupai shoupai1("m1112p456s789z111z1*");
	Shoupai shoupai2("m1112p456s789z111m1*");
	Shoupai shoupai3("m23p567s33345666s3*");
	Shoupai shoupai4("s1113445678999s1*");
	Shoupai shoupai5("m23s77789s7*,s5550,z6666");

	// リーチ後の暗槓なし
	{
		Rule rule;
		rule.lizhiPostClosedGangPermissionLevel/*リーチ後暗槓許可レベル*/ = 0;
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai1, {}, 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai2, {}, 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai3, {}, 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai4, {}, 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai5, {}, 1));
	}
	// リーチ後の牌姿の変わる暗槓なし
	{
		Rule rule;
		rule.lizhiPostClosedGangPermissionLevel/*リーチ後暗槓許可レベル*/ = 1;
		EXPECT_EQ((std::vector<std::string>{ "z1111" }), Game::get_gang_mianzi(rule, shoupai1, {}, 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai2, {}, 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai3, {}, 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai4, {}, 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai5, {}, 1));
	}
	// リーチ後の待ちの変わる暗槓なし
	{
		Rule rule;
		rule.lizhiPostClosedGangPermissionLevel/*リーチ後暗槓許可レベル*/ = 2;
		EXPECT_EQ((std::vector<std::string>{ "z1111" }), Game::get_gang_mianzi(rule, shoupai1, {}, 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai2, {}, 1));
		EXPECT_EQ((std::vector<std::string>{ "s3333" }), Game::get_gang_mianzi(rule, shoupai3, {}, 1));
		EXPECT_EQ((std::vector<std::string>{ "s1111" }), Game::get_gang_mianzi(rule, shoupai4, {}, 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai5, {}, 1));
	}
	// ハイテイはカンできない
	{
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi({}, Shoupai("m1112p456s789z111z1"), {}, 0));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi({}, Shoupai("m1112p456s789z111"), "z1=", 0));
	}
}

TEST(GameTest, static_allow_lizhi) {
	// 打牌できない場合、リーチはできない
	{
		EXPECT_FALSE(Game::allow_lizhi({}, Shoupai("m123p456s789z1122")).first);
	}
	// すでにリーチしている場合、リーチはできない
	{
		EXPECT_FALSE(Game::allow_lizhi({}, Shoupai("m123p456s789z11223*")).first);
	}
	// メンゼンでない場合、リーチはできない
	{
		EXPECT_FALSE(Game::allow_lizhi({}, Shoupai("m123p456s789z23,z111=")).first);
	}
	// ツモ番がない場合、リーチはできない
	{
		EXPECT_FALSE(Game::allow_lizhi({}, Shoupai("m123p456s789z11223"), "z3", 3));
	}
	// ルールが許せばツモ番がなくてもリーチは可能
	{
		Rule rule;
		rule.lizhiWithoutTsumoBonus/*ツモ番なしリーチあり*/ = true;
		EXPECT_TRUE(Game::allow_lizhi(rule, Shoupai("m123p456s789z11223"), "z3", 3));
	}
	// 持ち点が1000点に満たない場合、リーチはできない
	{
		EXPECT_FALSE(Game::allow_lizhi({}, Shoupai("m123p456s789z11223"), "z3", 3, 900));
	}
	// トビなしなら持ち点が1000点に満たなくてもリーチは可能
	{
		Rule rule;
		rule.bankruptcyEndAll/*トビ終了あり*/ = false;
		EXPECT_FALSE(Game::allow_lizhi(rule, Shoupai("m123p456s789z11223"), "z3", 3, 900));
	}
	// テンパイしていない場合、リーチはできない
	{
		EXPECT_FALSE(Game::allow_lizhi({}, Shoupai("m123p456s789z11234")).first);
	}
	// 形式テンパイと認められない牌姿でリーチはできない
	{
		EXPECT_FALSE(Game::allow_lizhi({}, Shoupai("m123p456s789z11112"), "z2"));
	}
	// 指定された打牌でリーチ可能な場合、真を返すこと
	{
		EXPECT_TRUE(Game::allow_lizhi({}, Shoupai("m123p456s789z11112"), "z1"));
	}
	// 打牌が指定されていない場合、リーチ可能な打牌一覧を返す
	{
		EXPECT_EQ((std::vector<std::string>{ "s7", "s8" }), Game::allow_lizhi({}, Shoupai("m123p456s788z11122")).second);
		EXPECT_EQ((std::vector<std::string>{ "z3_" }), Game::allow_lizhi({}, Shoupai("m123p456s789z11223")).second);
	}
	// リーチ可能な打牌がない場合、false を返す
	{
		EXPECT_FALSE(Game::allow_lizhi({}, Shoupai("m11112344449999")).first);
	}
}

TEST(GameTest, static_allow_hule) {
	// フリテンの場合、ロン和了できない
	{
		EXPECT_FALSE(Game::allow_hule({}, Shoupai("m123p456z1122,s789-"), "z1=", 0, 1, false, false));
	}
	// 和了形になっていない場合、和了できない
	{
		EXPECT_FALSE(Game::allow_hule({}, Shoupai("m123p456z11223,s789-"), {}, 0, 1, false, true));
	}
	// 役あり和了形の場合、和了できる
	{
		EXPECT_TRUE(Game::allow_hule({}, Shoupai("m123p456s789z3377"), "z3+", 0, 1, true, true));
	}
	// 役なし和了形の場合、和了できない
	{
		EXPECT_FALSE(Game::allow_hule({}, Shoupai("m123p456s789z3377"), "z3+", 0, 1, false, true));
	}
	// クイタンなしの場合、クイタンでは和了できない
	{
		Rule rule;
		rule.canduan/*クイタンあり*/ = false;
		EXPECT_FALSE(Game::allow_hule(rule, Shoupai("m22555p234s78,p777-"), "s6=", 0, 1, false, true));
	}
	// ツモ和了
	{
		EXPECT_TRUE(Game::allow_hule({}, Shoupai("m123p456s789z33377"), {}, 0, 1, false, false));
	}
	// ロン和了
	{
		EXPECT_TRUE(Game::allow_hule({}, Shoupai("m123p456z1122,s789-"), "z1=", 0, 1, false, true));
	}
}

TEST(GameTest, static_allow_pingju) {
	// 第一巡でない場合、九種九牌とならない
	{
		EXPECT_FALSE(Game::allow_pingju({}, Shoupai("m1234569z1234567"), false));
	}
	// ツモ後でない場合、九種九牌とならない
	{
		EXPECT_FALSE(Game::allow_pingju({}, Shoupai("m123459z1234567"), true));
	}
	// 途中流局なし場合、九種九牌とならない
	{
		Rule rule;
		rule.abortiveDraw/*途中流局あり*/ = false;
		EXPECT_FALSE(Game::allow_pingju(rule, Shoupai("m1234569z1234567"), true));
	}
	// 八種九牌は流局にできない
	{
		EXPECT_FALSE(Game::allow_pingju({}, Shoupai("m1234567z1234567"), true));
	}
	// 九種九牌
	{
		EXPECT_TRUE(Game::allow_pingju({}, Shoupai("m1234569z1234567"), true));
	}
}

TEST(GameTest, static_allow_no_daopai) {
	Rule rule;
	rule.notenDeclaration/*ノーテン宣言あり*/ = true;

	// 最終打牌以外はノーテン宣言できない
	{
		EXPECT_FALSE(Game::allow_no_daopai(rule, Shoupai("m123p456z1122,s789-"), 1));
		EXPECT_FALSE(Game::allow_no_daopai(rule, Shoupai("m123p456z1122,s789-").zimo("z3"), 0));
	}
	// ノーテン宣言ありのルールでない場合、ノーテン宣言できない
	{
		EXPECT_FALSE(Game::allow_no_daopai({}, Shoupai("m123p456z1122,s789-"), 0));
	}
	// リーチしている場合、ノーテン宣言できない
	{
		EXPECT_FALSE(Game::allow_no_daopai(rule, Shoupai("m123p456p789z1122*"), 0));
	}
	// テンパイしていない場合、ノーテン宣言できない
	{
		EXPECT_FALSE(Game::allow_no_daopai(rule, Shoupai("m123p456p789z1123"), 0));
	}
	// 形式テンパイと認められない牌姿の場合、ノーテン宣言できない
	{
		EXPECT_FALSE(Game::allow_no_daopai(rule, Shoupai("m123p456p789z1111"), 0));
	}
	// ノーテン宣言
	{
		EXPECT_TRUE(Game::allow_no_daopai(rule, Shoupai("m123p456z1122,s789-"), 0));
	}
}
