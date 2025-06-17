#pragma once

#include <string>
#include <vector>

struct Config {
    std::string pattern;
    std::string replacement;
    std::vector<std::string> files;
    bool ignore_case = false;
    bool line_number = false;
    bool invert_match = false;
    bool replace_mode = false;
};
