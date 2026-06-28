#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/MobEvent.h"



#include "catalyst/Macros.h"

class Block;
class BlockPos;


namespace Catalyst {

class CATALYST_API MobPlaceBlockEvent : public ll::event::entity::MobEvent {
    BlockPos     mPos;
    Block const& mBlock;

public:
    MobPlaceBlockEvent(Mob& mob, BlockPos const& pos, Block const& block)
    : MobEvent(mob),
      mPos(pos),
      mBlock(block) {}

    void serialize(CompoundTag&) const override;

    BlockPos const& pos() const { return mPos; }
    Block const&    block() const { return mBlock; }
};

class CATALYST_API MobPlaceBlockBeforeEvent final : public ll::event::Cancellable<MobPlaceBlockEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API MobPlaceBlockAfterEvent final : public MobPlaceBlockEvent {
public:
    using MobPlaceBlockEvent::MobPlaceBlockEvent;
};

} // namespace Catalyst
