#pragma once

#include "shoupai.h"
#include "rule.h"
#include "shan.h"

#include <array>

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
	std::vector<std::string> fubaopai;
	struct Jicun {
		int changbang = 0;
		int lizhibang = 0;
	} jicun;
};

struct Hupai {
	std::string name;
	int fanshu;
	char baojia; // 包家(放銃者)

	Hupai(const std::string& name, const int fanshu, const char baojia) : name{ name }, fanshu{ fanshu }, baojia{ baojia } {}
	Hupai(const std::string& name, const int fanshu) : name{ name }, fanshu{ fanshu }, baojia{} {}
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
Defen hule(const Shoupai& shoupai, std::string rongpai, const Param& param);
