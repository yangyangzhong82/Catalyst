#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API ActorRideBeforeEvent final : public ll::event::Cancellable<ll::event::entity::ActorEvent> {
    Actor& mPassenger;

public:
    constexpr ActorRideBeforeEvent(Actor& vehicle, Actor& passenger) : Cancellable(vehicle), mPassenger(passenger) {}

    Actor& passenger() const { return mPassenger; }
};

class CATALYST_API ActorRideAfterEvent final : public ll::event::entity::ActorEvent {
    Actor& mPassenger;

public:
    constexpr ActorRideAfterEvent(Actor& vehicle, Actor& passenger) : ActorEvent(vehicle), mPassenger(passenger) {}

    Actor& passenger() const { return mPassenger; }
};

} // namespace Catalyst
