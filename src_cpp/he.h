#pragma once

#include "shoupai.h"

#include <unordered_set>

class He {
public:
    // �Ŕv
    He& dapai(const std::string& p);
    // ���I
    He& fulou(const std::string& m);
    // �t���e��
    bool find(const std::string& p);

    const std::vector<std::string>& pai() const { return _pai; }

private:
    std::vector<std::string> _pai;
    // �t���e��
    std::unordered_set<std::string> _find;
};
