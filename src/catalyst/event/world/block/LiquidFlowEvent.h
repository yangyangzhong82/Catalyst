#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/world/level/BlockPos.h"

#include "catalyst/Macros.h"

class LiquidBlock;

namespace Catalyst {

// Fired when a LiquidBlock is about to spread to a target position.
class CATALYST_API LiquidFlowEvent : public ll::event::world::WorldEvent {
    LiquidBlock const& mLiquidBlock;
    BlockPos const&    mToPos;
    int                mNeighbor;
    BlockPos const&    mFromPos;
    uchar              mFlowFromDirection;

public:
    constexpr LiquidFlowEvent(
        BlockSource&      region,
        LiquidBlock const& liquidBlock,
        BlockPos const&   toPos,
        int               neighbor,
        BlockPos const&   fromPos,
        uchar             flowFromDirection
    )
    : WorldEvent(region),
      mLiquidBlock(liquidBlock),
      mToPos(toPos),
      mNeighbor(neighbor),
      mFromPos(fromPos),
      mFlowFromDirection(flowFromDirection) {}

    void serialize(CompoundTag&) const override;

    LiquidBlock const& liquidBlock() const { return mLiquidBlock; }
    BlockPos const&    toPos() const { return mToPos; }
    int                neighbor() const { return mNeighbor; }
    BlockPos const&    fromPos() const { return mFromPos; }
    uchar              flowFromDirection() const { return mFlowFromDirection; }
};

class CATALYST_API LiquidFlowBeforeEvent final : public ll::event::Cancellable<LiquidFlowEvent> {
public:
    using Cancellable::Cancellable;
};

// Fired after a LiquidBlock spread attempt has been executed.
class CATALYST_API LiquidFlowAfterEvent final : public LiquidFlowEvent {
public:
    using LiquidFlowEvent::LiquidFlowEvent;
};

} // namespace Catalyst

