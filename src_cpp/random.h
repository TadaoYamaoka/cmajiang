#pragma once

#include "game.h"
#include <map>

bool setup_zhuangfeng(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const int zhuangfeng, const Rule& rule, std::mt19937_64& mt);
bool setup_menfeng(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const int l, const Rule& rule, std::mt19937_64& mt);
bool setup_fanpai(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_pinghe(std::vector<std::string>& pai, std::map<std::string, int>& rest, const int zhuangfeng, const int l, const Rule& rule, std::mt19937_64& mt);
bool setup_duanyaojiu(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt);
bool setup_yibeikou(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt);
bool setup_sansetongshun(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_yiqitongguan(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_hunquandaiyaojiu(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_qiduizi(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt);
bool setup_duiduihu(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_sananke(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_sangangzi(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_sansetongke(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_hunlaotou(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_xiaosanyuan(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_hunyise(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_chunquandaiyaojiu(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_erbeikou(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_qingyise(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_guoshiwushuang(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt);
bool setup_sianke(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt);
bool setup_dasanyuan(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_xiaosixi(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_dasixi(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_ziyise(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_lvyise(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_qinglaotou(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_sigangzi(std::vector<std::string>& pai, std::map<std::string, int>& rest, std::vector<std::string>& fulou, const Rule& rule, std::mt19937_64& mt);
bool setup_jiulianbaodeng(std::vector<std::string>& pai, std::map<std::string, int>& rest, const Rule& rule, std::mt19937_64& mt);
