#include "PlayerShieldBlockEvent.h"

#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/actor/ActorDamageSource.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/level/Level.h"

#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void PlayerShieldBlockEvent::serialize(CompoundTag& nbt) const {
    ll::event::PlayerEvent::serialize(nbt);
    nbt["source"]  = ll::event::serializeRefObj(source());
    nbt["damage"]  = damage();
    nbt["damager"] = ll::event::serializePtrObj(damager());
}

void PlayerShieldBlockAfterEvent::serialize(CompoundTag& nbt) const {
    PlayerShieldBlockEvent::serialize(nbt);
    nbt["result"] = result();
}


LL_TYPE_INSTANCE_HOOK(
    PlayerShieldBlockEventHook,
    HookPriority::Normal,
    Player,
    &Player::_blockUsingShield,
    bool,
    ::ActorDamageSource const& source,
    float                      damage
) {
    auto& bus = ll::event::EventBus::getInstance();

    auto   damagerId = source.getDamagingEntityUniqueID();
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

CATALYST_HOOKED_EVENT_PAIR(
    PlayerShieldBlockBeforeEvent,
    PlayerShieldBlockAfterEvent,
    PlayerShieldBlockEventHook
)

} // namespace Catalyst
