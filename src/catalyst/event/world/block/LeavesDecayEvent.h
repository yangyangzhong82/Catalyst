#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/world/level/BlockPos.h"

#include "catalyst/Macros.h"

class Block;

namespace Catalyst {

class CATALYST_API LeavesDecayBeforeEvent final : public ll::event::Cancellable<ll::event::world::WorldEvent> {
    BlockPos       mPos;
    ::Block const& mLeavesBlock;

public:
    constexpr LeavesDecayBeforeEvent(BlockSource& blockSource, BlockPos const& pos, ::Block const& leavesBlock)
    : Cancellable(blockSource),
      mPos(pos),
      mLeavesBlock(leavesBlock) {}

    void serialize(CompoundTag&) const override;

    BlockPos const& pos() const { return mPos; }
    ::Block const&  leavesBlock() const { return mLeavesBlock; }
};

class CATALYST_API LeavesDecayAfterEvent final : public ll::event::world::WorldEvent {
    BlockPos       mPos;
    ::Block const& mLeavesBlock;

public:
    constexpr LeavesDecayAfterEvent(BlockSource& blockSource, BlockPos const& pos, ::Block const& leavesBlock)
    : WorldEvent(blockSource),
      mPos(pos),
      mLeavesBlock(leavesBlock) {}

    void serialize(CompoundTag&) const override;

    BlockPos const& pos() const { return mPos; }
    ::Block const&  leavesBlock() const { return mLeavesBlock; }
};

} // namespace Catalyst
