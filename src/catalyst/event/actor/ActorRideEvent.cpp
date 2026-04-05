#include "ActorRideEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/Actor.h"

#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void ActorRideBeforeEvent::serialize(CompoundTag& nbt) const {
    Cancellable::serialize(nbt);
    nbt["passenger"] = ll::event::serializeRefObj(passenger());
}

void ActorRideAfterEvent::serialize(CompoundTag& nbt) const {
    ll::event::entity::ActorEvent::serialize(nbt);
    nbt["passenger"] = ll::event::serializeRefObj(passenger());
}


LL_TYPE_INSTANCE_HOOK(
    ActorRideEventHook,
    ll::memory::HookPriority::Normal,
    Actor,
    &Actor::$addPassenger,
    void,
    Actor& passenger
) {
    auto& bus = ll::event::EventBus::getInstance();

    ActorRideBeforeEvent beforeEvent(*this, passenger);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(passenger);

    ActorRideAfterEvent afterEvent(*this, passenger);
    bus.publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class ActorRideBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, ActorRideBeforeEvent> {
    ll::memory::HookRegistrar<ActorRideEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<ActorRideBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class ActorRideAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, ActorRideAfterEvent> {
    ll::memory::HookRegistrar<ActorRideEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<ActorRideAfterEventEmitter>();
}

} // namespace Catalyst
