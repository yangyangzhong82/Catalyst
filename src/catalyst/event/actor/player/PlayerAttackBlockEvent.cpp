#include "PlayerAttackBlockEvent.h"

#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/network/ServerPlayerBlockUseHandler.h"
#include "mc/server/ServerPlayer.h"

#include "mc/deps/nbt/CompoundTag.h"

namespace Catalyst {

void PlayerAttackBlockEvent::serialize(CompoundTag& nbt) const {
    ll::event::ServerPlayerEvent::serialize(nbt);
    nbt["pos"]  = ListTag{pos().x, pos().y, pos().z};
    nbt["face"] = face();
}


LL_STATIC_HOOK(
    PlayerAttackBlockEventHook,
    HookPriority::Normal,
    &ServerPlayerBlockUseHandler::onStartDestroyBlock,
    void,
    ServerPlayer&   player,
    const BlockPos& pos,
    int             face
) {
    auto& bus = ll::event::EventBus::getInstance();

    PlayerAttackBlockBeforeEvent beforeEvent(player, pos, face);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(player, pos, face);

    PlayerAttackBlockAfterEvent afterEvent(player, pos, face);
    bus.publish(afterEvent);
}

CATALYST_HOOKED_EVENT_PAIR(
    PlayerAttackBlockBeforeEvent,
    PlayerAttackBlockAfterEvent,
    PlayerAttackBlockEventHook
)

} // namespace Catalyst
