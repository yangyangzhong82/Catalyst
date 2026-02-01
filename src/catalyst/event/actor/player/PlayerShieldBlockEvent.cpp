#include "PlayerShieldBlockEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/actor/ActorDamageSource.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/level/Level.h"

namespace Catalyst {

LL_AUTO_TYPE_INSTANCE_HOOK(
    PlayerShieldBlockEventHook,
    HookPriority::Normal,
    Player,
    &Player::_blockUsingShield,
    bool,
    ::ActorDamageSource const& source,
    float                      damage
) {
    auto& bus = ll::event::EventBus::getInstance();

    auto  damagerId = source.getDamagingEntityUniqueID();
    Actor* damager   = getLevel().fetchEntity(damagerId, false);

    PlayerShieldBlockBeforeEvent beforeEvent(*this, source, damage, damager);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return false;
    }

    bool result = origin(source, damage);

    PlayerShieldBlockAfterEvent afterEvent(*this, source, damage, damager, result);
    bus.publish(afterEvent);

    return result;
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class PlayerShieldBlockBeforeEventEmitter
: public ll::event::Emitter<beforeEmitterFactory, PlayerShieldBlockBeforeEvent> {
    ll::memory::HookRegistrar<PlayerShieldBlockEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<PlayerShieldBlockBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class PlayerShieldBlockAfterEventEmitter
: public ll::event::Emitter<afterEmitterFactory, PlayerShieldBlockAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<PlayerShieldBlockAfterEventEmitter>();
}

} // namespace Catalyst

