#pragma once

#include "shoupai.h"

#include <tuple>
#include <array>

std::tuple<std::array<int, 3>, std::array<int, 3>> mianzi(std::vector<int> bingpai, const int n = 1);
int mianzi_all(const Shoupai& shoupai, const bool jiangpai);
int xiangting_yiban(const Shoupai& shoupai);
int xiangting_guoshi(const Shoupai& shoupai);
int xiangting_qidui(const Shoupai& shoupai);
int xiangting(const Shoupai& shoupai);
std::vector<std::string> tingpai(const Shoupai& shoupai, const std::function<int(const Shoupai&)>& f_xiangting = xiangting);
