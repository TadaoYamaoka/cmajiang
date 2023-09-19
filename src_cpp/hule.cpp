﻿#include "hule.h"

#include <cmath>
#include <numeric>


static const std::vector<int> guoshi_zipai_n = {1, 2, 3, 4, 5, 6, 7};
static const std::vector<int> guoshi_yaojiu_n = { 1, 9 };

const std::regex re_add_hulepai{ R"([\+\=\-]|\d{4})" };

const std::regex re_sanyuanpai{ R"(^z[567].*$)" };
const std::regex re_yaojiu{ R"(^.*[z19].*$)" };
const std::regex re_zipai{ R"(^z.*$)" };
const std::regex re_kezi{ R"(^[mpsz](\d)\1\1.*$)" };
const std::regex re_ankezi{ R"(^[mpsz](\d)\1\1(?:\1|_\!)?$)" };
const std::regex re_gangzi{ R"(^[mpsz](\d)\1\1.*\1.*$)" };
const std::regex re_danqi{ R"(^[mpsz](\d)\1[\+\=\-\_]\!$)" };
const std::regex re_kanzhang{ R"(^[mps]\d\d[\+\=\-\_]\!\d$)" };
const std::regex re_bianzhang{ R"(^[mps](123[\+\=\-\_]\!|7[\+\=\-\_]\!89)$)" };

const std::regex re_get_hudi1{ R"([\+\=\-](?!\!))" };
const std::regex re_get_hudi2{ R"([\+\=\-]\!)" };

const std::regex re_xiaosanyuan{ R"(^z[567])" };
const std::regex re_dasanyuan{ R"(^z([567])\1\1(?:[\+\=\-]|\1)(?!\!))" };
const std::regex re_sixihu1{ R"(^z([1234])\1\1(?:[\+\=\-]|\1)(?!\!))" };
const std::regex re_sixihu2{ R"(^z[1234])" };
const std::regex re_lvyise1{ R"(^[mp])" };
const std::regex re_lvyise2{ R"(^z[^6])" };
const std::regex re_lvyise3{ R"(^s.*[1579])" };
const std::regex re_jiulianbaodeng{ R"(^[mpsz]1112345678999)" };

const std::regex re_get_post_hupai{ R"([mpsz][^mpsz,]*)" };


template<typename T>
T&& concat(T& m1, T& m2) {
	m1.insert(m1.end(), std::make_move_iterator(m2.begin()), std::make_move_iterator(m2.end()));
	return std::move(m1);
}
template<typename T>
T&& concat(T& m1, T&& m2) {
	m1.insert(m1.end(), std::make_move_iterator(m2.begin()), std::make_move_iterator(m2.end()));
	return std::move(m1);
}
template<typename T>
T&& concat(T&& m1, T& m2) {
	m1.insert(m1.end(), std::make_move_iterator(m2.begin()), std::make_move_iterator(m2.end()));
	return std::move(m1);
}
template<typename T>
T&& concat(T&& m1, T&& m2) {
	m1.insert(m1.end(), std::make_move_iterator(m2.begin()), std::make_move_iterator(m2.end()));
	return std::move(m1);
}

// 面子
std::vector<std::vector<std::string>> mianzi(const char s, std::vector<int>& bingpai, const int n = 1) {
	if (n > 9) return std::vector<std::vector<std::string>>(1);

	if (bingpai[n] == 0) return mianzi(s, bingpai, n + 1);

	std::vector<std::vector<std::string>> shunzi;
	if (n <= 7 && bingpai[n] > 0 && bingpai[n + 1] > 0 && bingpai[n + 2] > 0) {
		bingpai[n]--; bingpai[n + 1]--; bingpai[n + 2]--;
		shunzi = mianzi(s, bingpai, n);
		bingpai[n]++; bingpai[n + 1]++; bingpai[n + 2]++;
		for (auto& s_mianzi : shunzi) {
			s_mianzi.emplace(s_mianzi.begin(), to_string(s, n, n + 1, n + 2));
		}
	}

	std::vector<std::vector<std::string>> kezi;
	if (bingpai[n] == 3) {
		bingpai[n] -= 3;
		kezi = mianzi(s, bingpai, n + 1);
		bingpai[n] += 3;
		for (auto& k_mianzi : kezi) {
			k_mianzi.emplace(k_mianzi.begin(), to_string(s, n, n, n));
		}
	}

	return concat(shunzi, kezi);
}

std::vector<std::vector<std::string>> mianzi_all(const Shoupai& shoupai) {
	std::vector<std::vector<std::string>> shupai_all(1);
	for (const auto s : { 'm', 'p', 's' }) {
		std::vector<std::vector<std::string>> new_mianzi;
		for (auto& mm : shupai_all) {
			auto bingpai = shoupai.bingpai(s);
			for (auto& nn : mianzi(s, bingpai)) {
				new_mianzi.emplace_back(concat(mm, nn));
			}
		}
		shupai_all = std::move(new_mianzi);
	}

	std::vector<std::string> zipai;
	for (int n = 1; n <= 7; n++) {
		if (shoupai.z(n) == 0) continue;
		if (shoupai.z(n) != 3) return {};
		zipai.emplace_back(to_string('z', n, n, n));
	}

	std::vector<std::string> fulou;
	fulou.reserve(shoupai.fulou().size());
	for (const auto& m : shoupai.fulou()) {
		fulou.emplace_back(std::regex_replace(m, re_ling, "5"));
	}

	for (auto& shupai : shupai_all) {
		concat(concat(shupai, zipai), fulou);
	}

	return shupai_all;
}

// 和了牌をマーク
std::vector<std::vector<std::string>> add_hulepai(const std::vector<std::string>& mianzi, const std::string& p) {
	const char s = p[0];
	const char n = p[1];
	const char d = p[2];
	std::regex regexp{ std::string("(") + s + ".*" + n + ")" };
	std::string replacer = std::string("$1") + d + "!";

	std::vector<std::vector<std::string>> new_mianzi;

	for (int i = 0; i < mianzi.size(); i++) {
		if (std::regex_search(mianzi[i], re_add_hulepai)) continue;
		if (i > 0 && mianzi[i] == mianzi[i - 1]) continue;
		auto m = std::regex_replace(mianzi[i], regexp, replacer);
		if (m == mianzi[i]) continue;
		auto tmp_mianzi = mianzi;
		tmp_mianzi[i] = m;
		new_mianzi.emplace_back(std::move(tmp_mianzi));
	}

	return new_mianzi;
}

// 一般形(4面子1雀頭)
std::vector<std::vector<std::string>> hule_mianzi_yiban(Shoupai& shoupai, const std::string& hulepai) {
	std::vector<std::vector<std::string>> mianzi;

	for (const auto s : { 'm', 'p', 's', 'z' }) {
		auto& bingpai = shoupai._bingpai(s);
		for (int n = 1; n < bingpai.size(); n++) {
			if (bingpai[n] < 2) continue;
			bingpai[n] -= 2;
			auto jiangpai = to_string(s, n, n);
			for (auto& mm : mianzi_all(shoupai)) {
				mm.emplace(mm.begin(), jiangpai);
				if (mm.size() != 5) continue;
				concat(mianzi, add_hulepai(mm, hulepai));
			}
			bingpai[n] += 2;
		}
	}

	return mianzi;
}

// 七対子形
std::vector<std::vector<std::string>> hule_mianzi_qidui(const Shoupai& shoupai, const std::string& hulepai) {
	if (shoupai.fulou().size() > 0) return {};

	std::vector<std::string> mianzi;

	for (const auto s : { 'm', 'p', 's', 'z' }) {
		const auto& bingpai = shoupai.bingpai(s);
		for (int n = 1; n < bingpai.size(); n++) {
			if (bingpai[n] == 0) continue;
			if (bingpai[n] == 2) {
				auto m = (to_string(s, n) == hulepai.substr(0, 2))
					? to_string(s, n, n) + hulepai[2] + '!'
					: to_string(s, n, n);
				mianzi.emplace_back(std::move(m));
			}
			else return {};
		}
	}

	if (mianzi.size() == 7)
		return std::vector<std::vector<std::string>>{ std::move(mianzi) };
	else
		return {};
}

// 国士無双形
std::vector<std::vector<std::string>> hule_mianzi_guoshi(const Shoupai& shoupai, const std::string& hulepai) {
	if (shoupai.fulou().size() > 0) return {};

	std::vector<std::string> mianzi;
	int n_duizi = 0;

	for (const auto s : { 'm', 'p', 's', 'z' }) {
		const auto& bingpai = shoupai.bingpai(s);
		for (const auto n : (s == 'z') ? guoshi_zipai_n : guoshi_yaojiu_n) {
			if (bingpai[n] == 2) {
				auto m = (to_string(s, n) == hulepai.substr(0, 2))
					? to_string(s, n, n) + hulepai[2] + '!'
					: to_string(s, n, n);
				mianzi.emplace(mianzi.begin(), std::move(m));
				n_duizi++;
			}
			else if (bingpai[n] == 1) {
				auto m = (to_string(s, n) == hulepai.substr(0, 2))
					? to_string(s, n) + hulepai[2] + '!'
					: to_string(s, n);
				mianzi.emplace_back(std::move(m));
			}
			else return {};
		}
	}

	if (n_duizi == 1)
		return std::vector<std::vector<std::string>>{ std::move(mianzi) };
	else
		return {};
}

// 九蓮宝燈形
std::vector<std::vector<std::string>> hule_mianzi_jiulian(const Shoupai& shoupai, const std::string& hulepai) {
	if (shoupai.fulou().size() > 0) return {};

	const auto s = hulepai[0];
	if (s == 'z') return {};

	std::string mianzi(1, s);
	const auto& bingpai = shoupai.bingpai(s);
	for (int n = 1; n <= 9; n++) {
		if (bingpai[n] == 0) return {};
		if ((n == 1 || n == 9) && bingpai[n] < 3) return {};
		const auto n_pai = (n == to_int(hulepai[1])) ? bingpai[n] - 1 : bingpai[n];
		for (int i = 0; i < n_pai; i++) {
			mianzi += to_char(n);
		}
	}
	if (mianzi.size() != 14) return {};
	mianzi += hulepai.substr(1) + '!';

	return std::vector<std::vector<std::string>>{ std::vector<std::string>{ std::move(mianzi) } };
}

// 和了形
std::vector<std::vector<std::string>> hule_mianzi(const Shoupai& shoupai, const std::string& rongpai) {

	auto new_shoupai = shoupai;
	if (!rongpai.empty()) new_shoupai.zimo(rongpai);

	if (new_shoupai.zimo().empty() || new_shoupai.zimo().size() > 2) return {};
	const auto hulepai = std::regex_replace(rongpai.empty() ? new_shoupai.zimo() + '_': rongpai, re_ling, "5");

	return concat(concat(concat(
		hule_mianzi_yiban(new_shoupai, hulepai),
		hule_mianzi_qidui(new_shoupai, hulepai)),
		hule_mianzi_guoshi(new_shoupai, hulepai)),
		hule_mianzi_jiulian(new_shoupai, hulepai));
}

// 胡底(符計算の際に得た面子の情報)
struct Hudi {
	int fu = 20;
	bool menqian = true;
	bool zimo = true;
	std::vector<int> shunzi_m = { 0,0,0,0,0,0,0,0 };
	std::vector<int> shunzi_p = { 0,0,0,0,0,0,0,0 };
	std::vector<int> shunzi_s = { 0,0,0,0,0,0,0,0 };
	std::vector<int> kezi_m = { 0,0,0,0,0,0,0,0,0,0 };
	std::vector<int> kezi_p = { 0,0,0,0,0,0,0,0,0,0 };
	std::vector<int> kezi_s = { 0,0,0,0,0,0,0,0,0,0 };
	std::vector<int> kezi_z = { 0,0,0,0,0,0,0,0 };
	int n_shunzi = 0;
	int n_kezi = 0;
	int n_ankezi = 0;
	int n_gangzi = 0;
	int n_yaojiu = 0;
	int n_zipai = 0;
	int danqi = false;
	int pinghu = false;
	int zhuangfeng;
	int menfeng;

	Hudi(const int zhuangfeng, const int menfeng) : zhuangfeng(zhuangfeng), menfeng(menfeng) {}
	std::vector<int>& shunzi(const char s) {
		switch (s) {
		case 'm':
			return shunzi_m;
		case 'p':
			return shunzi_p;
		case 's':
			return shunzi_s;
		default:
			throw std::invalid_argument(std::string(1, s));
		}
	}
	const std::vector<int>& shunzi(const char s) const {
		return const_cast<Hudi&>(*this).shunzi(s);
	}
	std::vector<int>& kezi(const char s) {
		switch (s) {
		case 'm':
			return kezi_m;
		case 'p':
			return kezi_p;
		case 's':
			return kezi_s;
		case 'z':
			return kezi_z;
		default:
			throw std::invalid_argument(std::string(1, s));
		}
	}
	const std::vector<int>& kezi(const char s) const {
		return const_cast<Hudi&>(*this).kezi(s);
	}
};

Hudi get_hudi(const std::vector<std::string>& mianzi, const int zhuangfeng, const int menfeng, const Rule& rule) {
	const std::regex re_zhuangfengpai{ "^z" + std::to_string(zhuangfeng + 1) + ".*$" };
	const std::regex re_menfengpai{ "^z" + std::to_string(menfeng + 1) + ".*$" };

	Hudi hudi(zhuangfeng, menfeng);

	for (const auto& m : mianzi) {
		if (std::regex_search(m, re_get_hudi1)) hudi.menqian = false;
		if (std::regex_search(m, re_get_hudi2)) hudi.zimo = false;

		if (mianzi.size() == 1) continue;

		if (std::regex_match(m, re_danqi)) hudi.danqi = true;

		if (mianzi.size() == 13) continue;

		if (std::regex_match(m, re_yaojiu)) hudi.n_yaojiu++;
		if (std::regex_match(m, re_zipai))  hudi.n_zipai++;

		if (mianzi.size() != 5) continue;

		if (m == mianzi[0]) {
			int fu = 0;
			if (std::regex_match(m, re_zhuangfengpai)) fu += 2;
			if (std::regex_match(m, re_menfengpai))    fu += 2;
			if (std::regex_match(m, re_sanyuanpai))    fu += 2;
			// 連風牌は2符
			fu = rule.doubleWindTileScore && fu > 2 ? 2 : fu;
			hudi.fu += fu;
			if (hudi.danqi) hudi.fu += 2;
		}
		else if (std::regex_match(m, re_kezi)) {
			hudi.n_kezi++;
			int fu = 2;
			if (std::regex_match(m, re_yaojiu)) { fu *= 2; }
			if (std::regex_match(m, re_ankezi)) { fu *= 2; hudi.n_ankezi++; }
			if (std::regex_match(m, re_gangzi)) { fu *= 4; hudi.n_gangzi++; }
			hudi.fu += fu;
			hudi.kezi(m[0])[to_int(m[1])]++;
		}
		else {
			hudi.n_shunzi++;
			if (std::regex_match(m, re_kanzhang))  hudi.fu += 2;
			if (std::regex_match(m, re_bianzhang)) hudi.fu += 2;
			hudi.shunzi(m[0])[to_int(m[1])]++;
		}
	}

	if (mianzi.size() == 7) {
		hudi.fu = 25;
	}
	else if (mianzi.size() == 5) {
		hudi.pinghu = (hudi.menqian && hudi.fu == 20);
		if (hudi.zimo) {
			if (!hudi.pinghu)      hudi.fu += 2;
		}
		else {
			if (hudi.menqian)       hudi.fu += 10;
			else if (hudi.fu == 20) hudi.fu = 30;
		}
		hudi.fu = (int)std::ceil(hudi.fu / 10.0) * 10;
	}

	return hudi;
}

std::vector<Hupai> get_pre_hupai(const Param::Hupai& hupai) {

	std::vector<Hupai> pre_hupai;

	if (hupai.lizhi == 1)   pre_hupai.emplace_back("立直", 1);
	if (hupai.lizhi == 2)   pre_hupai.emplace_back("ダブル立直", 2);
	if (hupai.yifa)         pre_hupai.emplace_back("一発", 1);
	if (hupai.haidi == 1)   pre_hupai.emplace_back("海底摸月", 1);
	if (hupai.haidi == 2)   pre_hupai.emplace_back("河底撈魚", 1);
	if (hupai.lingshang)    pre_hupai.emplace_back("嶺上開花", 1);
	if (hupai.qianggang)    pre_hupai.emplace_back("槍槓", 1);

	if (hupai.tianhu == 1)  pre_hupai = { Hupai{ "天和", -1 } };
	if (hupai.tianhu == 2)  pre_hupai = { Hupai{ "地和", -1 } };

	return pre_hupai;
}

std::vector<Hupai> get_hupai(const std::vector<std::string>& mianzi, const Hudi& hudi, const std::vector<Hupai>& pre_hupai, const std::vector<Hupai>& post_hupai, const Rule& rule) {

	const auto menqianqing = [&hudi]() {
		if (hudi.menqian && hudi.zimo)
			return std::vector<Hupai>{ Hupai{ "門前清自摸和", 1 } };
		return std::vector<Hupai>{};
		};
	const auto fanpai = [&hudi]() {
		const std::string feng_hanzi[] = { "東", "南", "西", "北" };
		std::vector<Hupai> fanpai_all;
		if (hudi.kezi_z[hudi.zhuangfeng + 1])
			fanpai_all.emplace_back("場風 " + feng_hanzi[hudi.zhuangfeng], 1);
		if (hudi.kezi_z[hudi.menfeng + 1])
			fanpai_all.emplace_back("自風 " + feng_hanzi[hudi.menfeng], 1);
		if (hudi.kezi_z[5]) fanpai_all.emplace_back("翻牌 白", 1);
		if (hudi.kezi_z[6]) fanpai_all.emplace_back("翻牌 發", 1);
		if (hudi.kezi_z[7]) fanpai_all.emplace_back("翻牌 中", 1);
		return fanpai_all;
		};
	const auto pinghu = [&hudi]() {
		if (hudi.pinghu) return std::vector<Hupai>{ Hupai{ "平和", 1 } };
		return std::vector<Hupai>{};
		};
	const auto duanyaojiu = [&hudi, &rule]() {
		if (hudi.n_yaojiu > 0) return std::vector<Hupai>{};
		// クイタンあり
		if (rule.kuitanAllowed || hudi.menqian)
			return std::vector<Hupai>{ Hupai{ "断幺九", 1 } };
		return std::vector<Hupai>{};
		};
	const auto yibeikou = [&hudi]() {
		if (!hudi.menqian) return std::vector<Hupai>{};
		int beikou = 0;
		for (const int x : hudi.shunzi_m) beikou += x >> 1;
		for (const int x : hudi.shunzi_p) beikou += x >> 1;
		for (const int x : hudi.shunzi_s) beikou += x >> 1;
		if (beikou == 1) return std::vector<Hupai>{ Hupai{ "一盃口", 1 } };
		return std::vector<Hupai>{};
		};
	const auto sansetongshun = [&hudi]() {
		for (int n = 1; n <= 7; n++) {
			if (hudi.shunzi_m[n] && hudi.shunzi_p[n] && hudi.shunzi_s[n])
				return std::vector<Hupai>{ Hupai{ "三色同順", (hudi.menqian ? 2 : 1) } };
		}
		return std::vector<Hupai>{};
		};
	const auto yiqitongguan = [&hudi]() {
		for (const auto s : { 'm', 'p', 's' }) {
			if (hudi.shunzi(s)[1] && hudi.shunzi(s)[4] && hudi.shunzi(s)[7])
				return std::vector<Hupai>{ Hupai{ "一気通貫", (hudi.menqian ? 2 : 1) } };
		}
		return std::vector<Hupai>{};
		};
	const auto hunquandaiyaojiu = [&hudi]() {
		if (hudi.n_yaojiu == 5 && hudi.n_shunzi > 0 && hudi.n_zipai > 0)
			return std::vector<Hupai>{ Hupai{ "混全帯幺九", (hudi.menqian ? 2 : 1) } };
		return std::vector<Hupai>{};
		};
	const auto qiduizi = [&hudi, &mianzi]() {
		if (mianzi.size() == 7) return std::vector<Hupai>{ Hupai{ "七対子", 2 } };
		return std::vector<Hupai>{};
		};
	const auto duiduihu = [&hudi]() {
		if (hudi.n_kezi == 4) return std::vector<Hupai>{ Hupai{ "対々和", 2 } };
		return std::vector<Hupai>{};
		};
	const auto sananke = [&hudi]() {
		if (hudi.n_ankezi == 3) return std::vector<Hupai>{ Hupai{ "三暗刻", 2 } };
		return std::vector<Hupai>{};
		};
	const auto sangangzi = [&hudi]() {
		if (hudi.n_gangzi == 3) return std::vector<Hupai>{ Hupai{ "三槓子", 2 } };
		return std::vector<Hupai>{};
		};
	const auto sansetongke = [&hudi]() {
		for (int n = 1; n <= 9; n++) {
			if (hudi.kezi_m[n] && hudi.kezi_p[n] && hudi.kezi_s[n])
				return std::vector<Hupai>{ Hupai{ "三色同刻", 2 } };
		}
		return std::vector<Hupai>{};
		};
	const auto hunlaotou = [&hudi, &mianzi]() {
		if (hudi.n_yaojiu == mianzi.size()
			&& hudi.n_shunzi == 0 && hudi.n_zipai > 0)
			return std::vector<Hupai>{ Hupai{ "混老頭", 2 } };
		return std::vector<Hupai>{};
		};
	const auto xiaosanyuan = [&hudi, &mianzi]() {
		if (hudi.kezi_z[5] + hudi.kezi_z[6] + hudi.kezi_z[7] == 2
			&& std::regex_search(mianzi[0], re_xiaosanyuan))
			return std::vector<Hupai>{ Hupai{ "小三元", 2 } };
		return std::vector<Hupai>{};
		};
	const auto hunyise = [&hudi, &mianzi]() {
		for (const auto s : { 'm', 'p', 's' }) {
			const std::regex re_yise{ "^[z" + std::string(1, s) + "]" };
			if (std::count_if(mianzi.begin(), mianzi.end(), [&re_yise](const auto& m) { return std::regex_search(m, re_yise); }) == mianzi.size()
				&& hudi.n_zipai > 0)
				return std::vector<Hupai>{ Hupai{ "混一色", (hudi.menqian ? 3 : 2) } };
		}
		return std::vector<Hupai>{};
		};
	const auto chunquandaiyaojiu = [&hudi]() {
		if (hudi.n_yaojiu == 5 && hudi.n_shunzi > 0 && hudi.n_zipai == 0)
			return std::vector<Hupai>{ Hupai{ "純全帯幺九", (hudi.menqian ? 3 : 2) } };
		return std::vector<Hupai>{};
		};
	const auto erbeikou = [&hudi]() {
		if (!hudi.menqian) return std::vector<Hupai>{};
		int beikou = 0;
		for (const int x : hudi.shunzi_m) beikou += x >> 1;
		for (const int x : hudi.shunzi_p) beikou += x >> 1;
		for (const int x : hudi.shunzi_s) beikou += x >> 1;
		if (beikou == 2) return std::vector<Hupai>{ Hupai{ "二盃口", 3 } };
		return std::vector<Hupai>{};
		};
	const auto qingyise = [&hudi, &mianzi]() {
		for (const auto s : { 'm', 'p', 's' }) {
			const std::regex re_yise{ "^[" + std::string(1, s) + "]" };
			if (std::count_if(mianzi.begin(), mianzi.end(), [&re_yise](const auto& m) { return std::regex_search(m, re_yise); }) == mianzi.size())
				return std::vector<Hupai>{ Hupai{ "清一色", (hudi.menqian ? 6 : 5) } };
		}
		return std::vector<Hupai>{};
		};

	const auto guoshiwushuang = [&hudi, &mianzi]() {
		if (mianzi.size() != 13) return std::vector<Hupai>{};
		if (hudi.danqi)          return std::vector<Hupai>{ Hupai{ "国士無双十三面", -2 } };
		else                     return std::vector<Hupai>{ Hupai{ "国士無双", -1 } };
		};
	const auto sianke = [&hudi]() {
		if (hudi.n_ankezi != 4) return std::vector<Hupai>{};
		if (hudi.danqi)         return std::vector<Hupai>{ Hupai{ "四暗刻単騎", -2 } };
		else                    return std::vector<Hupai>{ Hupai{ "四暗刻", -1 } };
		};
	const auto dasanyuan = [&hudi, &mianzi]() {
		if (hudi.kezi_z[5] + hudi.kezi_z[6] + hudi.kezi_z[7] == 3) {
			std::vector<std::string> bao_mianzi;
			std::copy_if(mianzi.begin(), mianzi.end(), std::back_inserter(bao_mianzi), [](const auto& m) {
				return std::regex_search(m, re_dasanyuan); });
			std::smatch baojia;
			if (bao_mianzi.size() > 2 && std::regex_search(bao_mianzi[2], baojia, re_menqian))
				return std::vector<Hupai>{ Hupai{ "大三元", -1, baojia.str()[0]} };
			else return std::vector<Hupai>{ Hupai{ "大三元", -1 } };
		}
		return std::vector<Hupai>{};
		};
	const auto sixihu = [&hudi, &mianzi]() {
		if (hudi.kezi_z[1] + hudi.kezi_z[2] + hudi.kezi_z[3] + hudi.kezi_z[4] == 4) {
			std::vector<std::string> bao_mianzi;
			std::copy_if(mianzi.begin(), mianzi.end(), std::back_inserter(bao_mianzi), [](const auto& m) {
				return std::regex_search(m, re_sixihu1); });
			std::smatch baojia;
			if (bao_mianzi.size() > 3 && std::regex_search(bao_mianzi[3], baojia, re_menqian))
				return std::vector<Hupai>{ Hupai{ "大四喜", -2, baojia.str()[0]} };
			else return std::vector<Hupai>{ Hupai{ "大四喜", -2 } };
		}
		if (hudi.kezi_z[1] + hudi.kezi_z[2] + hudi.kezi_z[3] + hudi.kezi_z[4] == 3
			&& std::regex_search(mianzi[0], re_sixihu2))
			return std::vector<Hupai>{ Hupai{ "小四喜", -1 } };
		return std::vector<Hupai>{};
		};
	const auto ziyise = [&hudi, &mianzi]() {
		if (hudi.n_zipai == mianzi.size())
			return std::vector<Hupai>{ Hupai{ "字一色", -1 } };
		return std::vector<Hupai>{};
		};
	const auto lvyise = [&mianzi]() {
		if (std::count_if(mianzi.begin(), mianzi.end(), [](const auto& m) { return std::regex_search(m, re_lvyise1); }) > 0) return std::vector<Hupai>{};
		if (std::count_if(mianzi.begin(), mianzi.end(), [](const auto& m) { return std::regex_search(m, re_lvyise2); }) > 0) return std::vector<Hupai>{};
		if (std::count_if(mianzi.begin(), mianzi.end(), [](const auto& m) { return std::regex_search(m, re_lvyise3); }) > 0) return std::vector<Hupai>{};
		return std::vector<Hupai>{ Hupai{ "緑一色", -1 } };
		};
	const auto qinglaotou = [&hudi]() {
		if (hudi.n_yaojiu == 5 && hudi.n_kezi == 4 && hudi.n_zipai == 0)
			return std::vector<Hupai>{ Hupai{ "清老頭", -1 } };
		return std::vector<Hupai>{};
		};
	const auto sigangzi = [&hudi]() {
		if (hudi.n_gangzi == 4) return std::vector<Hupai>{ Hupai{ "四槓子", -1 } };
		return std::vector<Hupai>{};
		};
	const auto jiulianbaodeng = [&mianzi]() {
		if (mianzi.size() != 1) return std::vector<Hupai>{};
		if (std::regex_search(mianzi[0], re_jiulianbaodeng))
			return std::vector<Hupai>{ Hupai{ "純正九蓮宝燈", -2 } };
		else return std::vector<Hupai>{ Hupai{ "九蓮宝燈", -1 } };
		};

	auto damanguan = (pre_hupai.size() > 0 && pre_hupai[0].fanshu < 0)
		? pre_hupai : std::vector<Hupai>{};
	damanguan = concat(concat(concat(concat(concat(concat(concat(concat(concat(damanguan,
		guoshiwushuang()),
		sianke()),
		dasanyuan()),
		sixihu()),
		ziyise()),
		lvyise()),
		qinglaotou()),
		sigangzi()),
		jiulianbaodeng());

	for (auto& hupai : damanguan) {
		// ダブル役満あり
		if (!rule.doubleYakumanAllowed) hupai.fanshu = -1;
		// 役満パオあり
		if (!rule.yakumanPaoAllowed) hupai.baojia = 0;
	}
	if (damanguan.size() > 0) return damanguan;

	auto hupai = pre_hupai;
	hupai = concat(concat(concat(concat(concat(concat(concat(concat(concat(concat(concat(concat(concat(concat(concat(concat(concat(concat(concat(hupai,
		menqianqing()),
		fanpai()),
		pinghu()),
		duanyaojiu()),
		yibeikou()),
		sansetongshun()),
		yiqitongguan()),
		hunquandaiyaojiu()),
		qiduizi()),
		duiduihu()),
		sananke()),
		sangangzi()),
		sansetongke()),
		hunlaotou()),
		xiaosanyuan()),
		hunyise()),
		chunquandaiyaojiu()),
		erbeikou()),
		qingyise());

	if (hupai.size() > 0) hupai.insert(hupai.end(), post_hupai.begin(), post_hupai.end());

	return hupai;
}

std::vector<Hupai> get_post_hupai(const Shoupai& shoupai, const std::string& rongpai, const std::vector<std::string>& baopai, const std::vector<std::string>& fubaopai) {

	auto new_shoupai = shoupai;
	if (!rongpai.empty()) new_shoupai.zimo(rongpai);
	const auto paistr = new_shoupai.toString();

	std::vector<Hupai> post_hupai;

	std::vector<std::string> suitstr;
	for (std::sregex_iterator it(paistr.begin(), paistr.end(), re_get_post_hupai), end; it != end; ++it) {
		suitstr.emplace_back(it->str());
	}

	int n_baopai = 0;
	for (const auto& p : baopai) {
		const auto p_ = Shan::zhenbaopai(p);
		const std::regex regexp{ p_.substr(1, 1) };
		for (const auto& m : suitstr) {
			if (m[0] != p_[0]) continue;
			const auto m_ = std::regex_replace(m, re_ling, "5");
			int nn = 0;
			for (std::sregex_iterator it(m_.begin(), m_.end(), regexp), end; it != end; ++it)
				nn++;
			if (nn) n_baopai += nn;
		}
	}
	if (n_baopai) post_hupai.emplace_back("ドラ", n_baopai);

	int n_hongpai = 0;
	int nn = 0;
	for (std::sregex_iterator it(paistr.begin(), paistr.end(), re_ling), end; it != end; ++it)
		nn++;
	if (nn) n_hongpai = nn;
	if (n_hongpai) post_hupai.emplace_back("赤ドラ", n_hongpai);

	int n_fubaopai = 0;
	for (const auto& p : fubaopai) {
		const auto p_ = Shan::zhenbaopai(p);
		const std::regex regexp{ p_.substr(1, 1) };
		for (const auto& m : suitstr) {
			if (m[0] != p_[0]) continue;
			const auto m_ = std::regex_replace(m, re_ling, "5");
			int nn = 0;
			for (std::sregex_iterator it(m_.begin(), m_.end(), regexp), end; it != end; ++it)
				nn++;
			if (nn) n_fubaopai += nn;
		}
	}
	if (n_fubaopai) post_hupai.emplace_back("裏ドラ", n_fubaopai);

	return post_hupai;
}

// 得分(得点)
Defen get_defen(int fu, std::vector<Hupai>& hupai, const std::string& rongpai, const Param& param) {
	if (hupai.size() == 0) return {};

	const auto menfeng = param.menfeng;
	int fanshu{}, damanguan{}, defen{}, base{}, baojia{}, defen2{}, base2{}, baojia2{};

	if (hupai[0].fanshu < 0) {
		fu = 0;
		// 役満の複合あり
		damanguan = !param.rule.yakumanCombinationAllowed ? 1
			: std::accumulate(hupai.begin(), hupai.end(), 0, [](const int acc, const auto& h) { return acc - h.fanshu; });
		base = 8000 * damanguan;

		auto h = std::find_if(hupai.begin(), hupai.end(), [](const auto& h) { return h.baojia; });
		if (h != hupai.end()) {
			baojia2 = (menfeng + (h->baojia == '+' ? 1 : h->baojia == '=' ? 2 : 3)) % 4;
			base2 = 8000 * std::min(-h->fanshu, damanguan);
		}
	}
	else {
		fanshu = std::accumulate(hupai.begin(), hupai.end(), 0, [](const int acc, const auto& h) { return acc + h.fanshu; });
		base = (fanshu >= 13 && param.rule.countedYakumanAllowed) // 数え役満あり
			? 8000
			: (fanshu >= 11) ? 6000
			: (fanshu >= 8) ? 4000
			: (fanshu >= 6) ? 3000
			: param.rule.roundUpManganAllowed && fu << (2 + fanshu) == 1920
			? 2000
			: std::min(fu << (2 + fanshu), 2000);
	}

	std::array<int, 4> fenpei = {0, 0, 0, 0};
	const auto chang = param.jicun.changbang;
	const auto lizhi = param.jicun.lizhibang;

	if (baojia2) {
		if (!rongpai.empty()) base2 = base2 / 2;
		base = base - base2;
		defen2 = base2 * (menfeng == 0 ? 6 : 4);
		fenpei[menfeng] += defen2;
		fenpei[baojia2] -= defen2;
	}
	else defen2 = 0;

	if (!rongpai.empty() || base == 0) {
		baojia = (base == 0)
			? baojia2
			: (menfeng + (rongpai[2] == '+' ? 1 : rongpai[2] == '=' ? 2 : 3)) % 4;
		defen = (int)std::ceil(base * (menfeng == 0 ? 6 : 4) / 100.0) * 100;
		fenpei[menfeng] += defen + chang * 300 + lizhi * 1000;
		fenpei[baojia] -= defen + chang * 300;
	}
	else {
		const auto zhuangjia = (int)std::ceil(base * 2 / 100.0) * 100;
		const auto sanjia = (int)std::ceil(base / 100.0) * 100;
		if (menfeng == 0) {
			defen = zhuangjia * 3;
			for (int l = 0; l < 4; l++) {
				if (l == menfeng)
					fenpei[l] += defen + chang * 300 + lizhi * 1000;
				else    fenpei[l] -= zhuangjia + chang * 100;
			}
		}
		else {
			defen = zhuangjia + sanjia * 2;
			for (int l = 0; l < 4; l++) {
				if (l == menfeng)
					fenpei[l] += defen + chang * 300 + lizhi * 1000;
				else if (l == 0)
					fenpei[l] -= zhuangjia + chang * 100;
				else    fenpei[l] -= sanjia + chang * 100;
			}
		}
	}

	return {
		std::move(hupai),
		fu,
		fanshu,
		damanguan,
		defen + defen2,
		fenpei
	};
}

Defen hule(const Shoupai& shoupai, std::string rongpai, const Param& param) {
	if (!rongpai.empty()) {
		if (!std::regex_search(rongpai, re_menqian_end)) throw std::invalid_argument(rongpai);
		rongpai = rongpai.substr(0, 2) + rongpai.back();
	}

	Defen max{};

	const auto& pre_hupai = get_pre_hupai(param.hupai);
	const auto& post_hupai = get_post_hupai(shoupai, rongpai,
		param.baopai, param.fubaopai);

	for (const auto& mianzi : hule_mianzi(shoupai, rongpai)) {
		const auto& hudi = get_hudi(mianzi, param.zhuangfeng, param.menfeng,
			param.rule);
		auto hupai = get_hupai(mianzi, hudi, pre_hupai, post_hupai, param.rule);
		const auto& rv = get_defen(hudi.fu, hupai, rongpai, param);

		if (!max.defen || rv.defen > max.defen
			|| rv.defen == max.defen
			&& (!rv.fanshu || rv.fanshu > max.fanshu
				|| rv.fanshu == max.fanshu && rv.fu > max.fu)) max = rv;
	}

	return max;
}
