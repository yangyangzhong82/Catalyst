#include "PlayerCompleteUsingItemEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/item/ItemStack.h"

namespace Catalyst {

LL_TYPE_INSTANCE_HOOK(
    PlayerCompleteUsingItemEventHook,
    HookPriority::Normal,
    Player,
    &Player::completeUsingItem,
    void
) {
    ItemStack const& item = getCarriedItem();
    auto&            bus  = ll::event::EventBus::getInstance();

    PlayerCompleteUsingItemBeforeEvent beforeEvent(*this, item);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin();

    PlayerCompleteUsingItemAfterEvent afterEvent(*this, item);
    bus.publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class PlayerCompleteUsingItemBeforeEventEmitter
: public ll::event::Emitter<beforeEmitterFactory, PlayerCompleteUsingItemBeforeEvent> {
    ll::memory::HookRegistrar<PlayerCompleteUsingItemEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<PlayerCompleteUsingItemBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class PlayerCompleteUsingItemAfterEventEmitter
: public ll::event::Emitter<afterEmitterFactory, PlayerCompleteUsingItemAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<PlayerCompleteUsingItemAfterEventEmitter>();
}

} // namespace Catalyst