#include "ExplosionEvent.h"

#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/Explosion.h"

#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void ExplosionEvent::serialize(CompoundTag& nbt) const {
    ll::event::world::WorldEvent::serialize(nbt);
    nbt["explosion"] = ll::event::serializeRefObj(explosion());
    nbt["pos"]       = ListTag{pos().x, pos().y, pos().z};
    nbt["radius"]    = radius();
}

void ExplosionBeforeEvent::serialize(CompoundTag& nbt) const {
    Cancellable::serialize(nbt);
    nbt["fire"]     = fire();
    nbt["breaking"] = breaking();
}
LL_TYPE_INSTANCE_HOOK(
    ExplosionEventHook,
    ll::memory::HookPriority::Normal,
    Explosion,
    &Explosion::explode,
    bool,
    ::IRandom& random
) {
    auto& bus = ll::event::EventBus::getInstance();

    ExplosionBeforeEvent beforeEvent(mRegion, *this);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return false;
    }

    bool result = origin(random);

    ExplosionAfterEvent afterEvent(mRegion, *this);
    bus.publish(afterEvent);

    return result;
}

CATALYST_HOOKED_EVENT_PAIR(
    ExplosionBeforeEvent,
    ExplosionAfterEvent,
    ExplosionEventHook
)

} // namespace Catalyst
