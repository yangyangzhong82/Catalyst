#include "MossGrowthEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/WorldBlockTarget.h"
#include "mc/world/level/levelgen/feature/VegetationPatchFeature.h"

namespace Catalyst {

LL_AUTO_TYPE_INSTANCE_HOOK(
    MossGrowthEventHook,
    ll::memory::HookPriority::Normal,
    VegetationPatchFeature,
    &VegetationPatchFeature::_placeGroundPatch,
    std::vector<BlockPos>,
    ::IBlockWorldGenAPI& target,
    ::Random&            random,
    ::BlockPos const&    patchOrigin,
    int                  xRadius,
    int                  zRadius
) {
    auto& region = static_cast<WorldBlockTarget&>(target).mBlockSource;
    auto& bus    = ll::event::EventBus::getInstance();

    MossGrowthBeforeEvent beforeEvent(region, patchOrigin, xRadius, zRadius);
    bus.publish(beforeEvent);

    if (beforeEvent.isCancelled()) {
        return std::vector<BlockPos>{};
    }

    auto result = origin(target, random, patchOrigin, xRadius, zRadius);

    MossGrowthAfterEvent afterEvent(region, patchOrigin, xRadius, zRadius);
    bus.publish(afterEvent);

    return result;
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class MossGrowthBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, MossGrowthBeforeEvent> {
    ll::memory::HookRegistrar<MossGrowthEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<MossGrowthBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class MossGrowthAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, MossGrowthAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<MossGrowthAfterEventEmitter>();
}

} // namespace Catalyst