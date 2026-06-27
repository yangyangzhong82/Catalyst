#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"

#include "catalyst/Macros.h"
class BlockPos;

namespace Catalyst {

class CATALYST_API SculkSpreadEvent : public ll::event::world::WorldEvent {
    BlockPos mPos;
    BlockPos mTargetPos;
    uchar    mStartingFace;
    uchar    mSpreadDirection;

public:
    constexpr SculkSpreadEvent(
        BlockSource& blockSource,
        BlockPos     pos,
        BlockPos     targetPos,
        uchar        startingFace,
        uchar        spreadDirection
    )
    : WorldEvent(blockSource),
      mPos(pos),
      mTargetPos(targetPos),
      mStartingFace(startingFace),
      mSpreadDirection(spreadDirection) {}

    void serialize(CompoundTag&) const override;

    BlockPos const& pos() const { return mPos; }
    BlockPos const& targetPos() const { return mTargetPos; }
    uchar           startingFace() const { return mStartingFace; }
    uchar           spreadDirection() const { return mSpreadDirection; }
};

class CATALYST_API SculkSpreadBeforeEvent final : public ll::event::Cancellable<SculkSpreadEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API SculkSpreadAfterEvent final : public SculkSpreadEvent {
public:
    using SculkSpreadEvent::SculkSpreadEvent;
};

} // namespace Catalyst