#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API ActorChangeDimensionEvent : public ll::event::entity::ActorEvent {
    int mFromDimensionId;
    int mToDimensionId;

public:
    constexpr ActorChangeDimensionEvent(Actor& actor, int fromDimensionId, int toDimensionId)
    : ActorEvent(actor),
      mFromDimensionId(fromDimensionId),
      mToDimensionId(toDimensionId) {}

    void serialize(CompoundTag&) const override;

    int fromDimensionId() const { return mFromDimensionId; }
    int toDimensionId() const { return mToDimensionId; }
};

class CATALYST_API ActorChangeDimensionBeforeEvent final
: public ll::event::Cancellable<ActorChangeDimensionEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API ActorChangeDimensionAfterEvent final : public ActorChangeDimensionEvent {
public:
    using ActorChangeDimensionEvent::ActorChangeDimensionEvent;
};

} // namespace Catalyst
