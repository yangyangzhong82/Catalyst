#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API ActorChangeDimensionBeforeEvent final
: public ll::event::Cancellable<ll::event::entity::ActorEvent> {
    int mFromDimensionId;
    int mToDimensionId;

public:
    constexpr ActorChangeDimensionBeforeEvent(Actor& actor, int fromDimensionId, int toDimensionId)
    : Cancellable(actor),
      mFromDimensionId(fromDimensionId),
      mToDimensionId(toDimensionId) {}

    int fromDimensionId() const { return mFromDimensionId; }
    int toDimensionId() const { return mToDimensionId; }
};

class CATALYST_API ActorChangeDimensionAfterEvent final : public ll::event::entity::ActorEvent {
    int mFromDimensionId;
    int mToDimensionId;

public:
    constexpr ActorChangeDimensionAfterEvent(Actor& actor, int fromDimensionId, int toDimensionId)
    : ActorEvent(actor),
      mFromDimensionId(fromDimensionId),
      mToDimensionId(toDimensionId) {}

    int fromDimensionId() const { return mFromDimensionId; }
    int toDimensionId() const { return mToDimensionId; }
};

} // namespace Catalyst
