#include "plugin/Config.h"
#include "plugin/LL3Mine2.h"

#include <ll/api/Logger.h>
#include <ll/api/memory/Hook.h>


#include <mc/world/level/BlockPos.h>
#include <mc/world/level/BlockSource.h>
#include <mc/world/level/block/Block.h>
#include <mc/world/level/block/LiquidBlock.h>


// #include <memory>
#include <string>


namespace LL3Mine2_Class::Hook {

LL_TYPE_INSTANCE_HOOK(
    LiquidBlockSolidifyHook,
    HookPriority::Normal,
    LiquidBlock,
    "?solidify@LiquidBlock@@IEBAXAEAVBlockSource@@AEBVBlockPos@@1@Z",
    void,
    class BlockSource&    bs,
    class BlockPos const& bp1,
    class BlockPos const& bp2
) {
    origin(bs, bp1, bp2);
    if (!CONFIG->InitSuccess) {
        return;
    }
    let& bl = bs.getBlock(bp1);

    let blType = bl.getTypeName();
    if (blType != "minecraft:cobblestone" && blType != "minecraft:stone") {
        return;
    }

    std::string randomResBlockName = CONFIG->randomBlockType(blType); //"minecraft:bedrock";


    let res = Block::tryGetFromRegistry(randomResBlockName);
    if (res.has_value() == false) {
        LOGGER.error("Unknown block: \"{}\"!", randomResBlockName);
        return;
    }
    bs.setBlock(bp1, res.value(), 3, nullptr, nullptr);
};

using HookReg = ll::memory::HookRegistrar<LiquidBlockSolidifyHook>;

static HookReg hookRegister;
} // namespace LL3Mine2_Class::Hook