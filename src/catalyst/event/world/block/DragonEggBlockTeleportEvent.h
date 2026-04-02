#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"


#include "catalyst/Macros.h"
class BlockPos;
namespace Catalyst {

class CATALYST_API DragonEggBlockTeleportBeforeEvent final
: public ll::event::Cancellable<ll::event::world::WorldEvent> {
    BlockPos const& mOldPos;
    BlockPos        mNewPos;

public:
    DragonEggBlockTeleportBeforeEvent(BlockSource& region, BlockPos const& oldPos, BlockPos const& newPos)
    : Cancellable(region),
      mOldPos(oldPos),
      mNewPos(newPos) {}

    void serialize(CompoundTag&) const override;

    BlockPos const& getOldPos() const { return mOldPos; }
    BlockPos const& getNewPos() const { return mNewPos; }
    void            setNewPos(BlockPos const& pos) { mNewPos = pos; }
};

class CATALYST_API DragonEggBlockTeleportAfterEvent final : public ll::event::world::WorldEvent {
    BlockPos const& mOldPos;
    BlockPos const& mNewPos;

public:
    constexpr DragonEggBlockTeleportAfterEvent(BlockSource& region, BlockPos const& oldPos, BlockPos const& newPos)
    : WorldEvent(region),
      mOldPos(oldPos),
      mNewPos(newPos) {}

    void serialize(CompoundTag&) const override;

    BlockPos const& getOldPos() const { return mOldPos; }
    BlockPos const& getNewPos() const { return mNewPos; }
};

} // namespace Catalyst
