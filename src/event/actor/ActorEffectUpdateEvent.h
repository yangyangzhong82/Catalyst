#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"
#include "mc/world/effect/MobEffectInstance.h"

#include "Macros.h"

namespace Catalyst {

class CATALYST_API ActorEffectAddBeforeEvent final : public ll::event::Cancellable<ll::event::entity::ActorEvent> {
    MobEffectInstance const& mEffect;

public:
    constexpr ActorEffectAddBeforeEvent(Actor& actor, MobEffectInstance const& effect)
    : Cancellable(actor),
      mEffect(effect) {}

    MobEffectInstance const& effect() const { return mEffect; }
};

class CATALYST_API ActorEffectAddAfterEvent final : public ll::event::entity::ActorEvent {
    MobEffectInstance const& mEffect;

public:
    constexpr ActorEffectAddAfterEvent(Actor& actor, MobEffectInstance const& effect)
    : ActorEvent(actor),
      mEffect(effect) {}

    MobEffectInstance const& effect() const { return mEffect; }
};

class CATALYST_API ActorEffectUpdateBeforeEvent final : public ll::event::Cancellable<ll::event::entity::ActorEvent> {
    MobEffectInstance& mEffect;

public:
    constexpr ActorEffectUpdateBeforeEvent(Actor& actor, MobEffectInstance& effect)
    : Cancellable(actor),
      mEffect(effect) {}

    MobEffectInstance& effect() const { return mEffect; }
};

class CATALYST_API ActorEffectUpdateAfterEvent final : public ll::event::entity::ActorEvent {
    MobEffectInstance& mEffect;

public:
    constexpr ActorEffectUpdateAfterEvent(Actor& actor, MobEffectInstance& effect)
    : ActorEvent(actor),
      mEffect(effect) {}

    MobEffectInstance& effect() const { return mEffect; }
};

class CATALYST_API ActorEffectRemoveBeforeEvent final : public ll::event::Cancellable<ll::event::entity::ActorEvent> {
    MobEffectInstance& mEffect;

public:
    constexpr ActorEffectRemoveBeforeEvent(Actor& actor, MobEffectInstance& effect)
    : Cancellable(actor),
      mEffect(effect) {}

    MobEffectInstance& effect() const { return mEffect; }
};

class CATALYST_API ActorEffectRemoveAfterEvent final : public ll::event::entity::ActorEvent {
    MobEffectInstance& mEffect;

public:
    constexpr ActorEffectRemoveAfterEvent(Actor& actor, MobEffectInstance& effect)
    : ActorEvent(actor),
      mEffect(effect) {}

    MobEffectInstance& effect() const { return mEffect; }
};

} // namespace Catalyst