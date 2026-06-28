#include "ActorRideEvent.h"

#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/Actor.h"

#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void ActorRideEvent::serialize(CompoundTag& nbt) const {
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

CATALYST_HOOKED_EVENT_PAIR(
    ActorRideBeforeEvent,
    ActorRideAfterEvent,
    ActorRideEventHook
)

} // namespace Catalyst
