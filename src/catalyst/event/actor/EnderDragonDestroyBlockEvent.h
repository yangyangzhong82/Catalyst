#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/MobEvent.h"
#include "mc/world/actor/monster/EnderDragon.h"

#include "catalyst/Macros.h"

class Block;
class BlockPos;

namespace Catalyst {

class CATALYST_API EnderDragonDestroyBlockEvent : public ll::event::entity::MobEvent {
    BlockPos     mPos;
    Block const& mBlock;

public:
    EnderDragonDestroyBlockEvent(EnderDragon& dragon, BlockPos const& pos, Block const& block)
    : MobEvent(dragon),
      mPos(pos),
      mBlock(block) {}

    void serialize(CompoundTag&) const override;

    BlockPos const& pos() const { return mPos; }
    Block const&    block() const { return mBlock; }
};

class CATALYST_API EnderDragonDestroyBlockBeforeEvent final
: public ll::event::Cancellable<EnderDragonDestroyBlockEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API EnderDragonDestroyBlockAfterEvent final : public EnderDragonDestroyBlockEvent {
    bool mDestroyed;

public:
    EnderDragonDestroyBlockAfterEvent(EnderDragon& dragon, BlockPos const& pos, Block const& block, bool destroyed)
    : EnderDragonDestroyBlockEvent(dragon, pos, block),
      mDestroyed(destroyed) {}

    void serialize(CompoundTag&) const override;

    bool destroyed() const { return mDestroyed; }
};

} // namespace Catalyst

