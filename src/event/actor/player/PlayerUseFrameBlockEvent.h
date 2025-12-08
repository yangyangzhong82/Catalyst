#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"
#include "mc/world/level/BlockPos.h"

#include "Macros.h"

namespace Catalyst {

class CATALYST_API PlayerUseFrameBlockBeforeEvent final : public ll::event::Cancellable<ll::event::PlayerEvent> {
    BlockPos mPos;

public:
    constexpr PlayerUseFrameBlockBeforeEvent(Player& player, BlockPos pos) : Cancellable(player), mPos(pos) {}

    BlockPos const& pos() const { return mPos; }
};

class CATALYST_API PlayerUseFrameBlockAfterEvent final : public ll::event::PlayerEvent {
    BlockPos mPos;

public:
    constexpr PlayerUseFrameBlockAfterEvent(Player& player, BlockPos pos) : PlayerEvent(player), mPos(pos) {}

    BlockPos const& pos() const { return mPos; }
};

} // namespace Catalyst