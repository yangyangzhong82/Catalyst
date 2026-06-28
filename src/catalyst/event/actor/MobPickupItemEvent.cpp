
#include "MobPickupItemEvent.h"

#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/Mob.h"
#include "mc/world/actor/ai/goal/PickupItemsGoal.h"
#include "mc/world/actor/item/ItemActor.h"

#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void MobPickupItemEvent::serialize(CompoundTag& nbt) const {
    ll::event::entity::ActorEvent::serialize(nbt);
    nbt["itemActor"] = ll::event::serializeRefObj(itemActor());
}


LL_TYPE_INSTANCE_HOOK(
    MobPickupItemHook,
    HookPriority::Normal,
    PickupItemsGoal,
    &PickupItemsGoal::_pickItemUp,
    void,
    ::ItemActor& itemActor
) {
    auto& mob = this->mMob;

    auto& bus = ll::event::EventBus::getInstance();

    MobPickupItemBeforeEvent beforeEvent(mob, itemActor);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(itemActor);

    MobPickupItemAfterEvent afterEvent(mob, itemActor);
    bus.publish(afterEvent);
}

CATALYST_HOOKED_EVENT_PAIR(
    MobPickupItemBeforeEvent,
    MobPickupItemAfterEvent,
    MobPickupItemHook
)

} // namespace Catalyst
