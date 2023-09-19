#pragma once

#include "shoupai.h"

class Shan {
public:
    static std::string zhenbaopai(const std::string& p) {
        if (!Shoupai::valid_pai(p)) throw std::invalid_argument(p);
        const auto s = p[0];
        int n = to_int(p[1]);
        if (n == 0) n = 5;
        return s == 'z' ? (n < 5 ? to_string(s, (n % 4 + 1)) : to_string(s, ((n - 4) % 3 + 5)))
            : to_string(s, (n % 9 + 1));
    }
};