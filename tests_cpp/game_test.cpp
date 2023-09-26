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

	// �N�Ƃ��ݒ肳��邱��
	{
		game.kaiju(0);
		EXPECT_EQ(game.qijia(), 0);
	}
	// �ʒm���`��邱��
	EXPECT_EQ("kaiju", game.status());
	// �N�Ƃ𗐐��Őݒ�ł��邱��
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

	// �v�R����������邱��
	{
		game.qipai();
		const auto& shan = game.shan();
		EXPECT_EQ(70, shan.paishu());
		EXPECT_EQ(1, shan.baopai().size());
		EXPECT_TRUE(shan.fubaopai().empty());
	}
	// �z�v����邱��
	{
		std::regex regex(R"([mpsz])");
		for (int l = 0; l < 4; l++) {
			EXPECT_EQ(13, std::regex_replace(game.shoupai(l).toString(), regex, "").size());
		}
	}
	// �͂�����������邱��
	{
		for (int l = 0; l < 4; l++) {
			EXPECT_EQ(0, game.he(l).pai().size());
		}
	}
	// ���c�����ł��邱��
	EXPECT_TRUE(game.diyizimo());
	// �l���A�Œ��ł��邱��
	EXPECT_TRUE(game.fengpai());
	// �ʒm���`��邱��
	EXPECT_EQ("qipai", game.status());

	// �g�p����v�R���w��ł��邱��
	{
		auto game = init_game();
		Shan shan(game.rule());
		std::vector<std::string> qipai;
		std::copy(shan.pai().end() - 13, shan.pai().end(), std::back_inserter(qipai));
		Shoupai shoupai(qipai);
		game.qipai(shan);
		EXPECT_EQ(shoupai.toString(), game.model().shoupai[0].toString());
	}
	// �r�����ǂȂ��̏ꍇ�A�ŏ�����l���A�Œ��łȂ�����
	{
		Rule rule;
		rule.abortiveDraw/*�r�����ǂ���*/ = false;
		auto game = init_game(rule);
		game.qipai();
		EXPECT_FALSE(game.fengpai());
	}
}

TEST(GameTest, zimo) {
	auto game = init_game();

	// ��Ԃ��X�V����邱��
	{
		game.zimo();
		EXPECT_EQ(0, game.model().lunban);
	}
	// �v�R����c�����邱��
	EXPECT_EQ(69, game.model().shan.paishu());
	// ��v�Ƀc���v���������邱��
	EXPECT_TRUE(game.model().shoupai[0].get_dapai().size());
	// �ʒm���`��邱��
	EXPECT_EQ("zimo", game.status());
}

TEST(GameTest, dapai) {
	auto game = init_game();
	std::string dapai;

	// ��v����Ŕv���؂�o����邱��
	{
		game.zimo();
		dapai = game.model().shoupai[0].get_dapai()[0];
		game.dapai(dapai);
		EXPECT_THROW(game.model().shoupai[0].get_dapai(), std::runtime_error);
	}
	// �͂ɑŔv����邱��
	EXPECT_EQ(dapai, game.model().he[0].pai()[0]);
	// �ʒm���`��邱��
	EXPECT_EQ("dapai", game.status());

	// ���v�ȊO�̑Ŕv�Ŏl���A�Œ��łȂ��Ȃ邱��
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "" });
		EXPECT_TRUE(game.fengpai());
		game.zimo();
		game.dapai("m1");
		EXPECT_FALSE(game.fengpai());
	}
	// �قȂ镗�v�̑Ŕv�Ŏl���A�Œ��łȂ��Ȃ邱��
	{
		auto game = init_game({}, -1, 0, 0, { "_", "_", "", "" });
		EXPECT_TRUE(game.fengpai());
		game.zimo();
		game.dapai("z1");
		game.zimo();
		game.dapai("z2");
		EXPECT_FALSE(game.fengpai());
	}
	// ���c�����I���Ŏl���A�Œ��łȂ��Ȃ邱��
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
	// �_�u�����[�`
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "" });
		game.zimo();
		game.dapai("m1*");
		EXPECT_EQ(2, game.lizhi(game.model().lunban));
		EXPECT_TRUE(game.yifa(game.model().lunban));
	}
	// ���[�`��̑Ŕv�ňꔭ�̌�������������
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "" });
		game.set_yifa(0, true);
		game.zimo();
		game.dapai("m1");
		EXPECT_FALSE(game.yifa(game.model().lunban));
	}
	// �e���p�C���ɘa���v���͂ɂ���ꍇ�A�t���e���ƂȂ邱��
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z11122", "", "", "" });
		game.model().lunban = 0;
		EXPECT_TRUE(game.neng_rong(game.model().lunban));
		game.dapai("m1");
		EXPECT_FALSE(game.neng_rong(game.model().lunban));
	}
	// ���[�`��̓t���e������������Ȃ�����
	{
		auto game = init_game({}, -1, 0, 0, { "_____________*", "", "", "" });
		game.set_neng_rong(0, false);
		game.zimo();
		auto dapai = game.model().shoupai[0].zimo();
		game.dapai(dapai);
		EXPECT_FALSE(game.neng_rong(game.model().lunban));
	}
	// ���Ȍ�̑Ŕv�ŊJ�Ȃ���邱��
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

	// �͂��畛�I�v���E���邱��
	{
		game.zimo();
		game.dapai("m2_");
		game.fulou("m12-3");
		EXPECT_EQ("m2_-", game.model().he[0].pai()[0]);
	}
	// ��Ԃ��X�V����邱��(��Ƃ���̃`�[)
	EXPECT_EQ(1, game.model().lunban);
	// ��v�����I����邱��
	EXPECT_EQ("m12-3", game.model().shoupai[1].fulou()[0]);
	// �ʒm���`��邱��
	EXPECT_EQ("fulou", game.status());

	// �喾�Ȃ����I����邱��
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "_" });
		game.zimo();
		game.dapai("m2");
		game.fulou("m2222+");
		EXPECT_EQ("m2222+", game.model().shoupai[3].fulou()[0]);
	}
	// ���c�����łȂ��Ȃ邱��
	{
		auto game = init_game({}, -1, 0, 0, { "_", "_", "", "_" });
		game.zimo();
		game.dapai("m3");
		game.fulou("m123-");
		EXPECT_FALSE(game.diyizimo());
	}
	// �ꔭ���Ȃ��Ȃ邱��
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

	// ���Ȃ����I����邱��
	{
		game.zimo();
		game.gang("s555+0");
		EXPECT_EQ("s555+0", game.model().shoupai[0].fulou()[0]);
	}
	// �ʒm���`��邱��
	EXPECT_EQ("gang", game.status());

	// �ÞȂ����I����邱��
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "" });
		game.zimo();
		game.gang("s5550");
		EXPECT_EQ("s5550", game.model().shoupai[0].fulou()[0]);
	}
	// ��悹�̞Ȃ��J�Ȃ���邱��
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

	// �v�R����c�����邱��
	{
		game.zimo();
		game.gang("m5550");
		game.gangzimo();
		EXPECT_EQ(68, game.model().shan.paishu());
	}
	// ��v�Ƀc���v���������邱��
	EXPECT_TRUE(game.model().shoupai[0].get_dapai().size());
	// �ʒm���`��邱��
	EXPECT_EQ("gangzimo", game.status());

	// ���c�����łȂ��Ȃ邱��
	{
		auto game = init_game({}, -1, 0, 0, { "_", "_", "", "_" });
		game.zimo();
		game.gang("m3333");
		game.gangzimo();
		EXPECT_FALSE(game.diyizimo());
	}
	// �ꔭ���Ȃ��Ȃ邱��
	{
		auto game = init_game({}, -1, 0, 0, { "_", "_", "", "_" });
		game.zimo();
		game.dapai("m3*");
		game.zimo();
		game.gang("m4444");
		game.gangzimo();
		EXPECT_FALSE(find(game.yifa(), true));
	}
	// ���Ȃ̏ꍇ�A�����ɂ͊J�Ȃ���Ȃ�����
	{
		auto game = init_game({}, -1, 0, 0, { "__________,s333=", "", "", "" });
		game.zimo();
		game.gang("s333=3");
		game.gangzimo();
		EXPECT_EQ(1, game.model().shan.baopai().size());
	}
	// �J���h����悹�ł͂Ȃ��ꍇ�A���Ȃ������ɊJ�Ȃ���邱��
	{
		Rule rule;
		rule.gangbaopaiPostAddition/*�J���h����悹*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "__________,s333=", "", "", "" });
		game.zimo();
		game.gang("s333=3");
		game.gangzimo();
		EXPECT_EQ(2, game.model().shan.baopai().size());
	}
	// �喾�Ȃ̏ꍇ�A�����ɂ͊J�Ȃ���Ȃ�����
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "_", "" });
		game.zimo();
		game.dapai("s3");
		game.fulou("s3333=");
		game.gangzimo();
		EXPECT_EQ(1, game.model().shan.baopai().size());
	}
	// �J���h����悹�ł͂Ȃ��ꍇ�A�喾�Ȃ������ɊJ�Ȃ���邱��
	{
		Rule rule;
		rule.gangbaopaiPostAddition/*�J���h����悹*/ = false;
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

	// �ȃh���������邱��
	{
		game.zimo();
		game.gang("s555+0");
		game.gangzimo();
		game.kaigang();
		EXPECT_EQ(2, game.model().shan.baopai().size());
		EXPECT_TRUE(game.gang().empty());
	}
	// �ʒm���`��邱��
	EXPECT_EQ("gangzimo", game.status());

	// �J���h���Ȃ��̏ꍇ�A�J�Ȃ��Ȃ�����
	{
		Rule rule;
		rule.gangbaopai/*�J���h������*/ = false;
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

	// �a�����L�^����邱��
	{
		game.zimo();
		game.dapai("z1");
		game.set_hule({ 2 });
		game.hule();
		EXPECT_TRUE(game.defen().hupai.size());
	}
	// �ʒm���`��邱��
	EXPECT_EQ("hule", game.status());

	// �����E�ꔭ
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
	// �_�u������
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
	// ����
	{
		auto game = init_game({}, -1, 0, 0, { "_________m1,m111=", "_", "m23p456s789z11222", "" });
		game.zimo();
		game.gang("m111=1");
		game.set_hule({ 2 });
		game.hule();
		EXPECT_TRUE(find_if(game.defen().hupai, [](const auto& h) { return h.name == Hupai::QIANGGANG; }));
	}
	// ���J��
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s78z11,m111=", "", "", "" }, { "m4" }, { "s9" });
		game.zimo();
		game.gang("m111=1");
		game.gangzimo();
		game.hule();
		EXPECT_TRUE(find_if(game.defen().hupai, [](const auto& h) { return h.name == Hupai::LINGSHANGKAIHUA; }));
	}
	// �ŏI�v�ŗ��J��
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
	// �C��̌�
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z1122", "", "", "" }, { "z2" });
		game.set_diyizimo(false);
		game.zimo();
		while (game.model().shan.paishu() > 0) game.model().shan.zimo();
		game.hule();
		EXPECT_TRUE(find_if(game.defen().hupai, [](const auto& h) { return h.name == Hupai::HAIDIMOYUE; }));
	}
	// �͒ꝝ��
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
	// �V�a
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z1122", "", "", "" }, { "z2" });
		game.zimo();
		game.hule();
		EXPECT_TRUE(find_if(game.defen().hupai, [](const auto& h) { return h.name == Hupai::TIANHE; }));
	}
	// �n�a
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m123p456s789z1122", "", "" }, { "m1", "z2" });
		game.zimo();
		game.dapai("m1_");
		game.zimo();
		game.hule();
		EXPECT_TRUE(find_if(game.defen().hupai, [](const auto& h) { return h.name == Hupai::DIHE; }));
	}
	// ���ȂŃ_�u����
	{
		auto game = init_game({}, -1, 0, 0, { "__________,m111=", "m23p456s789z11122", "m23p789s456z33344", "" });
		game.zimo();
		game.gang("m111=1");
		game.set_hule({1, 2});
		game.hule();
		game.hule();
		EXPECT_TRUE(find_if(game.defen().hupai, [](const auto& h) { return h.name == Hupai::QIANGGANG; }));
	}
	// �q�̘a���֑͗�
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m123p456s789z1122", "", "" });
		game.zimo();
		game.dapai("z1");
		game.set_hule({ 1 });
		game.hule();
		EXPECT_FALSE(game.lianzhuang());
	}
	// �e�̘a���͘A��
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z1122", "", "", "" }, { "z1" });
		game.zimo();
		game.hule();
		EXPECT_TRUE(game.lianzhuang());
	}
	// �_�u�����͐e�̘a��������ΘA��
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
	// �A���Ȃ��̏ꍇ�͐e�̘a���������Ă��֑�
	{
		Rule rule;
		rule.dealerContinuationType/*�A������*/ = 0;
		auto game = init_game(rule, -1, 0, 0, { "m123p456s789z1122", "", "", "" }, { "z1" });
		game.zimo();
		game.hule();
		EXPECT_FALSE(game.lianzhuang());
	}
	// ��ǐ�̏ꍇ�͐e�̘a���ł��֑�
	{
		Rule rule;
		rule.roundsType/*�ꐔ*/ = 0;
		auto game = init_game(rule, -1, 0, 0, { "m123p456s789z1122", "", "", "" }, { "z1" });
		game.zimo();
		game.hule();
		EXPECT_FALSE(game.lianzhuang());
	}
}

TEST(GameTest, pingju) {
	auto game = init_game();

	// �r������
	{
		game.pingju("yao9"); // ����v
		EXPECT_TRUE(game.no_game());
		EXPECT_TRUE(game.lianzhuang());
	}
	// �ʒm���`��邱��
	EXPECT_EQ("pingju", game.status());

	// �S���e���p�C
	{
		Rule rule;
		rule.liujumanguan/*�������т���*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m22p12366s406789", "m55p40s123,z111-,p678-", "m67p678s22,s56-7,p444-", "m12345p33s333,m406-" });
		game.pingju();
		EXPECT_EQ("", game.get_pingju().name);
		EXPECT_EQ(4, count_if(game.get_pingju().shoupai, [](const auto& s) { return !s.empty(); }));
		EXPECT_EQ((std::array<int, 4>{}), game.fenpei());
	}
	// �S���m�[�e��
	{
		Rule rule;
		rule.liujumanguan/*�������т���*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m40789p4667s8z577", "m99p12306z277,m345-", "m3p1234689z55,s7-89", "m2233467p234555" });
		game.pingju();
		EXPECT_EQ("", game.get_pingju().name);
		EXPECT_EQ(0, count_if(game.get_pingju().shoupai, [](const auto& s) { return !s.empty(); }));
		EXPECT_EQ((std::array<int, 4>{}), game.fenpei());
	}
	// 2�l�e���p�C
	{
		Rule rule;
		rule.liujumanguan/*�������т���*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m22p12366s406789", "m99p12306z277,m345-", "m3p1234689z55,s7-89", "m12345p33s333,m406-" });
		game.pingju();
		EXPECT_EQ("", game.get_pingju().name);
		EXPECT_EQ(2, count_if(game.get_pingju().shoupai, [](const auto& s) { return !s.empty(); }));
		EXPECT_EQ((std::array<int, 4>{ 1500, -1500, -1500, 1500 }), game.fenpei());
	}
	// �`���e���p�C�ƂȂ�Ȃ��v�p
	{
		Rule rule;
		rule.liujumanguan/*�������т���*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m123p456s789z1111", "m99p12306z277,m345-", "m3p1234689z55,s7-89", "m12345p33s333,m406-" });
		game.pingju();
		EXPECT_EQ("", game.get_pingju().name);
		EXPECT_EQ(1, count_if(game.get_pingju().shoupai, [](const auto& s) { return !s.empty(); }));
		EXPECT_EQ((std::array<int, 4>{ -1000, -1000, -1000, 3000 }), game.fenpei());
	}
	// �m�[�e���錾����̏ꍇ�A�錾�Ȃ����m�[�e���Ƃ݂Ȃ�����
	{
		Rule rule;
		rule.liujumanguan/*�������т���*/ = false;
		rule.notenDeclaration/*�m�[�e���錾����*/ = true;
		auto game = init_game(rule, -1, 0, 0, { "m22p12366s406789", "m55p40s123,z111-,p678-", "m67p678s22,s56-7,p444-", "m12345p33s333,m406-" });
		game.pingju("", { "", "_", "_", "_" });
		EXPECT_EQ("", game.get_pingju().name);
		EXPECT_EQ(3, count_if(game.get_pingju().shoupai, [](const auto& s) { return !s.empty(); }));
		EXPECT_EQ((std::array<int, 4>{ -3000, 1000, 1000, 1000 }), game.fenpei());
	}
	// �m�[�e���錾�ł����Ă����[�`�҂̎�v�͌��J���邱��
	{
		Rule rule;
		rule.liujumanguan/*�������т���*/ = false;
		rule.notenDeclaration/*�m�[�e���錾����*/ = true;
		auto game = init_game(rule, -1, 0, 0, { "m22p12366s406789*", "", "", "" });
		game.pingju();
		EXPECT_EQ("", game.get_pingju().name);
		EXPECT_FALSE(game.get_pingju().shoupai[0].empty());
	}
	// �m�[�e�����Ȃ�
	{
		Rule rule;
		rule.liujumanguan/*�������т���*/ = false;
		rule.notenPenalty/*�m�[�e��������*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m22p12366s406789", "m99p12306z277,m345-", "m3p1234689z55,s7-89", "m12345p33s333,m406-" });
		game.pingju();
		EXPECT_EQ("", game.get_pingju().name);
		EXPECT_EQ(1, count_if(game.get_pingju().shoupai, [](const auto& s) { return !s.empty(); }));
		EXPECT_EQ((std::array<int, 4>{ 0, 0, 0, 0 }), game.fenpei());
	}
	// �e���p�C�A��
	{
		Rule rule;
		rule.liujumanguan/*�������т���*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m22p12366s406789", "m99p12306z277,m345-", "m3p1234689z55,s7-89", "m12345p33s333,m406-" });
		game.pingju();
		EXPECT_TRUE(game.lianzhuang());
	}
	// �m�[�e���e����
	{
		Rule rule;
		rule.liujumanguan/*�������т���*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m40789p4667s8z577", "m99p12306z277,m345-", "m3p1234689z55,s7-89", "m12345p33s333,m406-" });
		game.pingju();
		EXPECT_FALSE(game.lianzhuang());
	}
	// �a���A���̏ꍇ�A�e�̃e���p�C�ł��֑����邱��
	{
		Rule rule;
		rule.liujumanguan/*�������т���*/ = false;
		rule.dealerContinuationType/*�A������*/ = 1;
		auto game = init_game(rule, -1, 0, 0, { "m22p12366s406789", "m99p12306z277,m345-", "m3p1234689z55,s7-89", "m12345p33s333,m406-" });
		game.pingju();
		EXPECT_FALSE(game.lianzhuang());
	}
	// �m�[�e���A���̏ꍇ�A�e���m�[�e���ł��A�����邱��
	{
		Rule rule;
		rule.liujumanguan/*�������т���*/ = false;
		rule.dealerContinuationType/*�A������*/ = 3;
		auto game = init_game(rule, -1, 0, 0, { "m40789p4667s8z577", "m99p12306z277,m345-", "m3p1234689z55,s7-89", "m12345p33s333,m406-" });
		game.pingju();
		EXPECT_TRUE(game.lianzhuang());
	}
	// ��ǐ�̏ꍇ�A�e���m�[�e���ł��A�����邱��
	{
		Rule rule;
		rule.liujumanguan/*�������т���*/ = false;
		rule.roundsType/*�ꐔ*/ = 0;
		auto game = init_game(rule, -1, 0, 0, { "m40789p4667s8z577", "m99p12306z277,m345-", "m3p1234689z55,s7-89", "m12345p33s333,m406-" });
		game.pingju();
		EXPECT_TRUE(game.lianzhuang());
	}
	// ��������
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
	// ���ꂽ�ꍇ�A�������т͐������Ȃ�
	{
		auto game = init_game({}, -1, 0, 0, { "_", "_", "_", "_" });
		game.zimo(); game.dapai("z1");
		game.fulou("z111-"); game.dapai("m2");
		game.zimo(); game.dapai("p2");
		game.zimo(); game.dapai("s2");
		game.pingju();
		EXPECT_EQ("", game.get_pingju().name);
	}
	// 2�l����������
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
	// �m�[�e�����Ȃ��̃��[���̏ꍇ�A���[�`�҂Ɛe�ȊO�͎�v���J���Ȃ�����
	{
		Rule rule;
		rule.liujumanguan/*�������т���*/ = false;
		rule.notenPenalty/*�m�[�e��������*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m567999s4466777", "m05p123s56z333*,s8888", "m11p789s06,z555-,p406-", "" });
		game.pingju();
		EXPECT_EQ((std::array<std::string, 4>{ "m567999s4466777", "m05p123s56z333*,s8888", "", "" }),
			game.get_pingju().shoupai);
	}
	// �m�[�e�����Ȃ��Řa���A���̃��[���̏ꍇ�A���[�`�҈ȊO�͎�v���J���Ȃ�����
	{
		Rule rule;
		rule.liujumanguan/*�������т���*/ = false;
		rule.notenPenalty/*�m�[�e��������*/ = false;
		rule.dealerContinuationType/*�A������*/ = 1;
		auto game = init_game(rule, -1, 0, 0, { "m567999s4466777", "m05p123s56z333*,s8888", "m11p789s06,z555-,p406-", "" });
		game.pingju();
		EXPECT_EQ((std::array<std::string, 4>{ "", "m05p123s56z333*,s8888", "", "" }),
			game.get_pingju().shoupai);
	}
}

TEST(GameTest, last) {
	// �A�����ɋǂ��i�܂Ȃ�����
	{
		auto game = init_game();
		game.set_lianzhuang(true);
		game.last();
		EXPECT_EQ(0, game.model().zhuangfeng);
		EXPECT_EQ(0, game.model().jushu);
	}
	// �֑����ɋǂ��i�ނ���
	{
		auto game = init_game();
		game.model().zhuangfeng = 0;
		game.model().jushu = 3;
		game.last();
		EXPECT_EQ(1, game.model().zhuangfeng);
		EXPECT_EQ(0, game.model().jushu);
	}
	// ������͓��l�ǂŏI�ǂ��邱��
	{
		Rule rule;
		rule.roundsType/*�ꐔ*/ = 1;
		auto game = init_game(rule, -1, 0, 0, {}, {}, {}, {}, { 10000, 20000, 30000, 40000 });
		game.model().zhuangfeng = 0;
		game.model().jushu = 3;
		game.last();
		EXPECT_EQ("jieju", game.status());
	}
	// �����͓�l�ǂŏI�ǂ��邱��
	{
		Rule rule;
		rule.roundsType/*�ꐔ*/ = 2;
		auto game = init_game(rule, -1, 0, 0, {}, {}, {}, {}, { 10000, 20000, 30000, 40000 });
		game.model().zhuangfeng = 1;
		game.model().jushu = 3;
		game.last();
		EXPECT_EQ("jieju", game.status());
	}
	// �ꑑ��͖k�l�ǂŏI�ǂ��邱��
	{
		Rule rule;
		rule.roundsType/*�ꐔ*/ = 4;
		auto game = init_game(rule, -1, 0, 0, {}, {}, {}, {}, { 10000, 20000, 30000, 40000 });
		game.model().zhuangfeng = 3;
		game.model().jushu = 3;
		game.last();
		EXPECT_EQ("jieju", game.status());
	}
	// �A�����ł��g�r�I�����邱��
	{
		Rule rule;
		rule.roundsType/*�ꐔ*/ = 4;
		auto game = init_game(rule, -1, 0, 0, {}, {}, {}, {}, { 50100, 30000, 20000, -100 });
		game.set_lianzhuang(true);
		game.last();
		EXPECT_EQ("jieju", game.status());
	}
	// �g�r�I���Ȃ�
	{
		Rule rule;
		rule.bankruptcyEndAll/*�g�r�I������*/ = false;
		auto game = init_game(rule, -1, 0, 0, {}, {}, {}, {}, { 50100, 30000, 20000, -100 });
		game.last();
		EXPECT_EQ("qipai", game.status());
	}
	// �I�[���X�~��(������)
	{
		Rule rule;
		rule.roundsType/*�ꐔ*/ = 1;
		auto game = init_game(rule, -1, 0, 0, {}, {}, {}, {}, { 40000, 30000, 20000, 10000 });
		game.model().zhuangfeng = 0;
		game.model().jushu = 3;
		game.set_lianzhuang(true);
		game.last();
		EXPECT_EQ("jieju", game.status());
	}
	// �I�[���X�~��(�����)
	{
		auto game = init_game({}, -1, 0, 0, {}, {}, {}, {}, { 40000, 30000, 20000, 10000 });
		game.model().zhuangfeng = 1;
		game.model().jushu = 3;
		game.set_lianzhuang(true);
		game.last();
		EXPECT_EQ("jieju", game.status());
	}
	// �r�����ǂł̓I�[���X�~�߂��Ȃ�����
	{
		auto game = init_game({}, -1, 0, 0, {}, {}, {}, {}, { 40000, 30000, 20000, 10000 });
		game.model().zhuangfeng = 1;
		game.model().jushu = 3;
		game.set_lianzhuang(true);
		game.set_no_game(true);
		game.last();
		EXPECT_EQ("qipai", game.status());
	}
	// �I�[���X�~�߂Ȃ�
	{
		Rule rule;
		rule.lastRoundStop/*�I�[���X�~�߂���*/ = false;
		auto game = init_game(rule, -1, 0, 0, {}, {}, {}, {}, { 40000, 30000, 20000, 10000 });
		game.model().zhuangfeng = 1;
		game.model().jushu = 3;
		game.set_lianzhuang(true);
		game.last();
		EXPECT_EQ("qipai", game.status());
	}
	// �ꑑ��ł͉����킪�Ȃ�����
	{
		Rule rule;
		rule.roundsType/*�ꐔ*/ = 4;
		auto game = init_game(rule);
		game.model().zhuangfeng = 3;
		game.model().jushu = 3;
		game.last();
		EXPECT_EQ("jieju", game.status());
	}
	// ������Ȃ�
	{
		Rule rule;
		rule.overtimeType/*���������*/ = 0;
		auto game = init_game(rule);
		game.model().zhuangfeng = 1;
		game.model().jushu = 3;
		game.last();
		EXPECT_EQ("jieju", game.status());
	}
	// ������˓�
	{
		auto game = init_game();
		game.model().zhuangfeng = 1;
		game.model().jushu = 3;
		game.last();
		EXPECT_EQ("qipai", game.status());
	}
	// ������T�h���f�X
	{
		auto game = init_game({}, -1, 0, 0, {}, {}, {}, {}, { 10000, 20000, 30000, 40000 });
		game.model().zhuangfeng = 2;
		game.model().jushu = 0;
		game.last();
		EXPECT_EQ(7, game.max_jushu());
		EXPECT_EQ("jieju", game.status());
	}
	// �A���D��T�h���f�X
	{
		Rule rule;
		rule.overtimeType/*���������*/ = 2;
		auto game = init_game(rule);
		game.model().zhuangfeng = 1;
		game.model().jushu = 3;
		game.last();
		EXPECT_EQ(8, game.max_jushu());
		EXPECT_EQ("qipai", game.status());
	}
	// 4�ǌŒ艄����I�[���X�~��
	{
		Rule rule;
		rule.overtimeType/*���������*/ = 3;
		auto game = init_game(rule);
		game.model().zhuangfeng = 1;
		game.model().jushu = 3;
		game.last();
		EXPECT_EQ(11, game.max_jushu());
		EXPECT_EQ("qipai", game.status());
	}
	// ������͍ő�l�ǂŏI�����邱��
	{
		auto game = init_game();
		game.model().zhuangfeng = 2;
		game.model().jushu = 3;
		game.last();
		EXPECT_EQ("jieju", game.status());
	}
	// ��ǐ�ɂ͉�����͂Ȃ�
	{
		Rule rule;
		rule.roundsType/*�ꐔ*/ = 0;
		auto game = init_game(rule);
		game.model().zhuangfeng = 0;
		game.model().jushu = 0;
		game.last();
		EXPECT_EQ("jieju", game.status());
	}
}

TEST(GameTest, jieju) {
	auto game = init_game({}, 1, 0, 0, {}, {}, {}, {}, { 20400, 28500, 20500, 30600 });

	// �v�����L�^����邱��
	{
		game.jieju();
		EXPECT_EQ((std::array<int, 4>{ 30600, 20400, 28500, 20500 }), game.model().defen);
		EXPECT_EQ((std::array<int, 4>{ 1, 4, 2, 3 }), game.rank());
		EXPECT_EQ((std::array<float, 4>{ 40.6f, -29.6f, 8.5f, -19.5f }), game.point());
	}
	// �ʒm���`��邱��
	EXPECT_EQ("jieju", game.status());

	// ���_�̏ꍇ�͋N�Ƃɋ߂�������ʂƂ���
	{
		auto game = init_game({}, 2);
		game.jieju();
		EXPECT_EQ((std::array<int, 4>{ 3, 4, 1, 2 }), game.rank());
		EXPECT_EQ((std::array<float, 4>{ -15.0f, - 25.0f, 35.0f, 5.0f }), game.point());
	}
	// �I�ǎ��Ɏc�����������[�`�_�̓g�b�v�̓��_�ɉ��Z
	{
		auto game = init_game({}, 3, 3, 0, {}, {}, {}, {}, { 9000, 19000, 29000, 40000 });
		game.jieju();
		EXPECT_EQ((std::array<int, 4>{ 19000, 29000, 43000, 9000 }), game.model().defen);
	}
	// 1000�_�����̃|�C���g�͎l�̌ܓ�����
	{
		Rule rule;
		rule.rankPoints/*���ʓ_*/ = { "20","10","-10","-20" };
		auto game = init_game(rule, 0, 0, 0, {}, {}, {}, {}, { 20400, 28500, 20500, 30600 });
		game.jieju();
		EXPECT_EQ((std::array<float, 4>{ -30, 9, -19, 40 }), game.point());
	}
	// "1000�_�����̃|�C���g�͎l�̌ܓ�����(���̃P�[�X)
	{
		Rule rule;
		rule.rankPoints/*���ʓ_*/ = { "20","10","-10","-20" };
		auto game = init_game(rule, 0, 0, 0, {}, {}, {}, {}, { -1500, 83800, 6000, 11700 });
		game.jieju();
		EXPECT_EQ((std::array<float, 4>{ -51, 93, -34, -8 }), game.point());
	}
	// ���ʓ_��ύX�ł��邱��
	{
		Rule rule;
		rule.rankPoints/*���ʓ_*/ = { "30","10","-10","-30" };
		auto game = init_game(rule, 2);
		game.jieju();
		EXPECT_EQ((std::array<int, 4>{ 3, 4, 1, 2 }), game.rank());
		EXPECT_EQ((std::array<float, 4>{ -15, -35, 45, 5 }), game.point());
	}
}

TEST(GameTest, reply_kaiju) {
	// �z�v�ɑJ�ڂ��邱��
	{
		Game game;
		game.kaiju();
		game.next();
		EXPECT_EQ("qipai", game.status());
	}
}

TEST(GameTest, reply_qipai) {
	// �c���ɑJ�ڂ��邱��
	{
		auto game = init_game();
		game.qipai();
		game.next();
		EXPECT_EQ("zimo", game.status());
	}
}

TEST(GameTest, reply_zimo) {
	// �Ŕv
	{
		auto game = init_game({}, -1, 0, 0, {}, { "m1" });
		game.zimo();
		set_reply(game, 0, "dapai", "m1_");
		game.next();
		EXPECT_EQ("dapai", game.status());
		EXPECT_EQ("m1_", game.dapai());
	}
	// ���[�`
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z1122", "", "", ""  }, { "m1" });
		game.zimo();
		set_reply(game, 0, "dapai", "m1_*");
		game.next();
		EXPECT_EQ("dapai", game.status());
		EXPECT_EQ("m1_*", game.dapai());
	}
	// �Ŕv(�s������)
	{
		auto game = init_game({}, -1, 0, 0, {}, { "m1" });
		game.zimo();
		set_reply(game, 0, "dapai", "m2_");
		game.next();
		EXPECT_EQ("dapai", game.status());
		EXPECT_NE("m2_", game.dapai());
	}
	// ����v
	{
		auto game = init_game({}, -1, 0, 0, { "m123459z1234567", "", "", "" });
		game.zimo();
		set_reply(game, 0, "daopai", "-");
		game.next();
		EXPECT_EQ("yao9", game.get_pingju().name);
	}
	// ����v(�s������)
	{
		auto game = init_game({}, -1, 0, 0, { "m234567z1234567", "", "", "" });
		game.zimo();
		set_reply(game, 0, "daopai", "-");
		game.next();
		EXPECT_EQ("dapai", game.status());
	}
	// �r�����ǂȂ��̏ꍇ�͋���v�ɂł��Ȃ�����
	{
		Rule rule;
		rule.abortiveDraw/*�r�����ǂ���*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m123459z1234567", "", "", "" });
		game.zimo();
		set_reply(game, 0, "daopai", "-");
		game.next();
		EXPECT_EQ("dapai", game.status());
	}
	// �c���a��
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z1122", "", "", "" }, { "z1" });
		game.zimo();
		set_reply(game, 0, "hule", "-");
		game.next();
		EXPECT_EQ("hule", game.status());
	}
	// �c���a��(�s������)
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z1122", "", "", "" }, { "z3" });
		game.zimo();
		set_reply(game, 0, "hule", "-");
		game.next();
		EXPECT_EQ("dapai", game.status());
	}
	// �J��
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456z1122,s888+", "", "", "" }, { "s8" });
		game.zimo();
		set_reply(game, 0, "gang", "s888+8");
		game.next();
		EXPECT_EQ("gang", game.status());
		EXPECT_EQ("s888+8", game.gang());
	}
	// �J��(�s������)
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456z1122,s888+", "", "", "" }, { "s7" });
		game.zimo();
		set_reply(game, 0, "gang", "s888+8");
		game.next();
		EXPECT_EQ("dapai", game.status());
	}
	// 5�߂̃J�����ł��Ȃ�����
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456z1122,s888+", "", "", "" }, { "s8" });
		game.set_n_gang({ 0, 0, 0, 4 });
		game.zimo();
		set_reply(game, 0, "gang", "s888+8");
		game.next();
		EXPECT_EQ("dapai", game.status());
	}
	// �������̂Ƃ��Ƀc���؂肷�邱��
	{
		auto game = init_game({}, -1, 0, 0, {}, { "m1" });
		game.zimo();
		set_reply(game, 0, "gang", "s888+8");
		game.next();
		EXPECT_EQ("dapai", game.status());
		EXPECT_EQ("m1_", game.dapai());
	}
	// �ȃc��
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
	// �����Ȃ�
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "" });
		game.zimo();
		game.dapai("m1");
		game.next();
		EXPECT_EQ("zimo", game.status());
	}
	// �����a��
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m123p456s789z1122", "", "" });
		game.zimo();
		game.dapai("z1");
		set_reply(game, 1, "hule", "-");
		game.next();
		EXPECT_EQ("hule", game.status());
		EXPECT_EQ(1, max_index(game.fenpei()));
	}
	// �a���������Ńt���e���ɂȂ邱��
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m123p456s789z1122", "", "" });
		game.zimo();
		game.dapai("z1");
		game.next();
		EXPECT_FALSE(game.neng_rong(1));
	}
	// �_�u����
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
	// �_�u�����𓪃n�l�ɕύX�ł��邱��
	{
		Rule rule;
		rule.maxSimultaneousWinners/*�ő哯���a����*/ = 1;
		auto game = init_game(rule, -1, 0, 0, { "_", "m23446p45688s345", "m34s33,s444-,s666+,p406-", "" });
		game.zimo();
		game.dapai("m5*");
		set_reply(game, 1, "hule", "-");
		set_reply(game, 2, "hule", "-");
		game.next();
		EXPECT_EQ(1, max_index(game.fenpei()));
		EXPECT_EQ((std::vector<int>{}), game.get_hule());
	}
	// �O�Ƙa
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
	// �g�������ɕύX�ł��邱��
	{
		Rule rule;
		rule.maxSimultaneousWinners/*�ő哯���a����*/ = 3;
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
	// ���[�`����
	{
		auto game = init_game({}, 0, 0, 0, { "m55688p234567s06", "", "", "" }, { "s7" });
		game.zimo();
		game.dapai("m5*");
		game.next();
		EXPECT_EQ(24000, game.model().defen[0]);
		EXPECT_EQ(1, game.model().lizhibang);
		EXPECT_EQ("zimo", game.status());
	}
	// ���[�`�s����
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
	// �l�Ɨ���
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
	// �r�����ǂȂ��̏ꍇ�͎l�Ɨ����ł����s���邱��
	{
		Rule rule;
		rule.abortiveDraw/*�r�����ǂ���*/ = false;
		auto game = init_game(rule, 0, 0, 0, { "m11156p5688s2346", "m2227p11340s2356", "m2346789p345699", "m34056p4678s3456" }, { "p4","s1","m7","s6" });
		for (const auto& p : { "s6*", "m7*", "p6*", "p4*" }) {
			game.zimo();
			game.dapai(p);
		}
		game.next();
		EXPECT_EQ("zimo", game.status());
	}
	// �l���A��
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
	// �r�����ǂȂ��̏ꍇ�͎l���A�łƂȂ炸�A���c�������I�����邱��
	{
		Rule rule;
		rule.abortiveDraw/*�r�����ǂ���*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "_", "_", "_", "_" });
		for (int l = 0; l < 4; l++) {
			game.zimo();
			game.dapai("z1");
		}
		game.next();
		EXPECT_FALSE(game.diyizimo());
		EXPECT_EQ("zimo", game.status());
	}
	 // �l�J��
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
	// 1�l�Ŏl�J��
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
	// �r�����ǂȂ��ł͎l�J�ȂƂȂ�Ȃ�
	{
		Rule rule;
		rule.abortiveDraw/*�r�����ǂ���*/ = false;
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
	// ����
	{
		Rule rule;
		rule.liujumanguan/*�������т���*/ = false;
		rule.notenDeclaration/*�m�[�e���錾����*/ = true;
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
	// �J��
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "m111234p567s3378" });
		game.zimo();
		game.dapai("m1");
		set_reply(game, 3, "fulou", "m1111+");
		game.next();
		EXPECT_EQ("fulou", game.status());
		EXPECT_EQ("m1111+", game.fulou());
	}
	// �J��(�s������)
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "m111234p567s3378" });
		game.zimo();
		game.dapai("m2");
		set_reply(game, 3, "fulou", "m1111+");
		game.next();
		EXPECT_EQ("zimo", game.status());
	}
	// 5�߂̃J�����ł��Ȃ�����
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "m111234p567s3378" });
		game.set_n_gang({ 4, 0, 0, 0 });
		game.zimo();
		game.dapai("m2");
		set_reply(game, 3, "fulou", "m1111+");
		game.next();
		EXPECT_EQ("zimo", game.status());
	}
	// �|��
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
	// �|��(�s������)
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "m112345p567s3378", "" });
		game.zimo();
		game.dapai("m2");
		set_reply(game, 2, "fulou", "m111=");
		game.next();
		EXPECT_EQ("zimo", game.status());
	}
	// �`�[
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m112345p567s3378", "", "" });
		game.zimo();
		game.dapai("m6");
		set_reply(game, 1, "fulou", "m456-");
		game.next();
		EXPECT_EQ("fulou", game.status());
		EXPECT_EQ("m456-", game.fulou());
	}
	// �`�[(�s������)
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m112345p567s3378", "", "" });
		game.zimo();
		game.dapai("m5");
		set_reply(game, 1, "fulou", "m456-");
		game.next();
		EXPECT_EQ("zimo", game.status());
	}
	// �|���ƃ`�[�̋����̓|����D��
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
	// �喾��
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m1112356p456s889", "", "" });
		game.zimo();
		game.dapai("m1");
		game.fulou("m1111-");
		game.next();
		EXPECT_EQ("gangzimo", game.status());
	}
	// �`�[�E�|�� �� �Ŕv
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
	// �`�[�E�|�� �� �Ŕv(�s������)
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
	// �������̂Ƃ��ɉE�[�̔v��؂邱��
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
	// �����Ȃ�
	{
		auto game = init_game({}, -1, 0, 0, { "m45p456s11789,m111+", "", "", "" }, { "m1" });
		game.zimo();
		game.gang("m111+1");
		game.next();
		EXPECT_EQ("gangzimo", game.status());
	}
	// �����a��(����)
	{
		auto game = init_game({}, -1, 0, 0, { "m45p456s11789,m111+", "", "", "m23456p123s67899" }, { "m1" });
		game.zimo();
		game.gang("m111+1");
		set_reply(game, 3, "hule", "-");
		game.next();
		EXPECT_EQ("hule", game.status());
		EXPECT_EQ(3, max_index(game.fenpei()));
	}
	// �����a��(�s������)
	{
		auto game = init_game({}, -1, 0, 0, { "m45p456s11789,m111+", "", "", "m33456p123s67899" }, { "m1" });
		game.zimo();
		game.gang("m111+1");
		set_reply(game, 3, "hule", "-");
		game.next();
		EXPECT_EQ("gangzimo", game.status());
	}
	// �ÞȂ͑��Ȃł��Ȃ�
	{
		auto game = init_game({}, -1, 0, 0, { "m11145p456s11789", "", "", "m23456p123s67899" }, { "m1" });
		game.zimo();
		game.gang("m1111");
		set_reply(game, 3, "hule", "-");
		game.next();
		EXPECT_EQ("gangzimo", game.status());
	}
	// �a���������Ńt���e���ɂȂ邱��
	{
		auto game = init_game({}, -1, 0, 0, { "m45p456s11789,m111+", "", "", "m23456p123s67899" }, { "m1" });
		game.zimo();
		game.gang("m111+1");
		game.next();
		EXPECT_FALSE(game.neng_rong(3));
	}
	// �_�u����
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
	// �_�u�����𓪃n�l�ɕύX�ł��邱��
	{
		Rule rule;
		rule.maxSimultaneousWinners/*�ő哯���a����*/ = 1;
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
	// �e�̃c���a��
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
	// �q�̃����a��
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
	// �_�u�����ŘA��
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
	// ����
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
	// ���݂̎�Ԃ̉\�ȑŔv��Ԃ�����
	{
		auto game = init_game({}, -1, 0, 0, { "m123,z111+,z222=,z333-", "", "", "" }, { "m1" });
		game.zimo();
		EXPECT_EQ((std::vector<std::string>{ "m1", "m2", "m3", "m1_" }), game.get_dapai());
	}
	// �����򂢑ւ�����ɕύX�ł��邱��
	{
		Rule rule;
		rule.canChangePermissionLevel/*�򂢑ւ������x��*/ = 2;
		auto game = init_game(rule, -1, 0, 0, { "_", "m1234p567,z111=,s789-", "", "" });
		game.zimo();
		game.dapai("m1");
		game.fulou("m1-23");
		EXPECT_EQ((std::vector<std::string>{ "m1", "m4", "p5", "p6", "p7" }), game.get_dapai());
	}
}

TEST(GameTest, get_chi_mianzi) {
	// ���ݑł��ꂽ�v�Ń`�[�ł���ʎq��Ԃ�����
	{
		auto game = init_game({}, -1, 0, 0, { "", "_", "m1234p456s789z111", "" });
		game.zimo();
		game.dapai(game.get_dapai()[0]);
		game.zimo();
		game.dapai("m2");
		EXPECT_EQ((std::vector<std::string>{ "m12-3", "m2-34" }), game.get_chi_mianzi(2));
	}
	// ���g�̓`�[�ł��Ȃ�����
	{
		auto game = init_game({}, -1, 0, 0, { "m1234p456s789z111", "", "", "" });
		game.zimo();
		game.dapai(game.get_dapai().back());
		EXPECT_THROW(game.get_chi_mianzi(0), std::invalid_argument);
	}
	// �Ζʂ̓`�[�ł��Ȃ�����
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "m1234p456s789z111", "" });
		game.zimo();
		game.dapai("m2");
		EXPECT_EQ((std::vector<std::string>{}), game.get_chi_mianzi(2));
	}
	// �n�C�e�C�v�̓`�[�ł��Ȃ�����
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m1234p456s789z111", "", "" });
		game.zimo();
		while (game.model().shan.paishu()) game.model().shan.zimo();
		game.dapai("m2");
		EXPECT_EQ((std::vector<std::string>{}), game.get_chi_mianzi(1));
	}
	// �����򂢑ւ�����ɕύX�ł��邱��
	{
		Rule rule;
		rule.canChangePermissionLevel/*�򂢑ւ������x��*/ = 2;
		auto game = init_game(rule, -1, 0, 0, { "_", "m1123,p456-,z111=,s789-", "", "" });
		game.zimo();
		game.dapai("m1");
		EXPECT_EQ((std::vector<std::string>{ "m1-23" }), game.get_chi_mianzi(1));
	}
}

TEST(GameTest, get_peng_mianzi) {
	// ���ݑł��ꂽ�v�Ń|���ł���ʎq��Ԃ�����
	{
		auto game = init_game({}, -1, 0, 0, { "", "_", "", "m1123p456s789z111" });
		game.zimo();
		game.dapai(game.get_dapai()[0]);
		game.zimo();
		game.dapai("m1");
		EXPECT_EQ((std::vector<std::string>{ "m111=" }), game.get_peng_mianzi(3));
	}
	// ���g�̓|���ł��Ȃ�����
	{
		auto game = init_game({}, -1, 0, 0, { "m1123p456s789z111", "", "", "" });
		game.zimo();
		game.dapai(game.get_dapai().back());
		EXPECT_THROW(game.get_peng_mianzi(0), std::invalid_argument);
	}
	// �n�C�e�C�v�̓|���ł��Ȃ�����
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m1123p456s789z111", "", "" });
		game.zimo();
		while (game.model().shan.paishu()) game.model().shan.zimo();
		game.dapai("m1");
		EXPECT_EQ((std::vector<std::string>{}), game.get_peng_mianzi(2));
	}
}

TEST(GameTest, get_gang_mianzi) {
	// ���ݑł��ꂽ�v�ő喾�Ȃł���ʎq��Ԃ�����
	{
		auto game = init_game({}, -1, 0, 0, { "", "_", "", "m1112p456s789z111" });
		game.zimo();
		game.dapai(game.get_dapai()[0]);
		game.zimo();
		game.dapai("m1");
		EXPECT_EQ((std::vector<std::string>{ "m1111=" }), game.get_gang_mianzi(3));
	}
	// ���g�͑喾�Ȃł��Ȃ�����
	{
		auto game = init_game({}, -1, 0, 0, { "m1112p456s789z111", "", "", "" });
		game.zimo();
		game.dapai(game.get_dapai().back());
		EXPECT_THROW(game.get_gang_mianzi(0), std::invalid_argument);
	}
	// �n�C�e�C�v�͑喾�Ȃł��Ȃ�����
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m1112p456s789z111", "", "" });
		game.zimo();
		while (game.model().shan.paishu()) game.model().shan.zimo();
		game.dapai("m1");
		EXPECT_EQ((std::vector<std::string>{}), game.get_gang_mianzi(2));
	}
	// ���݂̎�Ԃ��ÞȂ������͉��Ȃł���ʎq��Ԃ�����
	{
		auto game = init_game({}, -1, 0, 0, { "m1111p4569s78,z111=", "", "", "" }, {"z1"});
		game.zimo();
		EXPECT_EQ((std::vector<std::string>{ "m1111", "z111=1" }), game.get_gang_mianzi());
	}
	// �n�C�e�C�v�͈ÞȂ������͉��Ȃł��Ȃ�����
	{
		auto game = init_game({}, -1, 0, 0, { "m1111p4567s78,z111=", "", "", "" }, { "z1" });
		game.zimo();
		while (game.model().shan.paishu()) game.model().shan.zimo();
		game.dapai("m1");
		EXPECT_EQ((std::vector<std::string>{}), game.get_gang_mianzi(2));
	}
	// ���[�`��̈ÞȂȂ��ɕύX�ł��邱��
	{
		Rule rule;
		rule.lizhiPostClosedGangPermissionLevel/*���[�`��Þȋ����x��*/ = 0;
		auto game = init_game(rule, -1, 0, 0, { "m111p456s789z1122*", "", "", "" }, { "m1" });
		game.zimo();
		EXPECT_EQ((std::vector<std::string>{}), game.get_gang_mianzi());
	}
}

TEST(GameTest, allow_lizhi) {
	// �w�肳�ꂽ�Ŕv�Ń��[�`�\�ȏꍇ�A�^��Ԃ�
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z1123", "", "", "" }, { "z2" });
		game.zimo();
		EXPECT_TRUE(game.allow_lizhi("z3*"));
	}
	// �c���Ԃ��Ȃ��ꍇ�A���[�`�ł��Ȃ�
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z1123", "", "", "" }, { "z2" });
		game.zimo();
		while (game.model().shan.paishu() >= 4) game.model().shan.zimo();
		EXPECT_FALSE(game.allow_lizhi("z3*"));
	}
	// �����_��1000�_�ɖ����Ȃ��ꍇ�A���[�`�ł��Ȃ�
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z1123", "", "", "" }, { "z2" }, {}, {}, { 900, 19100, 45000, 35000 });
		game.zimo();
		EXPECT_FALSE(game.allow_lizhi("z3*"));
	}
	// �c���ԂȂ��ł����[�`�ł���悤�ɕύX�ł��邱��
	{
		Rule rule;
		rule.lizhiWithoutTsumoBonus/*�c���ԂȂ����[�`����*/ = true;
		auto game = init_game(rule, -1, 0, 0, { "m123p456s789z1123", "", "", "" }, { "z2" });
		game.zimo();
		while (game.model().shan.paishu() >= 4) game.model().shan.zimo();
		EXPECT_TRUE(game.allow_lizhi("z3*"));
	}
	// �����_��1000�_�ɖ����Ȃ��Ă����[�`�ł���悤�ɕύX�ł��邱��
	{
		Rule rule;
		rule.bankruptcyEndAll/*�g�r�I������*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m123p456s789z1123", "", "", "" }, { "z2" }, {}, {}, { 900, 19100, 45000, 35000 });
		game.zimo();
		EXPECT_TRUE(game.allow_lizhi("z3*"));
	}
}

TEST(GameTest, allow_hule) {
	// �c���a��
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z3344", "", "", "" }, { "z4" });
		game.zimo();
		EXPECT_TRUE(game.allow_hule());
	}
	// ���[�`�E�c��
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z4*,z333=", "", "", "" }, { "z4" });
		game.zimo();
		EXPECT_TRUE(game.allow_hule());
	}
	// ���J��
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "m123p456s789z3334", "" }, {}, { "z4" });
		game.zimo();
		game.dapai("z3");
		game.fulou("z3333=");
		game.gangzimo();
		EXPECT_TRUE(game.allow_hule());
	}
	// �n�C�e�C�E�c��
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z4,z333=", "", "", "" }, { "z4" });
		game.zimo();
		while (game.model().shan.paishu()) game.model().shan.zimo();
		EXPECT_TRUE(game.allow_hule());
	}
	// �ꕗ�̂݁E�c��
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m123p456s789z4,z111=", "", "" }, { "m1", "z4" });
		game.zimo();
		game.dapai("m1");
		game.zimo();
		EXPECT_TRUE(game.allow_hule());
	}
	// �����̂݁E�c��
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m123p456s789z4,z222=", "", "" }, { "m1", "z4" });
		game.zimo();
		game.dapai("m1");
		game.zimo();
		EXPECT_TRUE(game.allow_hule());
	}
	// ���[�`�E����
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m123p456s789z3334*", "", "" });
		game.zimo();
		game.dapai("z4");
		EXPECT_TRUE(game.allow_hule(1));
	}
	// ����
	{
		auto game = init_game({}, -1, 0, 0, { "m1234p567s789,m111=", "", "m23p123567s12377", "" });
		game.zimo();
		game.gang("m111=1");
		EXPECT_TRUE(game.allow_hule(2));
	}
	// �n�C�e�C�E����
	{
		auto game = init_game({}, -1, 0, 0, { "_", "", "", "m123p456s789z4,z333=" });
		game.zimo();
		while (game.model().shan.paishu()) game.model().shan.zimo();
		game.dapai("z4");
		EXPECT_TRUE(game.allow_hule(3));
	}
	// �ꕗ�̂݁E����
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m123p456s789z4,z111=", "", "" });
		game.zimo();
		game.dapai("z4");
		EXPECT_TRUE(game.allow_hule(1));
	}
	// �����̂݁E����
	{
		auto game = init_game({}, -1, 0, 0, { "_", "m123p456s789z4,z222=", "", "" });
		game.zimo();
		game.dapai("z4");
		EXPECT_TRUE(game.allow_hule(1));
	}
	// �t���e���̓����a���ł��Ȃ�����
	{
		auto game = init_game({}, -1, 0, 0, { "m123p456s789z3344", "", "", "" }, { "z4", "z3" });
		game.zimo();
		game.dapai("z4");
		game.zimo();
		game.dapai("z3");
		EXPECT_FALSE(game.allow_hule(0));
	}
	// �N�C�^���Ȃ��ɂł��邱��
	{
		Rule rule;
		rule.canduan/*�N�C�^������*/ = false;
		auto game = init_game({}, -1, 0, 0, { "_", "m234p567s2244,m888-", "", "" });
		game.zimo();
		game.dapai("s4");
		EXPECT_TRUE(game.allow_hule(1));
	}
}

TEST(GameTest, allow_pingju) {
	// ����v
	{
		auto game = init_game({}, -1, 0, 0, { "m123459z1234567", "", "", "" });
		game.zimo();
		EXPECT_TRUE(game.allow_pingju());
	}
	// ���c���ȍ~�͋���v�ɂȂ�Ȃ�
	{
		auto game = init_game({}, -1, 0, 0, { "_", "_", "m123459z1234567", "" });
		game.zimo();
		game.dapai("s2");
		game.fulou("s2-34");
		game.dapai("z3");
		game.zimo();
		EXPECT_FALSE(game.allow_pingju());
	}
	// �r�����ǂȂ��̏ꍇ�͋���v�ɂł��Ȃ�
	{
		Rule rule;
		rule.abortiveDraw/*�r�����ǂ���*/ = false;
		auto game = init_game(rule, -1, 0, 0, { "m123459z1234567", "", "", "" });
		game.zimo();
		EXPECT_FALSE(game.allow_pingju());
	}
}

TEST(GameTest, static_get_dapai) {
	Shoupai shoupai("m5678p567,z111=,s789-");
	shoupai.fulou("m0-67");

	// �򂢑ւ��Ȃ�
	{
		Rule rule;
		rule.canChangePermissionLevel/*�򂢑ւ������x��*/ = 0;
		EXPECT_EQ((std::vector<std::string>{ "p5", "p6", "p7" }), Game::get_dapai(rule, shoupai));
	}
	// �����ȊO�̋򂢑ւ�����
	{
		Rule rule;
		rule.canChangePermissionLevel/*�򂢑ւ������x��*/ = 1;
		EXPECT_EQ((std::vector<std::string>{ "m8", "p5", "p6", "p7" }), Game::get_dapai(rule, shoupai));
	}
	// �����򂢑ւ�������
	{
		Rule rule;
		rule.canChangePermissionLevel/*�򂢑ւ������x��*/ = 2;
		EXPECT_EQ((std::vector<std::string>{ "m5", "m8", "p5", "p6", "p7" }), Game::get_dapai(rule, shoupai));
	}
}

TEST(GameTest, static_get_chi_mianzi) {
	Shoupai shoupai1("m1234,p456-,z111=,s789-");
	Shoupai shoupai2("m1123,p456-,z111=,s789-");

	// �򂢑ւ��Ȃ�
	{
		Rule rule;
		rule.canChangePermissionLevel/*�򂢑ւ������x��*/ = 0;
		EXPECT_EQ((std::vector<std::string>{}), Game::get_chi_mianzi(rule, shoupai1, "m1-", 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_chi_mianzi(rule, shoupai2, "m1-", 1));
	}
	// �����ȊO�̋򂢑ւ�����
	{
		Rule rule;
		rule.canChangePermissionLevel/*�򂢑ւ������x��*/ = 1;
		EXPECT_EQ((std::vector<std::string>{ "m1-23" }), Game::get_chi_mianzi(rule, shoupai1, "m1-", 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_chi_mianzi(rule, shoupai2, "m1-", 1));
	}
	// �����򂢑ւ�������
	{
		Rule rule;
		rule.canChangePermissionLevel/*�򂢑ւ������x��*/ = 2;
		EXPECT_EQ((std::vector<std::string>{ "m1-23" }), Game::get_chi_mianzi(rule, shoupai1, "m1-", 1));
		EXPECT_EQ((std::vector<std::string>{ "m1-23" }), Game::get_chi_mianzi(rule, shoupai2, "m1-", 1));
	}
	// �n�C�e�C�͖��Ȃ�
	{
		Rule rule;
		rule.canChangePermissionLevel/*�򂢑ւ������x��*/ = 2;
		EXPECT_EQ((std::vector<std::string>{}), Game::get_chi_mianzi(rule, shoupai1, "m1-", 0));
	}
	// �c��������ԂŃ`�[�ł��Ȃ�
	{
		EXPECT_EQ((std::vector<std::string>{}), Game::get_chi_mianzi({}, shoupai1, "s3-", 1));
	}
}

TEST(GameTest, static_get_peng_mianzi) {
	Shoupai shoupai("m1112,p456-,z111=,s789-");

	// �򂢑ւ��̂��߂Ƀ|���ł��Ȃ��P�[�X�͂Ȃ�
	{
		Rule rule;
		rule.canChangePermissionLevel/*�򂢑ւ������x��*/ = 0;
		EXPECT_EQ((std::vector<std::string>{ "m111+" }), Game::get_peng_mianzi(rule, shoupai, "m1+", 1));
	}
	// �n�C�e�C�͖��Ȃ�
	{
		Rule rule;
		rule.canChangePermissionLevel/*�򂢑ւ������x��*/ = 0;
		EXPECT_EQ((std::vector<std::string>{}), Game::get_peng_mianzi(rule, shoupai, "m1+", 0));
	}
	// �c��������ԂŃ|���ł��Ȃ�
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

	// ���[�`��̈ÞȂȂ�
	{
		Rule rule;
		rule.lizhiPostClosedGangPermissionLevel/*���[�`��Þȋ����x��*/ = 0;
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai1, {}, 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai2, {}, 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai3, {}, 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai4, {}, 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai5, {}, 1));
	}
	// ���[�`��̔v�p�̕ς��ÞȂȂ�
	{
		Rule rule;
		rule.lizhiPostClosedGangPermissionLevel/*���[�`��Þȋ����x��*/ = 1;
		EXPECT_EQ((std::vector<std::string>{ "z1111" }), Game::get_gang_mianzi(rule, shoupai1, {}, 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai2, {}, 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai3, {}, 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai4, {}, 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai5, {}, 1));
	}
	// ���[�`��̑҂��̕ς��ÞȂȂ�
	{
		Rule rule;
		rule.lizhiPostClosedGangPermissionLevel/*���[�`��Þȋ����x��*/ = 2;
		EXPECT_EQ((std::vector<std::string>{ "z1111" }), Game::get_gang_mianzi(rule, shoupai1, {}, 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai2, {}, 1));
		EXPECT_EQ((std::vector<std::string>{ "s3333" }), Game::get_gang_mianzi(rule, shoupai3, {}, 1));
		EXPECT_EQ((std::vector<std::string>{ "s1111" }), Game::get_gang_mianzi(rule, shoupai4, {}, 1));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi(rule, shoupai5, {}, 1));
	}
	// �n�C�e�C�̓J���ł��Ȃ�
	{
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi({}, Shoupai("m1112p456s789z111z1"), {}, 0));
		EXPECT_EQ((std::vector<std::string>{}), Game::get_gang_mianzi({}, Shoupai("m1112p456s789z111"), "z1=", 0));
	}
}

TEST(GameTest, static_allow_lizhi) {
	// �Ŕv�ł��Ȃ��ꍇ�A���[�`�͂ł��Ȃ�
	{
		EXPECT_FALSE(Game::allow_lizhi({}, Shoupai("m123p456s789z1122")).first);
	}
	// ���łɃ��[�`���Ă���ꍇ�A���[�`�͂ł��Ȃ�
	{
		EXPECT_FALSE(Game::allow_lizhi({}, Shoupai("m123p456s789z11223*")).first);
	}
	// �����[���łȂ��ꍇ�A���[�`�͂ł��Ȃ�
	{
		EXPECT_FALSE(Game::allow_lizhi({}, Shoupai("m123p456s789z23,z111=")).first);
	}
	// �c���Ԃ��Ȃ��ꍇ�A���[�`�͂ł��Ȃ�
	{
		EXPECT_FALSE(Game::allow_lizhi({}, Shoupai("m123p456s789z11223"), "z3", 3));
	}
	// ���[���������΃c���Ԃ��Ȃ��Ă����[�`�͉\
	{
		Rule rule;
		rule.lizhiWithoutTsumoBonus/*�c���ԂȂ����[�`����*/ = true;
		EXPECT_TRUE(Game::allow_lizhi(rule, Shoupai("m123p456s789z11223"), "z3", 3));
	}
	// �����_��1000�_�ɖ����Ȃ��ꍇ�A���[�`�͂ł��Ȃ�
	{
		EXPECT_FALSE(Game::allow_lizhi({}, Shoupai("m123p456s789z11223"), "z3", 3, 900));
	}
	// �g�r�Ȃ��Ȃ玝���_��1000�_�ɖ����Ȃ��Ă����[�`�͉\
	{
		Rule rule;
		rule.bankruptcyEndAll/*�g�r�I������*/ = false;
		EXPECT_FALSE(Game::allow_lizhi(rule, Shoupai("m123p456s789z11223"), "z3", 3, 900));
	}
	// �e���p�C���Ă��Ȃ��ꍇ�A���[�`�͂ł��Ȃ�
	{
		EXPECT_FALSE(Game::allow_lizhi({}, Shoupai("m123p456s789z11234")).first);
	}
	// �`���e���p�C�ƔF�߂��Ȃ��v�p�Ń��[�`�͂ł��Ȃ�
	{
		EXPECT_FALSE(Game::allow_lizhi({}, Shoupai("m123p456s789z11112"), "z2"));
	}
	// �w�肳�ꂽ�Ŕv�Ń��[�`�\�ȏꍇ�A�^��Ԃ�����
	{
		EXPECT_TRUE(Game::allow_lizhi({}, Shoupai("m123p456s789z11112"), "z1"));
	}
	// �Ŕv���w�肳��Ă��Ȃ��ꍇ�A���[�`�\�ȑŔv�ꗗ��Ԃ�
	{
		EXPECT_EQ((std::vector<std::string>{ "s7", "s8" }), Game::allow_lizhi({}, Shoupai("m123p456s788z11122")).second);
		EXPECT_EQ((std::vector<std::string>{ "z3_" }), Game::allow_lizhi({}, Shoupai("m123p456s789z11223")).second);
	}
	// ���[�`�\�ȑŔv���Ȃ��ꍇ�Afalse ��Ԃ�
	{
		EXPECT_FALSE(Game::allow_lizhi({}, Shoupai("m11112344449999")).first);
	}
}

TEST(GameTest, static_allow_hule) {
	// �t���e���̏ꍇ�A�����a���ł��Ȃ�
	{
		EXPECT_FALSE(Game::allow_hule({}, Shoupai("m123p456z1122,s789-"), "z1=", 0, 1, false, false));
	}
	// �a���`�ɂȂ��Ă��Ȃ��ꍇ�A�a���ł��Ȃ�
	{
		EXPECT_FALSE(Game::allow_hule({}, Shoupai("m123p456z11223,s789-"), {}, 0, 1, false, true));
	}
	// ������a���`�̏ꍇ�A�a���ł���
	{
		EXPECT_TRUE(Game::allow_hule({}, Shoupai("m123p456s789z3377"), "z3+", 0, 1, true, true));
	}
	// ���Ȃ��a���`�̏ꍇ�A�a���ł��Ȃ�
	{
		EXPECT_FALSE(Game::allow_hule({}, Shoupai("m123p456s789z3377"), "z3+", 0, 1, false, true));
	}
	// �N�C�^���Ȃ��̏ꍇ�A�N�C�^���ł͘a���ł��Ȃ�
	{
		Rule rule;
		rule.canduan/*�N�C�^������*/ = false;
		EXPECT_FALSE(Game::allow_hule(rule, Shoupai("m22555p234s78,p777-"), "s6=", 0, 1, false, true));
	}
	// �c���a��
	{
		EXPECT_TRUE(Game::allow_hule({}, Shoupai("m123p456s789z33377"), {}, 0, 1, false, false));
	}
	// �����a��
	{
		EXPECT_TRUE(Game::allow_hule({}, Shoupai("m123p456z1122,s789-"), "z1=", 0, 1, false, true));
	}
}

TEST(GameTest, static_allow_pingju) {
	// ��ꏄ�łȂ��ꍇ�A����v�ƂȂ�Ȃ�
	{
		EXPECT_FALSE(Game::allow_pingju({}, Shoupai("m1234569z1234567"), false));
	}
	// �c����łȂ��ꍇ�A����v�ƂȂ�Ȃ�
	{
		EXPECT_FALSE(Game::allow_pingju({}, Shoupai("m123459z1234567"), true));
	}
	// �r�����ǂȂ��ꍇ�A����v�ƂȂ�Ȃ�
	{
		Rule rule;
		rule.abortiveDraw/*�r�����ǂ���*/ = false;
		EXPECT_FALSE(Game::allow_pingju(rule, Shoupai("m1234569z1234567"), true));
	}
	// �����v�͗��ǂɂł��Ȃ�
	{
		EXPECT_FALSE(Game::allow_pingju({}, Shoupai("m1234567z1234567"), true));
	}
	// ����v
	{
		EXPECT_TRUE(Game::allow_pingju({}, Shoupai("m1234569z1234567"), true));
	}
}

TEST(GameTest, static_allow_no_daopai) {
	Rule rule;
	rule.notenDeclaration/*�m�[�e���錾����*/ = true;

	// �ŏI�Ŕv�ȊO�̓m�[�e���錾�ł��Ȃ�
	{
		EXPECT_FALSE(Game::allow_no_daopai(rule, Shoupai("m123p456z1122,s789-"), 1));
		EXPECT_FALSE(Game::allow_no_daopai(rule, Shoupai("m123p456z1122,s789-").zimo("z3"), 0));
	}
	// �m�[�e���錾����̃��[���łȂ��ꍇ�A�m�[�e���錾�ł��Ȃ�
	{
		EXPECT_FALSE(Game::allow_no_daopai({}, Shoupai("m123p456z1122,s789-"), 0));
	}
	// ���[�`���Ă���ꍇ�A�m�[�e���錾�ł��Ȃ�
	{
		EXPECT_FALSE(Game::allow_no_daopai(rule, Shoupai("m123p456p789z1122*"), 0));
	}
	// �e���p�C���Ă��Ȃ��ꍇ�A�m�[�e���錾�ł��Ȃ�
	{
		EXPECT_FALSE(Game::allow_no_daopai(rule, Shoupai("m123p456p789z1123"), 0));
	}
	// �`���e���p�C�ƔF�߂��Ȃ��v�p�̏ꍇ�A�m�[�e���錾�ł��Ȃ�
	{
		EXPECT_FALSE(Game::allow_no_daopai(rule, Shoupai("m123p456p789z1111"), 0));
	}
	// �m�[�e���錾
	{
		EXPECT_TRUE(Game::allow_no_daopai(rule, Shoupai("m123p456z1122,s789-"), 0));
	}
}
