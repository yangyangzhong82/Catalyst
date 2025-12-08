#include "PlayerUseFrameBlockEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/level/block/ItemFrameBlock.h"
#include "mc/world/level/block/block_events/BlockPlayerInteractEvent.h"


namespace Catalyst {

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

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class PlayerUseFrameBlockBeforeEventEmitter
: public ll::event::Emitter<beforeEmitterFactory, PlayerUseFrameBlockBeforeEvent> {
    ll::memory::HookRegistrar<PlayerUseFrameBlockEventHook>  hook1;
    ll::memory::HookRegistrar<PlayerUseFrameBlockEventHook2> hook2;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<PlayerUseFrameBlockBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class PlayerUseFrameBlockAfterEventEmitter
: public ll::event::Emitter<afterEmitterFactory, PlayerUseFrameBlockAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<PlayerUseFrameBlockAfterEventEmitter>();
}

} // namespace Catalyst