#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API ActorRideEvent : public ll::event::entity::ActorEvent {
    Actor& mPassenger;

public:
    constexpr ActorRideEvent(Actor& vehicle, Actor& passenger) : ActorEvent(vehicle), mPassenger(passenger) {}

    void serialize(CompoundTag&) const override;

    Actor& passenger() const { return mPassenger; }
};

class CATALYST_API ActorRideBeforeEvent final : public ll::event::Cancellable<ActorRideEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API ActorRideAfterEvent final : public ActorRideEvent {
public:
    using ActorRideEvent::ActorRideEvent;
};

} // namespace Catalyst
