#include "plugin/Config.h"
#include "plugin/LL3Mine2.h"

#include "./../utils/stringTool.hpp"

#include <mc/server/ServerPlayer.h>
#include <mc/server/commands/CommandOutput.h>
#include <mc/world/actor/Actor.h>
#include <mc/world/level/Command.h>

#include <ll/api/command/CommandHandle.h>
#include <ll/api/command/CommandRegistrar.h>
#include <ll/api/event/Event.h>
#include <ll/api/event/EventBus.h>
#include <ll/api/event/server/ServerStartedEvent.h>
#include <ll/api/service/Bedrock.h>

#include <string>
#include <vector>

#define ConfigNameSpace LL3Mine2_Class::Config

using ServerStartedEvent = ll::event::ServerStartedEvent;

void ProcessCommandMessage(std::string logger_raw, CommandOutput& out) {
    if (logger_raw.find(" ") == std::string::npos) {
        return;
    }
    std::vector<std::string> cut_str{};
    str_split((std::string)logger_raw, " ", cut_str);
    if (cut_str.size() < 2) {
        return;
    }
    std::vector<std::string> content_tmp(cut_str.begin() + 2, cut_str.end());
    std::string              content = "";
    str_join(content_tmp, " ", content);
    if (cut_str[0].find("INFO") != std::string::npos) {
        out.success("[§l§bINFO§r] " + content);
    } else if (cut_str[0].find("WARN") != std::string::npos) {
        out.success("[§l§eWARN§r] " + content);
    } else if (cut_str[0].find("ERROR") != std::string::npos) {
        out.error("[§l§eERROR§r] " + content);
    }
}

void RegReloadCmd(ServerStartedEvent&) {
    let& mc_reg  = ll::service::getCommandRegistry().get();
    let& ll_reg  = ll::command::CommandRegistrar::getInstance();
    let  findVal = mc_reg.findCommand("mine2");
    if (findVal != nullptr) {
        LOGGER.error("Command Create Fail! This Command Already Exists!");
        return;
    }
    auto& cmd = ll_reg.getOrCreateCommand(
        "mine2",
        "Automatic mining machine brushing",
        (CommandPermissionLevel)1,
        (CommandFlagValue)0
    );
    cmd.alias("mine");
    cmd.overload().text("reload").execute([](CommandOrigin const& ori, CommandOutput& out) -> void {
        if (!ConfigNameSpace::ResetConfig()) {
            LOGGER.error("Reset Config Failed! Unable to Reload Config!");
            return;
        }
        // let ac = ori.getEntity();
        // let sp = (ac == nullptr ? nullptr : ac->isPlayer() ? (ServerPlayer*)ac : nullptr);
        LOGGER.info("\"{}\" Is Requesting To Reload The Config!", ori.getName());
        int consoleLevel    = LOGGER.consoleLevel;
        LOGGER.consoleLevel = 0;
        LOGGER.setPlayerOutputFunc([&out](std::string_view str) -> void {
            ProcessCommandMessage(std::string(str), out);
        });
        if (!ConfigNameSpace::InitConfig()) {
            out.error("Init Config Failed!", {});
        } else {
            out.success("Reload Config Success!", {});
        }
        LOGGER.consoleLevel = consoleLevel;
        LOGGER.setPlayerOutputFunc([](std::string_view) -> void {});
    });
}

extern void RegReloadCmdListen() {
    let& bus = ll::event::EventBus::getInstance();
    bus.emplaceListener<ServerStartedEvent>(RegReloadCmd);
}
