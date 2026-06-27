#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/world/level/BlockPos.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API BlockFallEvent : public ll::event::world::WorldEvent {
    BlockPos       mPos;
    ::Block const& mBlock;
    bool           mCreative;

public:
    constexpr BlockFallEvent(BlockSource& blockSource, BlockPos const& pos, ::Block const& block, bool creative)
    : WorldEvent(blockSource),
      mPos(pos),
      mBlock(block),
      mCreative(creative) {}

    void serialize(CompoundTag&) const override;

    BlockPos const& pos() const { return mPos; }
    ::Block const&  block() const { return mBlock; }
    bool            isCreative() const { return mCreative; }
};

class CATALYST_API BlockFallBeforeEvent final : public ll::event::Cancellable<BlockFallEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API BlockFallAfterEvent final : public BlockFallEvent {
public:
    using BlockFallEvent::BlockFallEvent;
};

} // namespace Catalyst
