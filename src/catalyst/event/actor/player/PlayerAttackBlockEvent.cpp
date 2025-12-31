#include "PlayerAttackBlockEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/network/ServerPlayerBlockUseHandler.h"
#include "mc/server/ServerPlayer.h"

namespace Catalyst {

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

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class PlayerAttackBlockBeforeEventEmitter
: public ll::event::Emitter<beforeEmitterFactory, PlayerAttackBlockBeforeEvent> {
    ll::memory::HookRegistrar<PlayerAttackBlockEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<PlayerAttackBlockBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class PlayerAttackBlockAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, PlayerAttackBlockAfterEvent> {
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<PlayerAttackBlockAfterEventEmitter>();
}

} // namespace Catalyst