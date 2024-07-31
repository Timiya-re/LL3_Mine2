#include "plugin/Config.h"
#include "plugin/LL3Mine2.h"

#include <ll/api/command/CommandHandle.h>
#include <ll/api/command/CommandRegistrar.h>
#include <ll/api/event/Event.h>
#include <ll/api/event/EventBus.h>
#include <ll/api/event/server/ServerStartedEvent.h>
#include <ll/api/service/Bedrock.h>

#include <mc/server/commands/CommandOrigin.h>
#include <mc/server/commands/CommandRegistry.h>
#include <mc/world/level/Command.h>

#define ConfigNameSpace LL3Mine2_Class::Config

using ServerStartedEvent = ll::event::ServerStartedEvent;

void RegReloadCmd(ServerStartedEvent&) {
    LOGGER.info("REG!");
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
    cmd.overload().text("reload").execute([](CommandOrigin const&, CommandOutput&) -> void {
        if (!ConfigNameSpace::ResetConfig()) {
            LOGGER.error("Reset Config Failed! Unable to Reload Config!");
            return;
        }
        if (!ConfigNameSpace::InitConfig()) {
            LOGGER.error("Init Config Failed!");
        } else {
            LOGGER.info("Reload Config Success!");
        }
    });
}

extern void RegReloadCmdListen() {
    let& bus = ll::event::EventBus::getInstance();
    bus.emplaceListener<ServerStartedEvent>(RegReloadCmd);
}
