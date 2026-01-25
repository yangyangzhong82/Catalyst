#include "ActorRideEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/Actor.h"

namespace Catalyst {

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
class ActorRideAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, ActorRideAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<ActorRideAfterEventEmitter>();
}

} // namespace Catalyst