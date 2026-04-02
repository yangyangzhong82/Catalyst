#include "ExplosionEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/Explosion.h"

#include "mc/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void ExplosionBeforeEvent::serialize(CompoundTag& nbt) const {
    Cancellable::serialize(nbt);
    nbt["explosion"] = ll::event::serializeRefObj(explosion());
    nbt["pos"]       = ListTag{pos().x, pos().y, pos().z};
    nbt["radius"]    = radius();
    nbt["fire"]      = fire();
    nbt["breaking"]  = breaking();
}

void ExplosionAfterEvent::serialize(CompoundTag& nbt) const {
    ll::event::world::WorldEvent::serialize(nbt);
    nbt["explosion"] = ll::event::serializeRefObj(explosion());
    nbt["pos"]       = ListTag{pos().x, pos().y, pos().z};
    nbt["radius"]    = radius();
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

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class ExplosionBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, ExplosionBeforeEvent> {
    ll::memory::HookRegistrar<ExplosionEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<ExplosionBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class ExplosionAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, ExplosionAfterEvent> {
    ll::memory::HookRegistrar<ExplosionEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<ExplosionAfterEventEmitter>();
}

} // namespace Catalyst
