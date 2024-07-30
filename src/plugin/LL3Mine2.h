#pragma once

#include "ll/api/plugin/NativePlugin.h"

#define SELF   LL3Mine2_Class::LL3Mine2::getInstance().getSelf()
#define LOGGER LL3Mine2_Class::LL3Mine2::getInstance().getSelf().getLogger()
#define CONFIG LL3Mine2_Class::Config::getInstance()
#define let    auto

namespace LL3Mine2_Class {

class LL3Mine2 {

public:
    static LL3Mine2& getInstance();

    LL3Mine2(ll::plugin::NativePlugin& self) : mSelf(self) {}

    [[nodiscard]] ll::plugin::NativePlugin& getSelf() const { return mSelf; }

    /// @return True if the plugin is loaded successfully.
    bool load();

    /// @return True if the plugin is enabled successfully.
    bool enable();

    /// @return True if the plugin is disabled successfully.
    bool disable();

    // TODO: Implement this method if you need to unload the plugin.
    // /// @return True if the plugin is unloaded successfully.
    // bool unload();

private:
    ll::plugin::NativePlugin& mSelf;
};

} // namespace LL3Mine2_Class
