#include "PlayerStopSleepEvent.h"

#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/player/Player.h"

#include "mc/deps/nbt/CompoundTag.h"

namespace Catalyst {

void PlayerStopSleepEvent::serialize(CompoundTag& nbt) const {
    ll::event::PlayerEvent::serialize(nbt);
    nbt["forcefulWakeUp"]  = isForcefulWakeUp();
    nbt["updateLevelList"] = isUpdateLevelList();
}


LL_TYPE_INSTANCE_HOOK(
    PlayerStopSleepEventHook,
    ll::memory::HookPriority::Normal,
    Player,
    &Player::$stopSleepInBed,
    void,
    bool forcefulWakeUp,
    bool updateLevelList
) {
    auto& bus = ll::event::EventBus::getInstance();

    PlayerStopSleepBeforeEvent beforeEvent(*this, forcefulWakeUp, updateLevelList);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(forcefulWakeUp, updateLevelList);

    PlayerStopSleepAfterEvent afterEvent(*this, forcefulWakeUp, updateLevelList);
    bus.publish(afterEvent);
}

CATALYST_HOOKED_EVENT_PAIR(
    PlayerStopSleepBeforeEvent,
    PlayerStopSleepAfterEvent,
    PlayerStopSleepEventHook
)

} // namespace Catalyst
