#include "PlayerChangeDimensionEvent.h"

#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/level/ChangeDimensionRequest.h"
#include "mc/world/level/Level.h"

#include "mc/deps/nbt/CompoundTag.h"

namespace Catalyst {

void PlayerChangeDimensionEvent::serialize(CompoundTag& nbt) const {
    ll::event::PlayerEvent::serialize(nbt);
    nbt["fromDimensionId"] = fromDimensionId();
    nbt["toDimensionId"]   = toDimensionId();
    nbt["respawn"]         = isRespawn();
    nbt["usePortal"]       = isUsePortal();
}


LL_TYPE_INSTANCE_HOOK(
    PlayerChangeDimensionEventHook,
    HookPriority::Normal,
    Level,
    &Level::$requestPlayerChangeDimension,
    void,
    Player&                  player,
    ChangeDimensionRequest&& changeRequest
) {
    int  fromId    = player.getDimensionId();
    int  toId      = changeRequest.mToDimensionId->mValue;
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

CATALYST_HOOKED_EVENT_PAIR(
    PlayerChangeDimensionBeforeEvent,
    PlayerChangeDimensionAfterEvent,
    PlayerChangeDimensionEventHook
)

} // namespace Catalyst
