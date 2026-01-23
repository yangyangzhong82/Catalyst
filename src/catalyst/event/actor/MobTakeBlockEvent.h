#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/MobEvent.h"

#include "catalyst/Macros.h"

class Block;
class BlockPos;

namespace Catalyst {

class CATALYST_API MobTakeBlockBeforeEvent final : public ll::event::Cancellable<ll::event::entity::MobEvent> {
    BlockPos     mPos;
    Block const& mBlock;

public:
    MobTakeBlockBeforeEvent(Mob& mob, BlockPos const& pos, Block const& block)
    : Cancellable(mob),
      mPos(pos),
      mBlock(block) {}

    BlockPos const& pos() const { return mPos; }
    Block const&    block() const { return mBlock; }
};

class CATALYST_API MobTakeBlockAfterEvent final : public ll::event::entity::MobEvent {
    BlockPos     mPos;
    Block const& mBlock;

public:
    MobTakeBlockAfterEvent(Mob& mob, BlockPos const& pos, Block const& block)
    : MobEvent(mob),
      mPos(pos),
      mBlock(block) {}

    BlockPos const& pos() const { return mPos; }
    Block const&    block() const { return mBlock; }
};

} // namespace Catalyst
