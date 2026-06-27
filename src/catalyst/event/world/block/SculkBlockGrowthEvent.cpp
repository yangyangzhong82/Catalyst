#include "SculkBlockGrowthEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/block/SculkBlockBehavior.h"

#include "mc/deps/nbt/CompoundTag.h"

namespace Catalyst {

void SculkBlockGrowthEvent::serialize(CompoundTag& nbt) const {
    ll::event::world::WorldEvent::serialize(nbt);
    nbt["pos"] = ListTag{pos().x, pos().y, pos().z};
}


LL_STATIC_HOOK(
    SculkBlockGrowthHook,
    ll::memory::HookPriority::Normal,
    &SculkBlockBehavior::_placeGrowthAt,
    void,
    ::IBlockWorldGenAPI& target,
    ::BlockSource*       region,
    ::BlockPos const&    pos,
    ::Random&            random,
    ::SculkSpreader&     spreader
) {
    auto& bus = ll::event::EventBus::getInstance();
    WeakRef<BlockSource> regionWeak;

    if (region == nullptr) {
        // Some worldgen paths provide a null BlockSource pointer.
        // Preserve game behavior, but skip publishing events that require BlockSource&.
        origin(target, region, pos, random, spreader);
        return;
    }

    regionWeak = region->getWeakRef();

    SculkBlockGrowthBeforeEvent beforeEvent(*region, pos);
    bus.publish(beforeEvent);

    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(target, region, pos, random, spreader);

    auto lockedRegion = regionWeak.lock();
    if (!lockedRegion) {
        // _placeGrowthAt may invalidate the original BlockSource on some worldgen paths.
        // Do not publish an AfterEvent with a dangling WorldEvent::blockSource().
        return;
    }

    SculkBlockGrowthAfterEvent afterEvent(*lockedRegion, pos);
    bus.publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class SculkBlockGrowthBeforeEventEmitter
: public ll::event::Emitter<beforeEmitterFactory, SculkBlockGrowthBeforeEvent> {
    ll::memory::HookRegistrar<SculkBlockGrowthHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<SculkBlockGrowthBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class SculkBlockGrowthAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, SculkBlockGrowthAfterEvent> {
    ll::memory::HookRegistrar<SculkBlockGrowthHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<SculkBlockGrowthAfterEventEmitter>();
}

} // namespace Catalyst
