#include "FarmChangeEvent.h"

#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/block/FarmBlock.h"

#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void FarmChangeEvent::serialize(CompoundTag& nbt) const {
    ll::event::world::WorldEvent::serialize(nbt);
    nbt["pos"]        = ListTag{pos().x, pos().y, pos().z};
    nbt["actor"]      = ll::event::serializePtrObj(actor());
    nbt["toFarmland"] = toFarmland();
}


LL_TYPE_INSTANCE_HOOK(
    FarmChangeEventHook,
    ll::memory::HookPriority::Normal,
    FarmBlock,
    &FarmBlock::$transformOnFall,
    void,
    ::BlockSource&    region,
    ::BlockPos const& pos,
    ::Actor*          actor,
    float             fallDistance
) {
    auto& bus = ll::event::EventBus::getInstance();

    FarmChangeBeforeEvent beforeEvent(region, pos, actor, false);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(region, pos, actor, fallDistance);

    FarmChangeAfterEvent afterEvent(region, pos, actor, false);
    bus.publish(afterEvent);
}

CATALYST_HOOKED_EVENT_PAIR(
    FarmChangeBeforeEvent,
    FarmChangeAfterEvent,
    FarmChangeEventHook
)

} // namespace Catalyst
