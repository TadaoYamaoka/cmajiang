#pragma once

#include "shoupai.h"

#include <unordered_set>

class He {
public:
    // 打牌
    He& dapai(const std::string& p);
    // 副露
    He& fulou(const std::string& m);

    bool find(const std::string& p);

    const std::vector<std::string>& pai() const { return _pai; }

private:
    std::vector<std::string> _pai;
    std::unordered_set<std::string> _find;
};
