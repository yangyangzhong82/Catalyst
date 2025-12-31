#include "ActorPressurePlateTriggerEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"

namespace Catalyst {

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