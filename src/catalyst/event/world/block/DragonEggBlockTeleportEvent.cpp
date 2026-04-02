#include "DragonEggBlockTeleportEvent.h"

#include "catalyst/mod/Gloabl.h"
#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/deps/shared_types/legacy/LevelEvent.h"
#include "mc/util/Random.h"
#include "mc/world/events/gameevents/GameEventRegistry.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/block/Block.h"
#include "mc/world/level/block/BlockChangeContext.h"
#include "mc/world/level/block/DragonEggBlock.h"
#include "mc/world/level/block/VanillaBlockTypeIds.h"
#include "mc/world/level/block/registry/BlockTypeRegistry.h"

namespace Catalyst {

LL_STATIC_HOOK(
    DragonEggBlockTeleportEventHook,
    ll::memory::HookPriority::Normal,
    &DragonEggBlock::_attemptTeleport,
    void,
    ::BlockSource&    region,
    ::Random&         random,
    ::BlockPos const& pos
) {
    auto& level = region.getLevel();
    if (level.isClientSide()) {
        return origin(region, random, pos);
    }

    auto& coreRandom = random.mRandom->mObject;

    BlockPos newPos;
    bool     foundValidPos = false;

    for (int i = 0; i < 1000; ++i) {
        int offsetX = (coreRandom._genRandInt32() & 0xF) - (coreRandom._genRandInt32() & 0xF); // [-15, 15]
        int maxH    = region.getMaxHeight();
        int offsetY = (coreRandom._genRandInt32() & 7) - (coreRandom._genRandInt32() & 7); // [-7, 7]
        offsetY     = std::clamp(offsetY, 0, maxH);
        int offsetZ = (coreRandom._genRandInt32() & 0xF) - (coreRandom._genRandInt32() & 0xF); // [-15, 15]

        BlockPos currentAttemptPos  = pos;
        currentAttemptPos.x        += offsetX;
        currentAttemptPos.y        += offsetY;
        currentAttemptPos.z        += offsetZ;

        if (region.getBlock(currentAttemptPos).isAir()) {
            newPos        = currentAttemptPos;
            foundValidPos = true;
            break;
        }
    }

    if (!foundValidPos) {
        return; // No valid position found, do nothing.
    }

    auto& bus = ll::event::EventBus::getInstance();

    DragonEggBlockTeleportBeforeEvent beforeEvent(region, pos, newPos);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    // Use the (potentially modified) newPos from the event
    BlockPos finalNewPos = beforeEvent.getNewPos();

    logger.debug(
        "DragonEggBlockTeleportEvent: Teleporting egg from ({}, {}, {}) to ({}, {}, {})",
        pos.x,
        pos.y,
        pos.z,
        finalNewPos.x,
        finalNewPos.y,
        finalNewPos.z
    );

    region.postGameEvent(nullptr, GameEventRegistry::teleport(), pos, nullptr);

    int diffX = pos.x - finalNewPos.x;
    int diffY = pos.y - finalNewPos.y;
    int diffZ = pos.z - finalNewPos.z;
    int particleData =
        abs(diffZ)
        | ((abs(diffY) | (((((diffX >> 31) | (2 * ((diffY >> 31) | (2 * (diffZ >> 31))))) << 8) | abs(diffX)) << 8))
           << 8);

    level.broadcastLocalEvent(region, SharedTypes::Legacy::LevelEvent::ParticlesDragonEgg, pos, particleData);
    region.setBlock(
        finalNewPos,
        BlockTypeRegistry::get().getDefaultBlockState(VanillaBlockTypeIds::DragonEgg(), true),
        3 /* BlockUpdateFlag::All */,
        nullptr,
        BlockChangeContext{}
    );
    region.removeBlock(pos, BlockChangeContext{});

    DragonEggBlockTeleportAfterEvent afterEvent(region, pos, finalNewPos);
    bus.publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class DragonEggBlockTeleportBeforeEventEmitter
: public ll::event::Emitter<beforeEmitterFactory, DragonEggBlockTeleportBeforeEvent> {
    ll::memory::HookRegistrar<DragonEggBlockTeleportEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<DragonEggBlockTeleportBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class DragonEggBlockTeleportAfterEventEmitter
: public ll::event::Emitter<afterEmitterFactory, DragonEggBlockTeleportAfterEvent> {
    ll::memory::HookRegistrar<DragonEggBlockTeleportEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<DragonEggBlockTeleportAfterEventEmitter>();
}

} // namespace Catalyst