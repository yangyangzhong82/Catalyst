#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/world/level/BlockPos.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API SculkSpreadBeforeEvent final : public ll::event::Cancellable<ll::event::world::WorldEvent> {
    BlockPos mPos;
    BlockPos mTargetPos;
    uchar    mStartingFace;
    uchar    mSpreadDirection;

public:
    constexpr SculkSpreadBeforeEvent(
        BlockSource& blockSource,
        BlockPos     pos,
        BlockPos     targetPos,
        uchar        startingFace,
        uchar        spreadDirection
    )
    : Cancellable(blockSource),
      mPos(pos),
      mTargetPos(targetPos),
      mStartingFace(startingFace),
      mSpreadDirection(spreadDirection) {}

    BlockPos const& pos() const { return mPos; }
    BlockPos const& targetPos() const { return mTargetPos; }
    uchar           startingFace() const { return mStartingFace; }
    uchar           spreadDirection() const { return mSpreadDirection; }
};

class CATALYST_API SculkSpreadAfterEvent final : public ll::event::world::WorldEvent {
    BlockPos mPos;
    BlockPos mTargetPos;
    uchar    mStartingFace;
    uchar    mSpreadDirection;

public:
    constexpr SculkSpreadAfterEvent(
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

    BlockPos const& pos() const { return mPos; }
    BlockPos const& targetPos() const { return mTargetPos; }
    uchar           startingFace() const { return mStartingFace; }
    uchar           spreadDirection() const { return mSpreadDirection; }
};

} // namespace Catalyst