#include "pch.h"
#include "../src_cpp/hule.h"

#include <fstream>
#include <filesystem>
#include <map>

std::map<const std::string, Hupai::Name> HUPAI_MAP = {
    { u8"場風 東", Hupai::CHANGFENGDONG },
    { u8"場風 南", Hupai::CHANGFENGNAN },
    { u8"場風 西", Hupai::CHANGFENGXI },
    { u8"場風 北", Hupai::CHANGFENGBEI },
    { u8"自風 東", Hupai::ZIFENGDONG },
    { u8"自風 南", Hupai::ZIFENGNAN },
    { u8"自風 西", Hupai::ZIFENGXI },
    { u8"自風 北", Hupai::ZIFENGBEI },
    { u8"翻牌 白", Hupai::FANPAIBAI },
    { u8"翻牌 發", Hupai::FANPAIFA },
    { u8"翻牌 中", Hupai::FANPAIZHONG },
    { u8"ドラ", Hupai::BAOPAI },
    { u8"赤ドラ", Hupai::CHIBAOPAI },
    { u8"裏ドラ", Hupai::LIBAOPAI },
    { u8"立直", Hupai::LIZHI },
    { u8"ダブル立直", Hupai::DABULIZHI },
    { u8"一発", Hupai::YIPA },
    { u8"海底摸月", Hupai::HAIDIMOYUE },
    { u8"河底撈魚", Hupai::HEDILAOYU },
    { u8"嶺上開花", Hupai::LINGSHANGKAIHUA },
    { u8"槍槓", Hupai::QIANGGANG },
    { u8"天和", Hupai::TIANHE },
    { u8"地和", Hupai::DIHE },
    { u8"門前清自摸和", Hupai::MENQIANQINGZIMOHE },
    { u8"平和", Hupai::PINGHE },
    { u8"断幺九", Hupai::DUANYAOJIU },
    { u8"一盃口", Hupai::YIBEIKOU },
    { u8"三色同順", Hupai::SANSETONGSHUN },
    { u8"一気通貫", Hupai::YIQITONGGUAN },
    { u8"混全帯幺九", Hupai::HUNQUANDAIYAOJIU },
    { u8"七対子", Hupai::QIDUIZI },
    { u8"対々和", Hupai::DUIDUIHU },
    { u8"三暗刻", Hupai::SANANKE },
    { u8"三槓子", Hupai::SANGANGZI },
    { u8"三色同刻", Hupai::SANSETONGKE },
    { u8"混老頭", Hupai::HUNLAOTOU },
    { u8"小三元", Hupai::XIAOSANYUAN },
    { u8"混一色", Hupai::HUNYISE },
    { u8"純全帯幺九", Hupai::CHUNQUANDAIYAOJIU },
    { u8"二盃口", Hupai::ERBEIKOU },
    { u8"清一色", Hupai::QINGYISE },
    { u8"国士無双十三面", Hupai::GUOSHIWUSHUANGSHISANMIAN },
    { u8"国士無双", Hupai::GUOSHIWUSHUANG },
    { u8"四暗刻単騎", Hupai::SIANKEDANQI },
    { u8"四暗刻", Hupai::SIANKE },
    { u8"大三元", Hupai::DASANYUAN },
    { u8"大四喜", Hupai::DASIXI },
    { u8"小四喜", Hupai::XIAOSIXI },
    { u8"字一色", Hupai::ZIYISE },
    { u8"緑一色", Hupai::LVYISE },
    { u8"清老頭", Hupai::QINGLAOTOU },
    { u8"四槓子", Hupai::SIGANGZI },
    { u8"純正九蓮宝燈", Hupai::CHUNZHENGJIULIANBAODENG },
    { u8"九蓮宝燈", Hupai::JIULIANBAODENG },
};


TEST(HuleTest, hule_mianzi) {
    // 一般手(ツモ和了)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "z22_!", "m123", "p555", "s789", "z111" }}),
        hule_mianzi(Shoupai("m123p055s789z11122")));
    // 一般手(ロン和了)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "z22+!", "m123", "p555", "s789", "z111" }}),
        hule_mianzi(Shoupai("m123p055s789z1112"), "z2+"));
    // 一般手(副露あり)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "z22=!", "m123", "p555", "z111", "s7-89" }}),
        hule_mianzi(Shoupai("m123p055z1112,s7-89"), "z2="));
    // 七対子形
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "m22", "m55-!", "p44", "p66", "s11", "s99", "z33" }}),
        hule_mianzi(Shoupai("m225p4466s1199z33"), "m0-"));
    // 国士無双形(ツモ)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "z77", "m1_!", "m9", "p1", "p9", "s1", "s9", "z1", "z2", "z3", "z4", "z5", "z6" }}),
        hule_mianzi(Shoupai("m9p19s19z12345677m1"), {}));
    // 国士無双形(13面待ちロン)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "m99+!", "m1", "p1", "p9", "s1", "s9", "z1", "z2", "z3", "z4", "z5", "z6", "z7" }}),
        hule_mianzi(Shoupai("m19p19s19z1234567"), "m9+"));
    // 九蓮宝燈形
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "m55=!", "m111", "m234", "m678", "m999" },
            std::vector<std::string>{ "m11123456789995=!" }}),
        hule_mianzi(Shoupai("m1112345678999"), "m0="));
    // 和了形以外(少牌)
    EXPECT_EQ((std::vector<std::vector<std::string>>{}),
        hule_mianzi(Shoupai("m123p055s789z1122")));
    // 和了形以外(三面子)
    EXPECT_EQ((std::vector<std::vector<std::string>>{}),
        hule_mianzi(Shoupai("___m123p055z2,s7-89"), "z2="));
    // 和了形以外(一対子)
    EXPECT_EQ((std::vector<std::vector<std::string>>{}),
        hule_mianzi(Shoupai("m22")));
    // 和了形以外(国士無双テンパイ)
    EXPECT_EQ((std::vector<std::vector<std::string>>{}),
        hule_mianzi(Shoupai("m19p19s19z123456"), "z7="));
    // 和了形以外(九蓮宝燈テンパイ)
    EXPECT_EQ((std::vector<std::vector<std::string>>{}),
        hule_mianzi(Shoupai("m111234567899"), "m9="));
    // 複数の和了形としない(順子優先)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "z11_!", "m123", "m111", "p789", "p999" }}),
        hule_mianzi(Shoupai("m111123p789999z1z1")));
    // 複数の和了形(二盃口か七対子か)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "s88_!", "m234", "m234", "p567", "p567" },
            std::vector<std::string>{ "m22", "m33", "m44", "p55", "p66", "p77", "s88_!" }}),
        hule_mianzi(Shoupai("m223344p556677s88")));
    // 複数の和了形(順子か刻子か)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "p99", "m123", "m123", "m123", "p7_!89" },
            std::vector<std::string>{ "p99", "m111", "m222", "m333", "p7_!89" }}),
        hule_mianzi(Shoupai("m111222333p89997")));
    // 複数の和了形(雀頭の選択、平和かサンショクか)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "m22", "m3_!45", "m345", "p234", "s234" },
            std::vector<std::string>{ "m55", "m23_!4", "m234", "p234", "s234" }}),
        hule_mianzi(Shoupai("m2234455p234s234m3")));
    // 複数の和了形(暗刻を含む形)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "s33", "m1_!23", "p567", "s345", "s666" },
            std::vector<std::string>{ "s66", "m1_!23", "p567", "s333", "s456" }}),
        hule_mianzi(Shoupai("m23p567s33345666m1")));
    // 複数の和了形(九蓮宝燈形)
    EXPECT_EQ((std::vector<std::vector<std::string>>{
        std::vector<std::string>{ "s99", "s111", "s2_!34", "s456", "s789" },
            std::vector<std::string>{ "s11134456789992_!" }}),
        hule_mianzi(Shoupai("s1113445678999s2")));
}

TEST(HuleTest, hule) {
    // パラメータ不正
    EXPECT_THROW(hule(Shoupai(), "m1", {}), std::invalid_argument);
    // 和了形以外
    {
        auto hule_ = hule(Shoupai(), {}, {});
        EXPECT_EQ(0, hule_.defen);
    }
    // 符計算: 平和ツモは20符
    {
        auto hule_ = hule(Shoupai("m345567p234s33789"), {}, {});
        EXPECT_EQ(20, hule_.fu);
    }
    // 符計算: 平和ロンは30符
    {
        auto hule_ = hule(Shoupai("m345567p234s3378"), "s9=", {});
        EXPECT_EQ(30, hule_.fu);
    }
    // 符計算: オタ風の雀頭に符はつかない
    {
        auto hule_ = hule(Shoupai("m112233p456z33s78"), "s9=", {});
        EXPECT_EQ(30, hule_.fu);
    }
    // 符計算: 場風の雀頭は2符
    {
        auto hule_ = hule(Shoupai("m112233p456z11s78"), "s9=", {});
        EXPECT_EQ(40, hule_.fu);
    }
    // 符計算: 自風の雀頭は2符
    {
        auto hule_ = hule(Shoupai("m112233p456z22s78"), "s9=", {});
        EXPECT_EQ(40, hule_.fu);
    }
    // 符計算: 三元牌の雀頭は2符
    {
        auto hule_ = hule(Shoupai("m112233p456z55s78"), "s9=", {});
        EXPECT_EQ(40, hule_.fu);
    }
    // 符計算: 連風牌の雀頭は4符
    {
        Param param;
        param.menfeng = 0;
        auto hule_ = hule(Shoupai("m112233z444z11s78"), "s9=", param);
        EXPECT_EQ(50, hule_.fu);
    }
    // 符計算: 中張牌の明刻は2符
    {
        Param param;
        param.menfeng = 0;
        auto hule_ = hule(Shoupai("m123z11m88,p888+,s888-"), "m8=", param);
        EXPECT_EQ(30, hule_.fu);
    }
    // 符計算: 幺九牌の明刻は4符
    {
        auto hule_ = hule(Shoupai("m123p22s99,z222+,p111-"), "s9=", {});
        EXPECT_EQ(40, hule_.fu);
    }
    // 符計算: 中張牌の暗刻は4符
    {
        auto hule_ = hule(Shoupai("z33p222777s888m23"), "m4=", {});
        EXPECT_EQ(50, hule_.fu);
    }
    // 符計算: 幺九牌の暗刻は8符
    {
        auto hule_ = hule(Shoupai("s33p111999z555m23"), "m4=", {});
        EXPECT_EQ(60, hule_.fu);
    }
    // 符計算: 符計算: 中張牌の明槓は8符
    {
        auto hule_ = hule(Shoupai("p33m22245667,s444+4"), "m8=", {});
        EXPECT_EQ(40, hule_.fu);
    }
    // 符計算: 幺九牌の明槓は16符
    {
        auto hule_ = hule(Shoupai("p33m23445667,z6666-"), "m8=", {});
        EXPECT_EQ(40, hule_.fu);
    }
    // 符計算: 中張牌の暗槓は16符
    {
        auto hule_ = hule(Shoupai("p33m23445667,s4444"), "m8=", {});
        EXPECT_EQ(50, hule_.fu);
    }
    // 符計算: 幺九牌の暗槓は32符
    {
        auto hule_ = hule(Shoupai("p33m23445667,z7777"), "m8=", {});
        EXPECT_EQ(70, hule_.fu);
    }
    // 符計算: ツモ和了は2符加算
    {
        auto hule_ = hule(Shoupai("p33m222s222345,s888-"), {}, {});
        EXPECT_EQ(40, hule_.fu);
    }
    // 符計算: 単騎待ちは2符加算
    {
        auto hule_ = hule(Shoupai("m222s222345p3,s888-"), "p3=", {});
        EXPECT_EQ(40, hule_.fu);
    }
    // 符計算: 嵌張待ちは2符加算
    {
        auto hule_ = hule(Shoupai("p33m222s22235,s888-"), "s4=", {});
        EXPECT_EQ(40, hule_.fu);
    }
    // 符計算: 辺張待ちは2符加算
    {
        auto hule_ = hule(Shoupai("p33z111m12389,s222-"), "m7=", {});
        EXPECT_EQ(40, hule_.fu);
    }
    // 符計算: 喰い平和は30符
    {
        auto hule_ = hule(Shoupai("m22p345678s34,s67-8"), "s5=", {});
        EXPECT_EQ(30, hule_.fu);
    }
    // 符計算: 七対子は25符
    {
        auto hule_ = hule(Shoupai("m2255p88s1166z1155"), {}, {});
        EXPECT_EQ(25, hule_.fu);
    }
    // 符計算: 国士無双は符なし
    {
        auto hule_ = hule(Shoupai("m19p19s1z12345677s9"), {}, {});
        EXPECT_EQ(0, hule_.fu);
    }
    // 符計算: 九蓮宝燈は符なし
    {
        auto hule_ = hule(Shoupai("m11123456789995"), {}, {});
        EXPECT_EQ(0, hule_.fu);
    }
    // 和了役: 役なし
    {
        auto hule_ = hule(Shoupai("m344556s24678z66"), "s3=", {});
        EXPECT_EQ((std::vector<Hupai>{}), hule_.hupai);
    }
    // 和了役: 立直
    {
        Param param;
        param.hupai.lizhi = 1;
        auto hule_ = hule(Shoupai("m344556s24678z66*"), "s3=", param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::LIZHI, 1 } }), hule_.hupai);
    }
    // 和了役: ダブル立直
    {
        Param param;
        param.hupai.lizhi = 2;
        auto hule_ = hule(Shoupai("m344556s24678z66*"), "s3=", param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::DABULIZHI, 2 } }), hule_.hupai);
    }
    // 和了役: 立直・一発
    {
        Param param;
        param.hupai.lizhi = 1;
        param.hupai.yifa = true;
        auto hule_ = hule(Shoupai("m344556s24678z66*"), "s3=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ Hupai::LIZHI, 1 },
                Hupai{ Hupai::YIPA, 1 } }), hule_.hupai);
    }
    // 和了役: 海底摸月
    {
        Param param;
        param.hupai.haidi = 1;
        auto hule_ = hule(Shoupai("m344556s24z66s3,s6-78"), {}, param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::HAIDIMOYUE, 1 } }), hule_.hupai);
    }
    // 和了役: 河底撈魚
    {
        Param param;
        param.hupai.haidi = 2;
        auto hule_ = hule(Shoupai("m344556s24678z66"), "s3=", param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::HEDILAOYU, 1 } }), hule_.hupai);
    }
    // 和了役: 嶺上開花
    {
        Param param;
        param.hupai.lingshang = true;
        auto hule_ = hule(Shoupai("m344556s24z66s3,s777+7"), {}, param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::LINGSHANGKAIHUA, 1 } }), hule_.hupai);
    }
    // 和了役: 槍槓
    {
        Param param;
        param.hupai.qianggang = true;
        auto hule_ = hule(Shoupai("m344556s24678z66"), "s3=", param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::QIANGGANG, 1 } }), hule_.hupai);
    }
    // 和了役: 天和
    {
        Param param;
        param.hupai.tianhu = true;
        auto hule_ = hule(Shoupai("m344556s24678z66s3"), {}, param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::TIANHE, -1 } }), hule_.hupai);
    }
    // 和了役: 地和
    {
        Param param;
        param.hupai.tianhu = 2;
        auto hule_ = hule(Shoupai("m344556s24678z66s3"), {}, param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::DIHE, -1 } }), hule_.hupai);
    }
    // 和了役: 門前清自摸和
    {
        auto hule_ = hule(Shoupai("m344556s24678z66s3"), {}, {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::MENQIANQINGZIMOHE, 1 } }), hule_.hupai);
    }
    // 和了役: 場風 東
    {
        auto hule_ = hule(Shoupai("m345567s3378z111"), "s9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::CHANGFENGDONG, 1 } }), hule_.hupai);
    }
    // 和了役: 自風 西
    {
        Param param;
        param.menfeng = 2;
        auto hule_ = hule(Shoupai("m345567s33789,z333+"), {}, param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::ZIFENGXI, 1 } }), hule_.hupai);
    }
    // 和了役: 連風牌 南
    {
        Param param;
        param.zhuangfeng = 1;
        auto hule_ = hule(Shoupai("m345567s33z22,s789-"), "z2=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ Hupai::CHANGFENGNAN, 1 },
                Hupai{ Hupai::ZIFENGNAN, 1 } }), hule_.hupai);
    }
    // 和了役: 翻牌 白
    {
        auto hule_ = hule(Shoupai("m345567s33789,z555+5"), {}, {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::FANPAIBAI, 1 } }), hule_.hupai);
    }
    // 和了役: 翻牌 發・中
    {
        auto hule_ = hule(Shoupai("m345567s33,z6666+,z7777"), {}, {});
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ Hupai::FANPAIFA, 1 },
                Hupai{ Hupai::FANPAIZHONG, 1 } }), hule_.hupai);
    }
    // 和了役: 平和
    {
        auto hule_ = hule(Shoupai("z33m234456p78s123"), "p9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::PINGHE, 1 } }), hule_.hupai);
    }
    // 和了役: 平和・ツモ
    {
        auto hule_ = hule(Shoupai("z33m234456p78s123p9"), {}, {});
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ Hupai::MENQIANQINGZIMOHE, 1 },
                Hupai{ Hupai::PINGHE, 1 } }), hule_.hupai);
    }
    // 和了役: 喰い平和(役なし)
    {
        auto hule_ = hule(Shoupai("z33m234456p78,s1-23"), "p9=", {});
        EXPECT_EQ((std::vector<Hupai>{}), hule_.hupai);
    }
    // 和了役: 断幺九
    {
        auto hule_ = hule(Shoupai("m22555p234s78,p777-"), "s6=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::DUANYAOJIU, 1 } }), hule_.hupai);
    }
    // 和了役: 断幺九(七対子形)
    {
        auto hule_ = hule(Shoupai("m2255p4488s33667"), "s7=", {});
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ Hupai::DUANYAOJIU, 1 },
                Hupai{ Hupai::QIDUIZI, 2 } }), hule_.hupai);
    }
    // 和了役: 一盃口
    {
        auto hule_ = hule(Shoupai("m33455p111s33789"), "m4=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::YIBEIKOU, 1 } }), hule_.hupai);
    }
    // 和了役: 喰い一盃口(役なし)
    {
        auto hule_ = hule(Shoupai("m33455p111s33,s78-9"), "m4=", {});
        EXPECT_EQ((std::vector<Hupai>{}), hule_.hupai);
    }
    // 和了役: 三色同順
    {
        auto hule_ = hule(Shoupai("m567p567s2256799"), "s9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::SANSETONGSHUN, 2 } }), hule_.hupai);
    }
    // 和了役: 三色同順(喰い下がり)
    {
        auto hule_ = hule(Shoupai("m567s2256799,p56-7"), "s9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::SANSETONGSHUN, 1 } }), hule_.hupai);
    }
    // 和了役: 一気通貫
    {
        auto hule_ = hule(Shoupai("m12456789s33789"), "m3=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::YIQITONGGUAN, 2 } }), hule_.hupai);
    }
    // 和了役: 一気通貫(喰い下がり)
    {
        auto hule_ = hule(Shoupai("m12789s33789,m4-56"), "m3=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::YIQITONGGUAN, 1 } }), hule_.hupai);
    }
    // 和了役: 混全帯幺九
    {
        auto hule_ = hule(Shoupai("m123999p789z33s12"), "s3=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::HUNQUANDAIYAOJIU, 2 } }), hule_.hupai);
    }
    // 和了役: 混全帯幺九(喰い下がり)
    {
        auto hule_ = hule(Shoupai("m123p789z33s12,m999+"), "s3=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::HUNQUANDAIYAOJIU, 1 } }), hule_.hupai);
    }
    // 和了役: 七対子
    {
        auto hule_ = hule(Shoupai("m115599p2233s8z22"), "s8=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::QIDUIZI, 2 } }), hule_.hupai);
    }
    // 和了役: 対々和
    {
        auto hule_ = hule(Shoupai("m55888z333s22,p111="), "s2=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::DUIDUIHU, 2 } }), hule_.hupai);
    }
    // 和了役: 三暗刻
    {
        auto hule_ = hule(Shoupai("p99s111m555,p345-,s3333"), {}, {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::SANANKE, 2 } }), hule_.hupai);
    }
    // 和了役: 三槓子
    {
        auto hule_ = hule(Shoupai("p11m45,s2222+,m888=8,z4444"), "m3=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::SANGANGZI, 2 } }), hule_.hupai);
    }
    // 和了役: 三色同刻
    {
        auto hule_ = hule(Shoupai("s12377m22,p222-,s222-"), "m2=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::SANSETONGKE, 2 } }), hule_.hupai);
    }
    // 和了役: 混老頭(対々和形)
    {
        auto hule_ = hule(Shoupai("z11p11199,m111=,z333+"), "p9=", {});
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ Hupai::DUIDUIHU, 2 },
                Hupai{ Hupai::HUNLAOTOU, 2 } }), hule_.hupai);
    }
    // 和了役: 混老頭(七対子形)
    {
        auto hule_ = hule(Shoupai("m1199p11s99z11335"), "z5=", {});
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ Hupai::QIDUIZI, 2 },
                Hupai{ Hupai::HUNLAOTOU, 2 } }), hule_.hupai);
    }
    // 和了役: 小三元
    {
        auto hule_ = hule(Shoupai("z55577m567p22,z666-"), "p2=", {});
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ Hupai::FANPAIBAI, 1 },
                Hupai{ Hupai::FANPAIFA, 1 },
                Hupai{ Hupai::XIAOSANYUAN, 2 } }), hule_.hupai);
    }
    // 和了役: 混一色
    {
        auto hule_ = hule(Shoupai("m111234789z1133"), "z3=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::HUNYISE, 3 } }), hule_.hupai);
    }
    // 和了役: 混一色(喰い下がり)
    {
        auto hule_ = hule(Shoupai("z11333p23478,p111+"), "p9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::HUNYISE, 2 } }), hule_.hupai);
    }
    // 和了役: 混一色(七対子形)
    {
        auto hule_ = hule(Shoupai("s11224488z22557"), "z7=", {});
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ Hupai::QIDUIZI, 2 },
                Hupai{ Hupai::HUNYISE, 3 } }), hule_.hupai);
    }
    // 和了役: 純全帯幺九
    {
        auto hule_ = hule(Shoupai("m11s123p789s789m99"), "m9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::CHUNQUANDAIYAOJIU, 3 } }), hule_.hupai);
    }
    // 和了役: 純全帯幺九(喰い下がり)
    {
        auto hule_ = hule(Shoupai("m11s123p789s78,m999="), "s9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::CHUNQUANDAIYAOJIU, 2 } }), hule_.hupai);
    }
    // 和了役: 純全帯幺九(喰い下がり)
    {
        auto hule_ = hule(Shoupai("m11s123p789s78,m999="), "s9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::CHUNQUANDAIYAOJIU, 2 } }), hule_.hupai);
    }
    // 和了役: 二盃口
    {
        auto hule_ = hule(Shoupai("m223344p667788s9"), "s9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::ERBEIKOU, 3 } }), hule_.hupai);
    }
    // 和了役: 二盃口(4枚使い)
    {
        auto hule_ = hule(Shoupai("m222233334444s9"), "s9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::ERBEIKOU, 3 } }), hule_.hupai);
    }
    // 和了役: 喰い二盃口(役なし)
    {
        auto hule_ = hule(Shoupai("m223344p678s9,p678-"), "s9=", {});
        EXPECT_EQ((std::vector<Hupai>{}), hule_.hupai);
    }
    // 和了役: 清一色
    {
        auto hule_ = hule(Shoupai("m1113456677778"), "m9=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::QINGYISE, 6 } }), hule_.hupai);
    }
    // 和了役: 清一色(喰い下がり)
    {
        auto hule_ = hule(Shoupai("p2344555,p12-3,p7-89"), "p1=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::QINGYISE, 5 } }), hule_.hupai);
    }
    // 和了役: 清一色(七対子形)
    {
        auto hule_ = hule(Shoupai("s1122445577889"), "s9=", {});
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ Hupai::QIDUIZI, 2 },
                Hupai{ Hupai::QINGYISE, 6 } }), hule_.hupai);
    }
    // 和了役: 国士無双
    {
        auto hule_ = hule(Shoupai("m119p19s19z1234567"), {}, {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::GUOSHIWUSHUANG, -1 } }), hule_.hupai);
    }
    // 和了役: 国士無双十三面
    {
        auto hule_ = hule(Shoupai("m19p19s19z1234567m1"), {}, {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::GUOSHIWUSHUANGSHISANMIAN, -2 } }), hule_.hupai);
    }
    // 和了役: 四暗刻
    {
        auto hule_ = hule(Shoupai("m33m111p333s777z111"), {}, {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::SIANKE, -1 } }), hule_.hupai);
    }
    // 和了役: 四暗刻単騎
    {
        auto hule_ = hule(Shoupai("m111p333s777z111m3"), "m3=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::SIANKEDANQI, -2 } }), hule_.hupai);
    }
    // 和了役: 大三元
    {
        auto hule_ = hule(Shoupai("z555m456p22z66,z777+"), "z6=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::DASANYUAN, -1 } }), hule_.hupai);
    }
    // 和了役: 大三元(パオ)
    {
        auto hule_ = hule(Shoupai("m2234,z555-5,z6666,z777+"), "m5=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::DASANYUAN, -1, '+' } }), hule_.hupai);
    }
    // 和了役: 小四喜
    {
        auto hule_ = hule(Shoupai("m234z2244,z333+,z111-"), "z4=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::XIAOSIXI, -1 } }), hule_.hupai);
    }
    // 和了役: 大四喜
    {
        auto hule_ = hule(Shoupai("m22z22244,z333+,z111-"), "z4=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::DASIXI, -2 } }), hule_.hupai);
    }
    // 和了役: 大四喜(パオ)
    {
        auto hule_ = hule(Shoupai("m2,z222+,z4444,z333+,z111-"), "m2=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::DASIXI, -2, '-' } }), hule_.hupai);
    }
    // 和了役: 字一色
    {
        auto hule_ = hule(Shoupai("z1112277,z555=,z444+"), "z7=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::ZIYISE, -1 } }), hule_.hupai);
    }
    // 和了役: 字一色(七対子形)
    {
        auto hule_ = hule(Shoupai("z1122334455667"), "z7=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::ZIYISE, -1 } }), hule_.hupai);
    }
    // 和了役: 緑一色
    {
        auto hule_ = hule(Shoupai("s22334466z66,s888+"), "z6=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::LVYISE, -1 } }), hule_.hupai);
    }
    // 和了役: 緑一色(發なし)
    {
        auto hule_ = hule(Shoupai("s4466,s222=,s333+,s888-"), "s6=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::LVYISE, -1 } }), hule_.hupai);
    }
    // 和了役: 清老頭
    {
        auto hule_ = hule(Shoupai("s11p111m11,s999-,m999="), "m1=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::QINGLAOTOU, -1 } }), hule_.hupai);
    }
    // 和了役: 四槓子
    {
        auto hule_ = hule(Shoupai("m1,z5555,p222+2,p777-7,s1111-"), "m1=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::SIGANGZI, -1 } }), hule_.hupai);
    }
    // 和了役: 九蓮宝燈
    {
        auto hule_ = hule(Shoupai("m1112235678999"), "m4=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::JIULIANBAODENG, -1 } }), hule_.hupai);
    }
    // 和了役: 純正九蓮宝燈
    {
        auto hule_ = hule(Shoupai("m1112345678999"), "m2=", {});
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::CHUNZHENGJIULIANBAODENG, -2 } }), hule_.hupai);
    }

    // ドラ: ドラなし
    {
        Param param;
        param.baopai = { "s1" };
        auto hule_ = hule(Shoupai("p55m234s78,m4-56,z111+"), "s9=", param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::CHANGFENGDONG, 1 } }), hule_.hupai);
    }
    // ドラ: 手牌内: 1
    {
        Param param;
        param.baopai = { "m2" };
        auto hule_ = hule(Shoupai("p55m234s78,m4-56,z111+"), "s9=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ Hupai::CHANGFENGDONG, 1 },
            Hupai{ Hupai::BAOPAI, 1 } }), hule_.hupai);
    }
    // ドラ: 手牌内: 2
    {
        Param param;
        param.baopai = { "p4" };
        auto hule_ = hule(Shoupai("p55m234s78,m4-56,z111+"), "s9=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ Hupai::CHANGFENGDONG, 1 },
            Hupai{ Hupai::BAOPAI, 2 } }), hule_.hupai);
    }
    // ドラ: 手牌内: 1, 副露内: 1
    {
        Param param;
        param.baopai = { "m3" };
        auto hule_ = hule(Shoupai("p55m23s789,m4-56,z111+"), "m4=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ Hupai::CHANGFENGDONG, 1 },
            Hupai{ Hupai::BAOPAI, 2 } }), hule_.hupai);
    }
    // ドラ: 槓ドラ: 1
    {
        Param param;
        param.baopai = { "s1", "m2" };
        auto hule_ = hule(Shoupai("p55m234s78,m4-56,z111+"), "s9=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ Hupai::CHANGFENGDONG, 1 },
            Hupai{ Hupai::BAOPAI, 1 } }), hule_.hupai);
    }
    // ドラ: 赤ドラ: 2
    {
        Param param;
        param.baopai = { "s4" };
        auto hule_ = hule(Shoupai("p50m234s78,m4-06,z111+"), "s9=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ Hupai::CHANGFENGDONG, 1 },
                Hupai{ Hupai::CHIBAOPAI, 2 } }), hule_.hupai);
    }
    // ドラ: 赤のダブドラ
    {
        Param param;
        param.baopai = { "m4" };
        auto hule_ = hule(Shoupai("p55m234s78,m4-06,z111+"), "s9=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ Hupai::CHANGFENGDONG, 1 },
                Hupai{ Hupai::BAOPAI, 1 },
                Hupai{ Hupai::CHIBAOPAI, 1 } }), hule_.hupai);
    }
    // ドラ: ドラ表示牌が赤牌
    {
        Param param;
        param.baopai = { "m0" };
        auto hule_ = hule(Shoupai("p55m234s78,m4-56,z111+"), "s9=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ Hupai::CHANGFENGDONG, 1 },
                Hupai{ Hupai::BAOPAI, 1 } }), hule_.hupai);
    }
    // ドラ: 裏ドラなし
    {
        Param param;
        param.hupai.lizhi = 1;
        param.baopai = { "s9" };
        param.fubaopai = { "s9" };
        auto hule_ = hule(Shoupai("m344556s24678z66*"), "s3=", param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::LIZHI, 1 } }), hule_.hupai);
    }
    // ドラ: 裏ドラ: 1
    {
        Param param;
        param.hupai.lizhi = 1;
        param.baopai = { "s9" };
        param.fubaopai = { "m2" };
        auto hule_ = hule(Shoupai("m344556s24678z66*"), "s3=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ Hupai::LIZHI, 1 },
                Hupai{ Hupai::LIBAOPAI, 1 } }), hule_.hupai);
    }
    // ドラ: ドラ: 1, 裏ドラ: 1
    {
        Param param;
        param.hupai.lizhi = 1;
        param.baopai = { "m2" };
        param.fubaopai = { "m2" };
        auto hule_ = hule(Shoupai("m344556s24678z66*"), "s3=", param);
        EXPECT_EQ((std::vector<Hupai>{
            Hupai{ Hupai::LIZHI, 1 },
                Hupai{ Hupai::BAOPAI, 1 },
                Hupai{ Hupai::LIBAOPAI, 1 } }), hule_.hupai);
    }
    // ドラ: ドラのみでの和了は不可
    {
        Param param;
        param.baopai = { "m2" };
        auto hule_ = hule(Shoupai("m344556s24678z66"), "s3=", param);
        EXPECT_EQ((std::vector<Hupai>{}), hule_.hupai);
    }
    // ドラ: 役満にドラはつかない
    {
        Param param;
        param.baopai = { "m9" };
        auto hule_ = hule(Shoupai("m119p19s19z1234567"), {}, param);
        EXPECT_EQ((std::vector<Hupai>{ Hupai{ Hupai::GUOSHIWUSHUANG, -1 } }), hule_.hupai);
    }

    // 点計算: 20符 2翻 子 ツモ → 400/700
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::MENQIANQINGZIMOHE, 1 },
                Hupai{ Hupai::PINGHE, 1 } },
            20, 2, 0, 1500,
            std::array<int, 4>{ -700,  1500,  -400,  -400 } }),
            hule(Shoupai("z33m123p456s789m234"), {}, {}));
    }
    // 点計算: 20符 3翻 親 ツモ → 1300∀
    {
        Param param;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::MENQIANQINGZIMOHE, 1 },
                Hupai{ Hupai::PINGHE, 1 },
                Hupai{ Hupai::YIBEIKOU, 1 } },
            20, 3, 0, 3900,
            std::array<int, 4>{ 3900, -1300, -1300, -1300 } }),
            hule(Shoupai("z33m123p456s789m231"), {}, param));
    }
    // 点計算: 20符 4翻 子 ツモ → 1300/2600
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::MENQIANQINGZIMOHE, 1 },
                Hupai{ Hupai::PINGHE, 1 },
                Hupai{ Hupai::SANSETONGSHUN, 2 } },
            20, 4, 0, 5200,
            std::array<int, 4>{ -2600,  5200, -1300, -1300 } }),
            hule(Shoupai("z33m123p234s234m234"), {}, {}));
    }
    // 点計算: 25符 2翻 子 ロン → 1600
    {
        Param param;
        param.jicun.lizhibang = 1;
        param.jicun.changbang = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::QIDUIZI, 2 } },
            25, 2, 0, 1600,
            std::array<int, 4>{ -1900,  2900,     0,     0 } }),
            hule(Shoupai("m1122p3344s5566z7"), "z7-", param));
    }
    // 点計算: 25符 3翻 親 ツモ → 1600∀
    {
        Param param;
        param.menfeng = 0;
        param.jicun.lizhibang = 1;
        param.jicun.changbang = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::MENQIANQINGZIMOHE, 1 },
                Hupai{ Hupai::QIDUIZI, 2 } },
            25, 3, 0, 4800,
            std::array<int, 4>{ 6100, -1700, -1700, -1700 } }),
            hule(Shoupai("m1122p3344s5566z77"), {}, param));
    }
    // 点計算: 25符 4翻 子 ツモ → 1600/3200
    {
        Param param;
        param.jicun.lizhibang = 1;
        param.jicun.changbang = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::MENQIANQINGZIMOHE, 1 },
                Hupai{ Hupai::DUANYAOJIU, 1 },
                Hupai{ Hupai::QIDUIZI, 2 } },
            25, 4, 0, 6400,
            std::array<int, 4>{ -3300,  7700, -1700, -1700 } }),
            hule(Shoupai("m2277p3344s556688"), {}, param));
    }
    // 点計算: 30符 1翻 親 ロン → 1500
    {
        Param param;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::DUANYAOJIU, 1 } },
            30, 1, 0, 1500,
            std::array<int, 4>{ 1500,     0, -1500,     0 } }),
            hule(Shoupai("m77234p456s67,m34-5"), "s8=", param));
    }
    // 点計算: 30符 2翻 子 ロン → 2000
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::DUANYAOJIU, 1 },
                Hupai{ Hupai::SANSETONGSHUN, 1 } },
            30, 2, 0, 2000,
            std::array<int, 4>{ -2000,  2000,     0,     0 } }),
            hule(Shoupai("m77234p345s34,m34-5"), "s5-", {}));
    }
    // 点計算: 30符 3翻 親 ツモ → 2000∀
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::MENQIANQINGZIMOHE, 1 },
                Hupai{ Hupai::ZIFENGDONG, 1 },
                Hupai{ Hupai::YIBEIKOU, 1 } },
            30, 3, 0, 6000,
            std::array<int, 4>{ 6000, -2000, -2000, -2000 } }),
            hule(Shoupai("m22z111p445566s789"), {}, param));
    }
    // 点計算: 30符 4翻 子 ツモ → 2000/3900
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::MENQIANQINGZIMOHE, 1 },
                Hupai{ Hupai::CHANGFENGDONG, 1 },
                Hupai{ Hupai::HUNQUANDAIYAOJIU, 2 } },
            30, 4, 0, 7900,
            std::array<int, 4>{ -3900,  7900, -2000, -2000 } }),
            hule(Shoupai("m11z111p123789s789"), {}, {}));
    }
    // 点計算: 40符 1翻 親 ロン → 2000
    {
        Param param;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::YIBEIKOU, 1 } },
            40, 1, 0, 2000,
            std::array<int, 4>{ 2000,     0, -2000,     0 } }),
            hule(Shoupai("m11234234p456s89"), "s7=", param));
    }
    // 点計算: 40符 2翻 子 ロン → 2600
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::DUANYAOJIU, 1 },
                Hupai{ Hupai::YIBEIKOU, 1 } },
            40, 2, 0, 2600,
            std::array<int, 4>{ -2600,  2600,     0,     0 } }),
            hule(Shoupai("m22334455p456s68"), "s7-", {}));
    }
    // 点計算: 40符 3翻 親 ツモ → 2600∀
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 },
                Hupai{ Hupai::DUIDUIHU, 2 } },
            40, 3, 0, 7800,
            std::array<int, 4>{ 7800, -2600, -2600, -2600 } }),
            hule(Shoupai("z33222m222,s222=,p999+"), {}, param));
    }
    // 点計算: 40符 4翻 子 ツモ → 2000/4000
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 },
                Hupai{ Hupai::ZIFENGNAN, 1 },
                Hupai{ Hupai::DUIDUIHU, 2 } },
            40, 4, 0, 8000,
            std::array<int, 4>{ -4000,  8000, -2000, -2000 } }),
            hule(Shoupai("z33222m222,s222=,p999+"), {}, param));
    }
    // 点計算: 50符 1翻 親 ロン → 2400
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 } },
            50, 1, 0, 2400,
            std::array<int, 4>{ 2400,     0, -2400,     0 } }),
            hule(Shoupai("m123p456s789z2227"), "z7=", param));
    }
    // 点計算: 50符 2翻 子 ロン → 3200
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 },
                Hupai{ Hupai::ZIFENGNAN, 1 } },
            50, 2, 0, 3200,
            std::array<int, 4>{ -3200,  3200,     0,     0 } }),
            hule(Shoupai("m123p456s789z2227"), "z7-", param));
    }
    // 点計算: 50符 3翻 親 ツモ → 3200∀
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 },
                Hupai{ Hupai::SANANKE, 2 } },
            50, 3, 0, 9600,
            std::array<int, 4>{ 9600, -3200, -3200, -3200 } }),
            hule(Shoupai("z33m222z222,p8888,s789-"), {}, param));
    }
    // 点計算: 50符 4翻 子 ツモ → 2000/4000
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 },
                Hupai{ Hupai::ZIFENGNAN, 1 },
                Hupai{ Hupai::SANANKE, 2 } },
            50, 4, 0, 8000,
            std::array<int, 4>{ -4000,  8000, -2000, -2000 } }),
            hule(Shoupai("z33m222z222,p8888,s789-"), {}, param));
    }
    // 点計算: 60符 1翻 親 ロン → 2900
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 } },
            60, 1, 0, 2900,
            std::array<int, 4>{ 2900,     0, -2900,     0 } }),
            hule(Shoupai("s789z2227,m2222,p111="), "z7=", param));
    }
    // 点計算: 60符 2翻 子 ロン → 3900
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 },
                Hupai{ Hupai::ZIFENGNAN, 1 } },
            60, 2, 0, 3900,
            std::array<int, 4>{ -3900,  3900,     0,     0 } }),
            hule(Shoupai("s789z2227,m2222,p111="), "z7-", param));
    }
    // 点計算: 60符 3翻 親 ツモ → 3900∀
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 },
                Hupai{ Hupai::HUNYISE, 2 } },
            60, 3, 0, 11700,
            std::array<int, 4>{ 11700, -3900, -3900, -3900 } }),
            hule(Shoupai("m11222789,z2222,m444="), {}, param));
    }
    // 点計算: 60符 4翻 子 ツモ → 2000/4000
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 },
                Hupai{ Hupai::ZIFENGNAN, 1 },
                Hupai{ Hupai::HUNYISE, 2 } },
            60, 4, 0, 8000,
            std::array<int, 4>{ -4000,  8000, -2000, -2000 } }),
            hule(Shoupai("m11222789,z2222,m444="), {}, param));
    }
    // 点計算: 70符 1翻 親 ロン → 3400
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 } },
            70, 1, 0, 3400,
            std::array<int, 4>{ 3400,     0, -3400,     0 } }),
            hule(Shoupai("m12377p456s78,z2222"), "s9=", param));
    }
    // 点計算: 70符 2翻 子 ロン → 4500
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 },
                Hupai{ Hupai::ZIFENGNAN, 1 } },
            70, 2, 0, 4500,
            std::array<int, 4>{ -4500,  4500,     0,     0 } }),
            hule(Shoupai("m12377p456s78,z2222"), "s9-", param));
    }
    // 点計算: 70符 3翻 親 ツモ → 4000∀
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::MENQIANQINGZIMOHE, 1 },
                Hupai{ Hupai::CHANGFENGNAN, 1 },
                Hupai{ Hupai::YIBEIKOU, 1 } },
            70, 3, 0, 12000,
            std::array<int, 4>{ 12000, -4000, -4000, -4000 } }),
            hule(Shoupai("p77s223344,z2222,m2222"), {}, param));
    }
    // 点計算: 80符 1翻 親 ロン → 3900
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 } },
            80, 1, 0, 3900,
            std::array<int, 4>{ 3900,     0, -3900,     0 } }),
            hule(Shoupai("m22s888p34,z222+2,z4444"), "p5=", param));
    }
    // 点計算: 80符 2翻 子 ロン → 5200
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 },
                Hupai{ Hupai::ZIFENGNAN, 1 } },
            80, 2, 0, 5200,
            std::array<int, 4>{ -5200,  5200,     0,     0 } }),
            hule(Shoupai("m22s888p34,z222+2,z4444"), "p5-", param));
    }
    // 点計算: 80符 3翻 親 ツモ → 4000∀
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 },
                Hupai{ Hupai::ZIFENGDONG, 1 },
                Hupai{ Hupai::HUNQUANDAIYAOJIU, 1 } },
            80, 3, 0, 12000,
            std::array<int, 4>{ 12000, -4000, -4000, -4000 } }),
            hule(Shoupai("m11p999s123,z222+2,z1111"), {}, param));
    }
    // 点計算: 90符 1翻 親 ロン → 4400
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 } },
            90, 1, 0, 4400,
            std::array<int, 4>{ 4400,     0, -4400,     0 } }),
            hule(Shoupai("p88m123s99,s6666,z2222"), "s9=", param));
    }
    // 点計算: 90符 2翻 子 ロン → 5800
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 },
                Hupai{ Hupai::ZIFENGNAN, 1 } },
            90, 2, 0, 5800,
            std::array<int, 4>{ -5800,  5800,     0,     0 } }),
            hule(Shoupai("p88m123s99,s6666,z2222"), "s9-", param));
    }
    // 点計算: 90符 3翻 親 ツモ → 4000∀
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 },
                Hupai{ Hupai::FANPAIBAI, 1 },
                Hupai{ Hupai::FANPAIFA, 1 } },
            90, 3, 0, 12000,
            std::array<int, 4>{ 12000, -4000, -4000, -4000 } }),
            hule(Shoupai("m22s345,z5555,z2222,z666-"), {}, param));
    }
    // 点計算: 100符 1翻 親 ロン → 4800
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 } },
            100, 1, 0, 4800,
            std::array<int, 4>{ 4800,     0, -4800,     0 } }),
            hule(Shoupai("m22p345s67,z2222,s9999"), "s8=", param));
    }
    // 点計算: 100符 2翻 子 ロン → 6400
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 },
                Hupai{ Hupai::ZIFENGNAN, 1 } },
            100, 2, 0, 6400,
            std::array<int, 4>{ -6400,  6400,     0,     0 } }),
            hule(Shoupai("m22p345s67,z2222,s9999"), "s8-", param));
    }
    // 点計算: 100符 3翻 親 ツモ → 4000∀
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::MENQIANQINGZIMOHE, 1 },
                Hupai{ Hupai::SANANKE, 2 } },
            100, 3, 0, 12000,
            std::array<int, 4>{ 12000, -4000, -4000, -4000 } }),
            hule(Shoupai("z11m999p243,s1111,s9999"), {}, param));
    }
    // 点計算: 110符 1翻 親 ロン → 5300
    {
        Param param;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::FANPAIZHONG, 1 } },
            110, 1, 0, 5300,
            std::array<int, 4>{ 5300,     0, -5300,     0 } }),
            hule(Shoupai("m234z1177,p1111,s9999"), "z7=", param));
    }
    // 点計算: 110符 2翻 子 ロン → 7100
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::FANPAIBAI, 1 },
                Hupai{ Hupai::FANPAIZHONG, 1 } },
            110, 2, 0, 7100,
            std::array<int, 4>{ -7100,  7100,     0,     0 } }),
            hule(Shoupai("m234z2277,p1111,z5555"), "z7-", param));
    }
    // 点計算: 110符 3翻 親 ツモ → 4000∀
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::FANPAIBAI, 1 },
                Hupai{ Hupai::SANGANGZI, 2 } },
            110, 3, 0, 12000,
            std::array<int, 4>{ 12000, -4000, -4000, -4000 } }),
            hule(Shoupai("m243z11,p1111,s9999,z555+5"), {}, param));
    }
    // 点計算: 5翻 親 ロン → 12000
    {
        Param param;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::PINGHE, 1 },
                Hupai{ Hupai::DUANYAOJIU, 1 },
                Hupai{ Hupai::YIBEIKOU, 1 },
                Hupai{ Hupai::SANSETONGSHUN, 2 } },
            30, 5, 0, 12000,
            std::array<int, 4>{ 12000,     0,-12000,     0 } }),
            hule(Shoupai("m22456p456s44556"), "s6=", param));
    }
    // 点計算: 6翻 子 ツモ → 3000/6000
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::MENQIANQINGZIMOHE, 1 },
                Hupai{ Hupai::PINGHE, 1 },
                Hupai{ Hupai::DUANYAOJIU, 1 },
                Hupai{ Hupai::YIBEIKOU, 1 },
                Hupai{ Hupai::SANSETONGSHUN, 2 } },
            20, 6, 0, 12000,
            std::array<int, 4>{ -6000, 12000, -3000, -3000 } }),
            hule(Shoupai("m22456p456s445566"), {}, {}));
    }
    // 点計算: 7翻 親 ロン → 18000
    {
        Param param;
        param.zhuangfeng = 1;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 },
                Hupai{ Hupai::DUIDUIHU, 2 },
                Hupai{ Hupai::HUNLAOTOU, 2 },
                Hupai{ Hupai::HUNYISE, 2 } },
            50, 7, 0, 18000,
            std::array<int, 4>{ 18000,     0,-18000,     0 } }),
            hule(Shoupai("m111z3334,z222=,m999-"), "z4=", param));
    }
    // 点計算: 8翻 子 ツモ → 4000/8000
    {
        Param param;
        param.zhuangfeng = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::CHANGFENGNAN, 1 },
                Hupai{ Hupai::ZIFENGNAN, 1 },
                Hupai{ Hupai::DUIDUIHU, 2 },
                Hupai{ Hupai::HUNLAOTOU, 2 },
                Hupai{ Hupai::HUNYISE, 2 } },
            50, 8, 0, 16000,
            std::array<int, 4>{ -8000, 16000, -4000, -4000 } }),
            hule(Shoupai("m111z333444,z222=,m999-"), {}, param));
    }
    // 点計算: 9翻 親 ロン → 24000
    {
        Param param;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::DUANYAOJIU, 1 },
                Hupai{ Hupai::SANANKE, 2 },
                Hupai{ Hupai::QINGYISE, 6 } },
            50, 9, 0, 24000,
            std::array<int, 4>{ 24000,     0,-24000,     0 } }),
            hule(Shoupai("s2223334455567"), "s8=", param));
    }
    // 点計算: 10翻 子 ツモ → 4000/8000
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::MENQIANQINGZIMOHE, 1 },
                Hupai{ Hupai::DUANYAOJIU, 1 },
                Hupai{ Hupai::SANANKE, 2 },
                Hupai{ Hupai::QINGYISE, 6 } },
            40, 10, 0, 16000,
            std::array<int, 4>{ -8000, 16000, -4000, -4000 } }),
            hule(Shoupai("s22233344555678"), {}, {}));
    }
    // 点計算: 11翻 親 ロン → 36000
    {
        Param param;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::PINGHE, 1 },
                Hupai{ Hupai::DUANYAOJIU, 1 },
                Hupai{ Hupai::ERBEIKOU, 3 },
                Hupai{ Hupai::QINGYISE, 6 } },
            30, 11, 0, 36000,
            std::array<int, 4>{ 36000,     0,-36000,     0 } }),
            hule(Shoupai("p2233445566778"), "p8=", param));
    }
    // 点計算: 12翻 子 ツモ → 6000/12000
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::MENQIANQINGZIMOHE, 1 },
                Hupai{ Hupai::PINGHE, 1 },
                Hupai{ Hupai::DUANYAOJIU, 1 },
                Hupai{ Hupai::ERBEIKOU, 3 },
                Hupai{ Hupai::QINGYISE, 6 } },
            20, 12, 0, 24000,
            std::array<int, 4>{ -12000, 24000, -6000, -6000 } }),
            hule(Shoupai("p22334455667788"), {}, {}));
    }
    // 点計算: 13翻 親 ロン → 48000
    {
        Param param;
        param.menfeng = 0;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::PINGHE, 1 },
                Hupai{ Hupai::CHUNQUANDAIYAOJIU, 3 },
                Hupai{ Hupai::ERBEIKOU, 3 },
                Hupai{ Hupai::QINGYISE, 6 } },
            30, 13, 0, 48000,
            std::array<int, 4>{ 48000,     0,-48000,     0 } }),
            hule(Shoupai("m1177778888999"), "m9=", param));
    }
    // 点計算: 役満複合 子 ツモ → 24000/48000
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::DASIXI, -2 },
                Hupai{ Hupai::ZIYISE, -1 } },
            0, 0, 3, 96000,
            std::array<int, 4>{ -48000, 96000,-24000,-24000 } }),
            hule(Shoupai("z77111z444,z222+,z333-"), {}, {}));
    }
    // 点計算: 役満パオ 放銃者なし、責任払い
    {
        Param param;
        param.menfeng = 0;
        param.jicun.lizhibang = 1;
        param.jicun.changbang = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::DASANYUAN, -1, '-' } },
            0, 0, 1, 48000,
            std::array<int, 4>{ 49300,     0,     0,-48300 } }),
            hule(Shoupai("m11p456,z555+,z666=,z777-"), {}, param));
    }
    // 点計算: 役満パオ 放銃者あり、放銃者と折半
    {
        Param param;
        param.menfeng = 0;
        param.jicun.lizhibang = 1;
        param.jicun.changbang = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::DASANYUAN, -1, '-'} },
            0, 0, 1, 48000,
            std::array<int, 4>{ 49300,     0,-24300,-24000 } }),
            hule(Shoupai("m11p45,z555+,z666=,z777-"), "p6=", param));
    }
    // 点計算: 役満パオ パオが放銃、全額責任払い
    {
        Param param;
        param.menfeng = 0;
        param.jicun.lizhibang = 1;
        param.jicun.changbang = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::DASANYUAN, -1, '-' } },
            0, 0, 1, 48000,
            std::array<int, 4>{ 49300,     0,     0,-48300 } }),
            hule(Shoupai("m11p45,z555+,z666=,z777-"), "p6-", param));
    }
    // 点計算: ダブル役満パオ 放銃者なし、関連役満のみ責任払い
    {
        Param param;
        param.jicun.lizhibang = 1;
        param.jicun.changbang = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::DASIXI, -2, '+' },
                Hupai{ Hupai::ZIYISE, -1 } },
            0, 0, 3, 96000,
            std::array<int, 4>{ -16100, 97300,-72100, -8100 } }),
            hule(Shoupai("z77,z111-,z2222,z333=3,z444+"), {}, param));
    }
    // 点計算: ダブル役満パオ 放銃者あり、関連役満のみ放銃者と折半
    {
        Param param;
        param.jicun.lizhibang = 1;
        param.jicun.changbang = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::DASIXI, -2, '+' },
                Hupai{ Hupai::ZIYISE, -1 } },
            0, 0, 3, 96000,
            std::array<int, 4>{ -64300, 97300,-32000,     0 } }),
            hule(Shoupai("z7,z111-,z2222,z333=3,z444+"), "z7-", param));
    }
    // 点計算: ダブル役満パオ パオが放銃、全額責任払い
    {
        Param param;
        param.jicun.lizhibang = 1;
        param.jicun.changbang = 1;
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::DASIXI, -2, '+' },
                Hupai{ Hupai::ZIYISE, -1 } },
            0, 0, 3, 96000,
            std::array<int, 4>{ 0, 97300,-96300,     0 } }),
            hule(Shoupai("z7,z111-,z2222,z333=3,z444+"), "z7+", param));
    }
    // 高点法: 七対子と二盃口の選択
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::DUANYAOJIU, 1 },
                Hupai{ Hupai::ERBEIKOU, 3 } },
            40, 4, 0, 8000,
            std::array<int, 4>{ 0,  8000,    0, -8000 } }),
            hule(Shoupai("m223344p556677s8"), "s8=", {}));
    }
    // 高点法: 雀頭候補2つの選択
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::DUANYAOJIU, 1 },
                Hupai{ Hupai::YIBEIKOU, 1 },
                Hupai{ Hupai::SANSETONGSHUN, 2 } },
            40, 4, 0, 8000,
            std::array<int, 4>{ 0,  8000,    0, -8000 } }),
            hule(Shoupai("m2234455p234s234"), "m3=", {}));
    }
    // 高点法: 順子と刻子の選択
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::YIBEIKOU, 1 },
                Hupai{ Hupai::CHUNQUANDAIYAOJIU, 3 } },
            40, 4, 0, 8000,
            std::array<int, 4>{ 0,  8000,    0, -8000 } }),
            hule(Shoupai("m111222333p8999"), "p7=", {}));
    }
    // 高点法: 嵌張待ち両面待ちの選択
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::FANPAIZHONG, 1 } },
            50, 1, 0, 1600,
            std::array<int, 4>{ 0,  1600,    0, -1600 } }),
            hule(Shoupai("m12334p567z11z777"), "m2=", {}));
    }
    // 高点法: 得点が同じ場合は翻数が多い方を選択
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::PINGHE, 1 },
                Hupai{ Hupai::YIBEIKOU, 1 },
                Hupai{ Hupai::CHUNQUANDAIYAOJIU, 3 } },
            30, 5, 0, 8000,
            std::array<int, 4>{ 0,  8000,    0, -8000 } }),
            hule(Shoupai("m111222333p7899"), "p9=", {}));
    }
    // 高点法: 得点・翻数が同じ場合は符が多い方を選択
    {
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::SANANKE, 2 },
                Hupai{ Hupai::QINGYISE, 6 } },
            50, 8, 0, 16000,
            std::array<int, 4>{ 0, 16000,    0,-16000 } }),
            hule(Shoupai("s1112223335578"), "s9=", {}));
    }
    // 高点法: 役満と数え役満では役満を選択
    {
        Param param;
        param.hupai.lizhi = 1;
        param.hupai.yifa = true;
        param.baopai = { "m2" };
        EXPECT_EQ((Defen{ std::vector<Hupai>{
                Hupai{ Hupai::JIULIANBAODENG, -1 } },
            0, 0, 1, 32000,
            std::array<int, 4>{ -16000, 32000, -8000, -8000 } }),
            hule(Shoupai("m11123457899996"), {}, param));
    }
}

TEST(HuleTest, hule_json) {
    // 和了点計算: 10000パターン
    std::ifstream ifs("../../tests_cpp/data/hule.json");
    nlohmann::json data;
    ifs >> data;
    for (const auto& t : data) {
        Param param;
        const auto& in = t["in"];
        const auto& in_param = in["param"];
        param.zhuangfeng = in_param["zhuangfeng"].get<int>();
        param.menfeng = in_param["menfeng"].get<int>();
        const auto& in_hupai = in_param["hupai"];
        if (in_hupai.contains("lizhi")) param.hupai.lizhi = in_hupai["lizhi"].get<int>();
        if (in_hupai.contains("yifa")) param.hupai.yifa = in_hupai["yifa"].get<bool>();
        if (in_hupai.contains("lingshang")) param.hupai.lingshang = in_hupai["lingshang"].get<bool>();
        if (in_hupai.contains("haidi")) param.hupai.haidi = in_hupai["haidi"].get<int>();
        param.baopai = in_param["baopai"].get<std::vector<std::string>>();
        param.fubaopai = in_param["fubaopai"].get<std::vector<std::string>>();
        const auto& in_jicun = in_param["jicun"];
        if (in_jicun.contains("changbang")) param.jicun.changbang = in_jicun["changbang"].get<int>();
        if (in_jicun.contains("lizhibang")) param.jicun.lizhibang = in_jicun["lizhibang"].get<int>();

        std::string rongpai;
        if (in.contains("rongpai")) rongpai = in["rongpai"].get<std::string>();

        const auto& out = t["out"];
        Defen defen;
        for (const auto& out_hupai : out["hupai"]) {
            const auto name = out_hupai["name"].get<std::string>();
            int fanshu;
            if (out_hupai["fanshu"].is_string())
                fanshu = -(int)out_hupai["fanshu"].get<std::string>().size();
            else
                fanshu = out_hupai["fanshu"].get<int>();
            defen.hupai.emplace_back(HUPAI_MAP[name], fanshu);
        }
        defen.fu = out["fu"].is_null() ? 0 : out["fu"].get<int>();
        defen.fanshu = out["fanshu"].is_null() ? 0 : out["fanshu"].get<int>();
        defen.damanguan = out["damanguan"].is_null() ? 0 : out["damanguan"].get<int>();
        defen.defen = out["defen"].get<int>();
        defen.fenpei = out["fenpei"].get<std::array<int, 4>>();

        EXPECT_EQ(defen, hule(Shoupai(in["shoupai"].get<std::string>()), rongpai, param));
    }
}
