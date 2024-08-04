#include "./../utils/stringTool.hpp"

std::vector<std::string>& str_split(const std::string& str, const std::string& delim, std::vector<std::string>& res) {
    if (str == "") return res;
    std::string strs = str + delim;
    size_t      pos  = strs.find(delim);
    while (pos != strs.npos) {
        std::string temp = strs.substr(0, pos);
        res.push_back(temp);
        strs = strs.substr(pos + 1, strs.size());
        pos  = strs.find(delim);
    }
    return res;
}

std::string& str_join(const std::vector<std::string>& val, const std::string& cent, std::string& input) {
    for (auto v : val) {
        if (input == "") {
            input += v;
            continue;
        }
        input += cent + v;
    }
    return input;
}