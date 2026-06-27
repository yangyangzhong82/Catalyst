#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"
#include "mc/world/actor/player/BedSleepingResult.h"
#include "mc/world/level/BlockPos.h"


#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API PlayerStartSleepEvent : public ll::event::PlayerEvent {
    BlockPos mBedBlockPos;

public:
    constexpr PlayerStartSleepEvent(Player& player, BlockPos const& bedBlockPos)
    : PlayerEvent(player),
      mBedBlockPos(bedBlockPos) {}

    void serialize(CompoundTag&) const override;

    BlockPos const& getBedBlockPos() const { return mBedBlockPos; }
};

class CATALYST_API PlayerStartSleepBeforeEvent final : public ll::event::Cancellable<PlayerStartSleepEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API PlayerStartSleepAfterEvent final : public PlayerStartSleepEvent {
    BedSleepingResult mResult;

public:
    constexpr PlayerStartSleepAfterEvent(Player& player, BlockPos const& bedBlockPos, BedSleepingResult result)
    : PlayerStartSleepEvent(player, bedBlockPos),
      mResult(result) {}

    void serialize(CompoundTag&) const override;

    BedSleepingResult getResult() const { return mResult; }
};

} // namespace Catalyst
