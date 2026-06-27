#include "PlayerStopSleepEvent.h"

#include "ll/api/event/Emitter.h"
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

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class PlayerStopSleepBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, PlayerStopSleepBeforeEvent> {
    ll::memory::HookRegistrar<PlayerStopSleepEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<PlayerStopSleepBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class PlayerStopSleepAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, PlayerStopSleepAfterEvent> {
    ll::memory::HookRegistrar<PlayerStopSleepEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<PlayerStopSleepAfterEventEmitter>();
}

} // namespace Catalyst
