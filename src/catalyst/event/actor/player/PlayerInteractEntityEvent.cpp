#include "PlayerInteractEntityEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/actor/player/Player.h"


#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void PlayerInteractEntityBeforeEvent::serialize(CompoundTag& nbt) const {
    Cancellable::serialize(nbt);
    nbt["actor"]    = ll::event::serializeRefObj(actor());
    nbt["location"] = ListTag{location().x, location().y, location().z};
}

void PlayerInteractEntityAfterEvent::serialize(CompoundTag& nbt) const {
    ll::event::PlayerEvent::serialize(nbt);
    nbt["actor"]         = ll::event::serializeRefObj(actor());
    nbt["location"]      = ListTag{location().x, location().y, location().z};
    nbt["resultSuccess"] = result().mSuccess;
    nbt["resultSwing"]   = result().mSwing;
}


LL_TYPE_INSTANCE_HOOK(
    InteractEntityHook,
    ll::memory::HookPriority::Normal,
    Player,
    &Player::interact,
    InteractionResult,
    Actor&            actor,
    Vec3 const&       location
) {
    auto& bus = ll::event::EventBus::getInstance();

    PlayerInteractEntityBeforeEvent beforeEvent(*this, actor, location);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return InteractionResult{};
    }

    InteractionResult result = origin(actor, location);

    PlayerInteractEntityAfterEvent afterEvent(*this, actor, location, result);
    bus.publish(afterEvent);

    return result;
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class PlayerInteractEntityBeforeEventEmitter
: public ll::event::Emitter<beforeEmitterFactory, PlayerInteractEntityBeforeEvent> {
    ll::memory::HookRegistrar<InteractEntityHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<PlayerInteractEntityBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class PlayerInteractEntityAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, PlayerInteractEntityAfterEvent> {
    ll::memory::HookRegistrar<InteractEntityHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<PlayerInteractEntityAfterEventEmitter>();
}

} // namespace Catalyst
