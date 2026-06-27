#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"

class BlockPos;

#include "catalyst/Macros.h"

namespace Catalyst {

enum class PistonAction : char { Extend = 0, Retract = 1 };

class CATALYST_API BlockPistonEvent : public ll::event::world::WorldEvent {
    BlockPos     mPos;
    PistonAction mAction;
    int          mDirection;

public:
    constexpr BlockPistonEvent(BlockSource& blockSource, BlockPos pos, PistonAction action, int direction)
    : WorldEvent(blockSource),
      mPos(pos),
      mAction(action),
      mDirection(direction) {}

    void serialize(CompoundTag&) const override;

    BlockPos const& pos() const { return mPos; }
    PistonAction    action() const { return mAction; }
    int             direction() const { return mDirection; }
};

class CATALYST_API BlockPistonBeforeEvent final : public ll::event::Cancellable<BlockPistonEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API BlockPistonAfterEvent final : public BlockPistonEvent {
public:
    using BlockPistonEvent::BlockPistonEvent;
};

} // namespace Catalyst