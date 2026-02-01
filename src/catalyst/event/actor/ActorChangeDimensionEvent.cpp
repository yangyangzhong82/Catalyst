#include "ActorChangeDimensionEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/actor/Mob.h"
#include "mc/world/level/Level.h"

namespace Catalyst {

LL_TYPE_INSTANCE_HOOK(
    ActorChangeDimensionEventHook,
    HookPriority::Normal,
    Level,
    &Level::$entityChangeDimension,
    void,
    class Actor&              entity,
    DimensionType             toId,
    std::optional<class Vec3> entityPos
) {
    int fromId  = entity.getDimensionId();
    int toIdInt = static_cast<int>(toId);

    auto& bus = ll::event::EventBus::getInstance();

    ActorChangeDimensionBeforeEvent beforeEvent(entity, fromId, toIdInt);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(entity, toId, entityPos);

    ActorChangeDimensionAfterEvent afterEvent(entity, fromId, toIdInt);
    bus.publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class ActorChangeDimensionBeforeEventEmitter
: public ll::event::Emitter<beforeEmitterFactory, ActorChangeDimensionBeforeEvent> {
    ll::memory::HookRegistrar<ActorChangeDimensionEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<ActorChangeDimensionBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class ActorChangeDimensionAfterEventEmitter
: public ll::event::Emitter<afterEmitterFactory, ActorChangeDimensionAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<ActorChangeDimensionAfterEventEmitter>();
}

} // namespace Catalyst