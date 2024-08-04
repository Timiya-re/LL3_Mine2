#include "plugin/Config.h"
#include "LL3Mine2.h"
#include "ll/api/Expected.h"
#include "plugin/Exception.h"
#include "plugin/LL3Mine2.h"

#include <ll/api/Logger.h>

#include <mc/server/commands/CommandOutput.h>

#include <corecrt_io.h>
#include <fstream>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <string>

#define CONFIG_PATH "./plugins/LL3Mine2/config.json"

namespace LL3Mine2_Class::Config {

static Config* instance = nullptr;

bool InitConfig() {
    try {
        instance = new Config();
        bool res = instance->Init();
        return res;
    } catch (...) {
        _CATCH_CODES("LL3Mine2_Class::Config::InitConfig")
        return false;
    }
}

bool ResetConfig() {
    try {
        delete instance;
    } catch (...) {
        return false;
    }
    return true;
}

Config* getInstance() { return instance; }

void generateRandomNumber(size_t minValue, size_t maxValue, size_t& result) {
    std::random_device rd;        // 用于获取随机数种子
    std::mt19937_64    gen(rd()); // Mersenne Twister 引擎

    std::uniform_int_distribution<size_t> dis(minValue, maxValue);
    result = dis(gen);
}

Config::Config() {
    InitSuccess                  = false;
    this->probabilityTable       = new std::map<std::string, size_t>();
    this->allProbabilityTableSum = new size_t(0);
}
Config::~Config() {
    delete this->probabilityTable;
    delete this->allProbabilityTableSum;
}

bool Config::Init() {
    if (_access(CONFIG_PATH, 0) == -1) {
        nlohmann::basic_json<> config = {
            {"minecraft:cobblestone",  200},
            {"minecraft:coal_ore",     20 },
            {"minecraft:copper_ore",   15 },
            {"minecraft:iron_ore",     10 },
            {"minecraft:gold_ore",     5  },
            {"minecraft:emerald_ore",  3  },
            {"minecraft:redstone_ore", 10 },
            {"minecraft:lapis_ore",    10 },
            {"minecraft:diamond_ore",  2  },
        };
        let file = std::unique_ptr<std::fstream>(new std::fstream(CONFIG_PATH, std::ios_base::out));
        if (!file->is_open()) {
            LOGGER.error("Config file: {} cannot be opened! This plugin cannot continue to work!", CONFIG_PATH);
            return false;
        }
        (*file) << config.dump(2);
        LOGGER.warn("Config file: {} was not found, it has been created automatically", CONFIG_PATH);
    }
    let file = std::unique_ptr<std::fstream>(new std::fstream(CONFIG_PATH, std::ios::in));

    if (!file->is_open()) {
        LOGGER.error("Config file: {} cannot be opened! This plugin cannot continue to work!", CONFIG_PATH);
        return false;
    }
    std::stringstream content;
    content << file->rdbuf();
    let config = nlohmann::json::parse(content);
    CreateProbabilityTable(config);
    InitSuccess = true;
    return true;
}
void Config::CreateProbabilityTable(nlohmann::basic_json<> json) {
    LOGGER.info("[CreateTable] Starting To Parse Config...");
    for (auto& [key, val] : json.items()) {
        if (!val.is_number_integer()) {
            LOGGER.warn("[CreateTable] Key \"{}\" Val Is Not A Integer! Auto Ignore!", key);
            continue;
        }
        size_t st = val.get<size_t>();

        this->probabilityTable->insert({key, st});
        *(this->allProbabilityTableSum) += st;
        LOGGER.info("[CreateTable] {}: {}", key, st);
    }
}
std::string& Config::randomBlockType(std::string& defBlockType) {
    size_t random_number = 0;
    generateRandomNumber(0, *allProbabilityTableSum, random_number);
    for (auto& [blType, weight] : *probabilityTable) {
        if (random_number < weight) {
            return (std::string&)blType;
        }
        random_number -= weight;
    }
    return defBlockType;
}
} // namespace LL3Mine2_Class::Config