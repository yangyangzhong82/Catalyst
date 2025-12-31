#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/world/level/BlockPos.h"

#include "catalyst/Macros.h"

namespace Catalyst {

enum class PistonAction : char { Extend = 0, Retract = 1 };

class CATALYST_API BlockPistonBeforeEvent final : public ll::event::Cancellable<ll::event::world::WorldEvent> {
    BlockPos     mPos;
    PistonAction mAction;
    int          mDirection;

public:
    constexpr BlockPistonBeforeEvent(BlockSource& blockSource, BlockPos pos, PistonAction action, int direction)
    : Cancellable(blockSource),
      mPos(pos),
      mAction(action),
      mDirection(direction) {}

    BlockPos const& pos() const { return mPos; }
    PistonAction    action() const { return mAction; }
    int             direction() const { return mDirection; }
};

class CATALYST_API BlockPistonAfterEvent final : public ll::event::world::WorldEvent {
    BlockPos     mPos;
    PistonAction mAction;
    int          mDirection;

public:
    constexpr BlockPistonAfterEvent(BlockSource& blockSource, BlockPos pos, PistonAction action, int direction)
    : WorldEvent(blockSource),
      mPos(pos),
      mAction(action),
      mDirection(direction) {}

    BlockPos const& pos() const { return mPos; }
    PistonAction    action() const { return mAction; }
    int             direction() const { return mDirection; }
};

} // namespace Catalyst