#include "PlayerChangeDimensionEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/level/ChangeDimensionRequest.h"
#include "mc/world/level/Level.h"

namespace Catalyst {

LL_AUTO_TYPE_INSTANCE_HOOK(
    PlayerChangeDimensionEventHook,
    HookPriority::Normal,
    Level,
    &Level::$requestPlayerChangeDimension,
    void,
    Player&                  player,
    ChangeDimensionRequest&& changeRequest
) {
    int  fromId    = player.getDimensionId();
    int  toId      = changeRequest.mToDimensionId->id;
    bool respawn   = changeRequest.mRespawn;
    bool usePortal = changeRequest.mUsePortal;

    auto& bus = ll::event::EventBus::getInstance();

    PlayerChangeDimensionBeforeEvent beforeEvent(player, fromId, toId, respawn, usePortal);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(player, std::move(changeRequest));

    PlayerChangeDimensionAfterEvent afterEvent(player, fromId, toId, respawn, usePortal);
    bus.publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class PlayerChangeDimensionBeforeEventEmitter
: public ll::event::Emitter<beforeEmitterFactory, PlayerChangeDimensionBeforeEvent> {
    ll::memory::HookRegistrar<PlayerChangeDimensionEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<PlayerChangeDimensionBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class PlayerChangeDimensionAfterEventEmitter
: public ll::event::Emitter<afterEmitterFactory, PlayerChangeDimensionAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<PlayerChangeDimensionAfterEventEmitter>();
}

} // namespace Catalyst