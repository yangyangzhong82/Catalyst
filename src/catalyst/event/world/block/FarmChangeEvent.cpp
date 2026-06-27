#include "FarmChangeEvent.h"

#include "ll/api/event/Emitter.h"
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

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class FarmChangeBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, FarmChangeBeforeEvent> {
    ll::memory::HookRegistrar<FarmChangeEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<FarmChangeBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class FarmChangeAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, FarmChangeAfterEvent> {
    ll::memory::HookRegistrar<FarmChangeEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<FarmChangeAfterEventEmitter>();
}

} // namespace Catalyst
