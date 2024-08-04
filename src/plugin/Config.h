#pragma once

#include "../utils/json.hpp"
#include <map>
#include <string>


namespace LL3Mine2_Class::Config {
bool InitConfig();
bool ResetConfig();
class Config;
Config* getInstance();

class Config {
public:
    bool InitSuccess;
    Config();
    ~Config();
    bool         Init();
    std::string& randomBlockType(std::string&);

protected:
    std::map<std::string, size_t>* probabilityTable;
    size_t*                        allProbabilityTableSum;
    void                           CreateProbabilityTable(nlohmann::basic_json<>);
};


} // namespace LL3Mine2_Class::Config