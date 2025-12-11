#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/world/level/BlockPos.h"

#include "Macros.h"

namespace Catalyst {

class CATALYST_API FarmChangeBeforeEvent final : public ll::event::Cancellable<ll::event::world::WorldEvent> {
    BlockPos mPos;
    bool     mToFarmland;

public:
    constexpr FarmChangeBeforeEvent(BlockSource& blockSource, BlockPos pos, bool toFarmland)
    : Cancellable(blockSource),
      mPos(pos),
      mToFarmland(toFarmland) {}

    BlockPos const& pos() const { return mPos; }
    bool            toFarmland() const { return mToFarmland; }
};

class CATALYST_API FarmChangeAfterEvent final : public ll::event::world::WorldEvent {
    BlockPos mPos;
    bool     mToFarmland;

public:
    constexpr FarmChangeAfterEvent(BlockSource& blockSource, BlockPos pos, bool toFarmland)
    : WorldEvent(blockSource),
      mPos(pos),
      mToFarmland(toFarmland) {}

    BlockPos const& pos() const { return mPos; }
    bool            toFarmland() const { return mToFarmland; }
};

} // namespace Catalyst