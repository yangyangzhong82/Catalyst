#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/world/level/BlockPos.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API BlockFallBeforeEvent final : public ll::event::Cancellable<ll::event::world::WorldEvent> {
    BlockPos       mPos;
    ::Block const& mBlock;
    bool           mCreative;

public:
    constexpr BlockFallBeforeEvent(BlockSource& blockSource, BlockPos const& pos, ::Block const& block, bool creative)
    : Cancellable(blockSource),
      mPos(pos),
      mBlock(block),
      mCreative(creative) {}

    BlockPos const& pos() const { return mPos; }
    ::Block const&  block() const { return mBlock; }
    bool            isCreative() const { return mCreative; }
};

class CATALYST_API BlockFallAfterEvent final : public ll::event::world::WorldEvent {
    BlockPos       mPos;
    ::Block const& mBlock;
    bool           mCreative;

public:
    constexpr BlockFallAfterEvent(BlockSource& blockSource, BlockPos const& pos, ::Block const& block, bool creative)
    : WorldEvent(blockSource),
      mPos(pos),
      mBlock(block),
      mCreative(creative) {}

    BlockPos const& pos() const { return mPos; }
    ::Block const&  block() const { return mBlock; }
    bool            isCreative() const { return mCreative; }
};

} // namespace Catalyst
