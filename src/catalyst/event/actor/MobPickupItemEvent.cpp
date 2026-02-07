
#include "MobPickupItemEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/Mob.h"
#include "mc/world/actor/ai/goal/PickupItemsGoal.h"
#include "mc/world/actor/item/ItemActor.h"

namespace Catalyst {

LL_TYPE_INSTANCE_HOOK(
    MobPickupItemHook,
    HookPriority::Normal,
    PickupItemsGoal,
    &PickupItemsGoal::_pickItemUp,
    void,
    ::ItemActor* itemActor
) {
    auto& mob = this->mMob;

    if (!itemActor) {
        origin(itemActor);
        return;
    }

    auto& bus = ll::event::EventBus::getInstance();

    MobPickupItemBeforeEvent beforeEvent(mob, *itemActor);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(itemActor);

    MobPickupItemAfterEvent afterEvent(mob, *itemActor);
    bus.publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class MobPickupItemBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, MobPickupItemBeforeEvent> {
    ll::memory::HookRegistrar<MobPickupItemHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<MobPickupItemBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class MobPickupItemAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, MobPickupItemAfterEvent> {
    ll::memory::HookRegistrar<MobPickupItemHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<MobPickupItemAfterEventEmitter>();
}

} // namespace Catalyst