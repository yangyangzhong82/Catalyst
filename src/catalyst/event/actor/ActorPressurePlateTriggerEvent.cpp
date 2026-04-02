#include "ActorPressurePlateTriggerEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/block/BasePressurePlateBlock.h"

#include "mc/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void ActorPressurePlateTriggerBeforeEvent::serialize(CompoundTag& nbt) const {
    Cancellable::serialize(nbt);
    nbt["block"]  = ll::event::serializeRefObj(block());
    nbt["region"] = ll::event::serializeRefObj(region());
    nbt["pos"]    = ListTag{pos().x, pos().y, pos().z};
}


LL_TYPE_INSTANCE_HOOK(
    ActorPressurePlateTriggerEventHook,
    ll::memory::HookPriority::Normal,
    BasePressurePlateBlock,
    &BasePressurePlateBlock::$shouldTriggerEntityInside,
    bool,
    BlockSource&    region,
    BlockPos const& pos,
    Actor&          entity
) {
    auto& bus = ll::event::EventBus::getInstance();

    ActorPressurePlateTriggerBeforeEvent beforeEvent(entity, *this, region, pos);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return false;
    }

    return origin(region, pos, entity);
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory();
class ActorPressurePlateTriggerBeforeEventEmitter
: public ll::event::Emitter<emitterFactory, ActorPressurePlateTriggerBeforeEvent> {
    ll::memory::HookRegistrar<ActorPressurePlateTriggerEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> emitterFactory() {
    return std::make_unique<ActorPressurePlateTriggerBeforeEventEmitter>();
}

} // namespace Catalyst