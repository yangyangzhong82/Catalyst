#include "MobHealthChangeEvent.h"

#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/Mob.h"
#include "mc/world/attribute/AttributeBuff.h"
#include "mc/world/attribute/HealthAttributeDelegate.h"

#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void MobHealthChangeEvent::serialize(CompoundTag& nbt) const {
    ll::event::entity::MobEvent::serialize(nbt);
    nbt["oldValue"] = oldValue();
    nbt["newValue"] = newValue();
    nbt["buff"]     = ll::event::serializeRefObj(buff());
}


LL_TYPE_INSTANCE_HOOK(
    MobHealthChangeHook,
    ll::memory::HookPriority::Normal,
    HealthAttributeDelegate,
    &HealthAttributeDelegate::$change,
    std::optional<float>,
    float                  oldValue,
    float                  newValue,
    ::AttributeBuff const& buff
) {
    auto mob = this->mMob;
    if (!mob) {
        return origin(oldValue, newValue, buff);
    }

    auto& bus = ll::event::EventBus::getInstance();

    MobHealthChangeBeforeEvent beforeEvent(*mob, oldValue, newValue, buff);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return oldValue;
    }

    std::optional<float> result = origin(oldValue, newValue, buff);

    MobHealthChangeAfterEvent afterEvent(*mob, oldValue, newValue, buff);
    bus.publish(afterEvent);

    return result;
}

CATALYST_HOOKED_EVENT_PAIR(
    MobHealthChangeBeforeEvent,
    MobHealthChangeAfterEvent,
    MobHealthChangeHook
)

} // namespace Catalyst
