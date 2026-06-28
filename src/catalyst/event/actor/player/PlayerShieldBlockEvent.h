#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"

#include "catalyst/Macros.h"

class Actor;
class ActorDamageSource;

namespace Catalyst {

class CATALYST_API PlayerShieldBlockEvent : public ll::event::PlayerEvent {
    ActorDamageSource const& mSource;
    float                    mDamage;
    Actor*                   mDamager;

public:
    constexpr PlayerShieldBlockEvent(Player& player, ActorDamageSource const& source, float damage, Actor* damager)
    : PlayerEvent(player),
      mSource(source),
      mDamage(damage),
      mDamager(damager) {}

    void serialize(CompoundTag&) const override;

    ActorDamageSource const& source() const { return mSource; }
    float                    damage() const { return mDamage; }
    Actor*                   damager() const { return mDamager; }
};

class CATALYST_API PlayerShieldBlockBeforeEvent final : public ll::event::Cancellable<PlayerShieldBlockEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API PlayerShieldBlockAfterEvent final : public PlayerShieldBlockEvent {
    bool mResult;

public:
    constexpr PlayerShieldBlockAfterEvent(
        Player&                  player,
        ActorDamageSource const& source,
        float                    damage,
        Actor*                   damager,
        bool                     result
    )
    : PlayerShieldBlockEvent(player, source, damage, damager),
      mResult(result) {}

    void serialize(CompoundTag&) const override;

    bool result() const { return mResult; }
};

} // namespace Catalyst

