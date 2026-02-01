#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"

#include "catalyst/Macros.h"

class Actor;
class ActorDamageSource;

namespace Catalyst {

class CATALYST_API PlayerShieldBlockBeforeEvent final : public ll::event::Cancellable<ll::event::PlayerEvent> {
    ActorDamageSource const& mSource;
    float                    mDamage;
    Actor*                   mDamager;

public:
    constexpr PlayerShieldBlockBeforeEvent(Player& player, ActorDamageSource const& source, float damage, Actor* damager)
    : Cancellable(player),
      mSource(source),
      mDamage(damage),
      mDamager(damager) {}

    ActorDamageSource const& source() const { return mSource; }
    float                    damage() const { return mDamage; }
    Actor*                   damager() const { return mDamager; }
};

class CATALYST_API PlayerShieldBlockAfterEvent final : public ll::event::PlayerEvent {
    ActorDamageSource const& mSource;
    float                    mDamage;
    Actor*                   mDamager;
    bool                     mResult;

public:
    constexpr PlayerShieldBlockAfterEvent(
        Player&                 player,
        ActorDamageSource const& source,
        float                   damage,
        Actor*                  damager,
        bool                    result
    )
    : PlayerEvent(player),
      mSource(source),
      mDamage(damage),
      mDamager(damager),
      mResult(result) {}

    ActorDamageSource const& source() const { return mSource; }
    float                    damage() const { return mDamage; }
    Actor*                   damager() const { return mDamager; }
    bool                     result() const { return mResult; }
};

} // namespace Catalyst

