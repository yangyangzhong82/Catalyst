#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/ServerPlayerEvent.h"
#include "mc/world/level/BlockPos.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API PlayerAttackBlockBeforeEvent final : public ll::event::Cancellable<ll::event::ServerPlayerEvent> {
    BlockPos mPos;
    int      mFace;

public:
    constexpr PlayerAttackBlockBeforeEvent(ServerPlayer& player, BlockPos pos, int face)
    : Cancellable(player),
      mPos(pos),
      mFace(face) {}

    BlockPos const& pos() const { return mPos; }
    int             face() const { return mFace; }
};

class CATALYST_API PlayerAttackBlockAfterEvent final : public ll::event::ServerPlayerEvent {
    BlockPos mPos;
    int      mFace;

public:
    constexpr PlayerAttackBlockAfterEvent(ServerPlayer& player, BlockPos pos, int face)
    : ServerPlayerEvent(player),
      mPos(pos),
      mFace(face) {}

    BlockPos const& pos() const { return mPos; }
    int             face() const { return mFace; }
};

} // namespace Catalyst