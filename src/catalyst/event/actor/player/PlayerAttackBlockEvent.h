#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/ServerPlayerEvent.h"
#include "mc/world/level/BlockPos.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API PlayerAttackBlockEvent : public ll::event::ServerPlayerEvent {
    BlockPos mPos;
    int      mFace;

public:
    constexpr PlayerAttackBlockEvent(ServerPlayer& player, BlockPos pos, int face)
    : ServerPlayerEvent(player),
      mPos(pos),
      mFace(face) {}

    void serialize(CompoundTag&) const override;

    BlockPos const& pos() const { return mPos; }
    int             face() const { return mFace; }
};

class CATALYST_API PlayerAttackBlockBeforeEvent final : public ll::event::Cancellable<PlayerAttackBlockEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API PlayerAttackBlockAfterEvent final : public PlayerAttackBlockEvent {
public:
    using PlayerAttackBlockEvent::PlayerAttackBlockEvent;
};

} // namespace Catalyst