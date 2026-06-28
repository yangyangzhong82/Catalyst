#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"
#include "mc/world/level/BlockPos.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API PlayerUseFrameBlockEvent : public ll::event::PlayerEvent {
    BlockPos mPos;

public:
    constexpr PlayerUseFrameBlockEvent(Player& player, BlockPos pos) : PlayerEvent(player), mPos(pos) {}

    void serialize(CompoundTag&) const override;

    BlockPos const& pos() const { return mPos; }
};

class CATALYST_API PlayerUseFrameBlockBeforeEvent final : public ll::event::Cancellable<PlayerUseFrameBlockEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API PlayerUseFrameBlockAfterEvent final : public PlayerUseFrameBlockEvent {
public:
    using PlayerUseFrameBlockEvent::PlayerUseFrameBlockEvent;
};

} // namespace Catalyst