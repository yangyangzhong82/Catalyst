#include "MossGrowthEvent.h"

#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/WorldBlockTarget.h"
#include "mc/world/level/levelgen/feature/VegetationPatchFeature.h"

#include "mc/deps/nbt/CompoundTag.h"

namespace Catalyst {

void MossGrowthEvent::serialize(CompoundTag& nbt) const {
    ll::event::world::WorldEvent::serialize(nbt);
    nbt["origin"]  = ListTag{origin().x, origin().y, origin().z};
    nbt["xRadius"] = xRadius();
    nbt["zRadius"] = zRadius();
}


LL_TYPE_INSTANCE_HOOK(
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

CATALYST_HOOKED_EVENT_PAIR(
    MossGrowthBeforeEvent,
    MossGrowthAfterEvent,
    MossGrowthEventHook
)

} // namespace Catalyst
