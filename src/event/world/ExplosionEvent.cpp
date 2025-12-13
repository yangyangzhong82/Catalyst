#include "ExplosionEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/Explosion.h"

namespace Catalyst {

LL_AUTO_TYPE_INSTANCE_HOOK(
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
class ExplosionAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, ExplosionAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<ExplosionAfterEventEmitter>();
}

} // namespace Catalyst