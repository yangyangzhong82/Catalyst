#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"


#include "catalyst/Macros.h"

class MobEffectInstance;
namespace Catalyst {

class CATALYST_API ActorEffectAddEvent : public ll::event::entity::ActorEvent {
    MobEffectInstance const& mEffect;

public:
    constexpr ActorEffectAddEvent(Actor& actor, MobEffectInstance const& effect)
    : ActorEvent(actor),
      mEffect(effect) {}

    void serialize(CompoundTag&) const override;

    MobEffectInstance const& effect() const { return mEffect; }
};

class CATALYST_API ActorEffectAddBeforeEvent final : public ll::event::Cancellable<ActorEffectAddEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API ActorEffectAddAfterEvent final : public ActorEffectAddEvent {
public:
    using ActorEffectAddEvent::ActorEffectAddEvent;
};

class CATALYST_API ActorEffectUpdateEvent : public ll::event::entity::ActorEvent {
    MobEffectInstance& mEffect;

public:
    constexpr ActorEffectUpdateEvent(Actor& actor, MobEffectInstance& effect)
    : ActorEvent(actor),
      mEffect(effect) {}

    void serialize(CompoundTag&) const override;

    MobEffectInstance& effect() const { return mEffect; }
};

class CATALYST_API ActorEffectUpdateBeforeEvent final : public ll::event::Cancellable<ActorEffectUpdateEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API ActorEffectUpdateAfterEvent final : public ActorEffectUpdateEvent {
public:
    using ActorEffectUpdateEvent::ActorEffectUpdateEvent;
};

class CATALYST_API ActorEffectRemoveEvent : public ll::event::entity::ActorEvent {
    MobEffectInstance& mEffect;

public:
    constexpr ActorEffectRemoveEvent(Actor& actor, MobEffectInstance& effect)
    : ActorEvent(actor),
      mEffect(effect) {}

    void serialize(CompoundTag&) const override;

    MobEffectInstance& effect() const { return mEffect; }
};

class CATALYST_API ActorEffectRemoveBeforeEvent final : public ll::event::Cancellable<ActorEffectRemoveEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API ActorEffectRemoveAfterEvent final : public ActorEffectRemoveEvent {
public:
    using ActorEffectRemoveEvent::ActorEffectRemoveEvent;
};

} // namespace Catalyst