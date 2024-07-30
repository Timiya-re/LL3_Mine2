#include "plugin/LL3Mine2.h"
#include <plugin/Config.h>

#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "ll/api/plugin/NativePlugin.h"
#include "ll/api/plugin/RegisterHelper.h"
#include <ll/api/event/server/ServerStoppingEvent.h>

namespace LL3Mine2_Class {

static std::unique_ptr<LL3Mine2> instance;
static bool                      inStoping = false;

void RegReloadCmdListen();

LL3Mine2& LL3Mine2::getInstance() { return *instance; }

bool LL3Mine2::load() {
    RegReloadCmdListen();
    let& bus = ll::event::EventBus::getInstance();

    using ServerStoppingEvent = ll::event::ServerStoppingEvent;
    bus.emplaceListener<ServerStoppingEvent>([](ServerStoppingEvent&) { inStoping = true; });
    getSelf().getLogger().info("LL3 Mine2 Loaded!");
    return true;
}

bool LL3Mine2::enable() {
    if (LL3Mine2_Class::Config::InitConfig()) {
        LOGGER.info("Config Loaded Successfully!");
    } else {
        LOGGER.error("Plugin Load Fail!");
    }
    return true;
}

bool LL3Mine2::disable() {
    if (inStoping) {
        LOGGER.info("Disabling...");
        return true;
    }
    getSelf().getLogger().warn("Plugin Can not Disable!");
    return false;
}

} // namespace LL3Mine2_Class

LL_REGISTER_PLUGIN(LL3Mine2_Class::LL3Mine2, LL3Mine2_Class::instance);