#include "PlayerUseFrameBlockEvent.h"

#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/level/block/ItemFrameBlock.h"
#include "mc/world/level/block/block_events/BlockPlayerInteractEvent.h"


#include "mc/deps/nbt/CompoundTag.h"

namespace Catalyst {

void PlayerUseFrameBlockEvent::serialize(CompoundTag& nbt) const {
    ll::event::PlayerEvent::serialize(nbt);
    nbt["pos"] = ListTag{pos().x, pos().y, pos().z};
}


LL_TYPE_INSTANCE_HOOK(
    PlayerUseFrameBlockEventHook,
    ll::memory::HookPriority::Normal,
    ItemFrameBlock,
    &ItemFrameBlock::$attack,
    bool,
    ::Player*         player,
    ::BlockPos const& pos
) {
    auto& bus = ll::event::EventBus::getInstance();

    PlayerUseFrameBlockBeforeEvent beforeEvent(*player, pos);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return false;
    }

    bool result = origin(player, pos);

    PlayerUseFrameBlockAfterEvent afterEvent(*player, pos);
    bus.publish(afterEvent);

    return result;
}

LL_TYPE_INSTANCE_HOOK(
    PlayerUseFrameBlockEventHook2,
    ll::memory::HookPriority::Normal,
    ItemFrameBlock,
    &ItemFrameBlock::use,
    void,
    ::BlockEvents::BlockPlayerInteractEvent& eventData
) {
    auto& bus = ll::event::EventBus::getInstance();

    PlayerUseFrameBlockBeforeEvent beforeEvent(eventData.mPlayer, eventData.mPos);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(eventData);

    PlayerUseFrameBlockAfterEvent afterEvent(eventData.mPlayer, eventData.mPos);
    bus.publish(afterEvent);
}

CATALYST_HOOKED_EVENT_PAIR(
    PlayerUseFrameBlockBeforeEvent,
    PlayerUseFrameBlockAfterEvent,
    PlayerUseFrameBlockEventHook,
    PlayerUseFrameBlockEventHook2
)

} // namespace Catalyst
