#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/world/level/BlockPos.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API SculkBlockGrowthBeforeEvent final : public ll::event::Cancellable<ll::event::world::WorldEvent> {
    BlockPos mPos;

public:
    constexpr SculkBlockGrowthBeforeEvent(BlockSource& blockSource, BlockPos pos)
    : Cancellable(blockSource),
      mPos(pos) {}

    BlockPos const& pos() const { return mPos; }
};

class CATALYST_API SculkBlockGrowthAfterEvent final : public ll::event::world::WorldEvent {
    BlockPos mPos;

public:
    constexpr SculkBlockGrowthAfterEvent(BlockSource& blockSource, BlockPos pos) : WorldEvent(blockSource), mPos(pos) {}

    BlockPos const& pos() const { return mPos; }
};

} // namespace Catalyst