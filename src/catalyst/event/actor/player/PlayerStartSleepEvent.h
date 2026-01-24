#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"
#include "mc/world/actor/player/BedSleepingResult.h"
#include "mc/world/level/BlockPos.h"


#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API PlayerStartSleepBeforeEvent final : public ll::event::Cancellable<ll::event::PlayerEvent> {
    BlockPos mBedBlockPos;

public:
    constexpr PlayerStartSleepBeforeEvent(Player& player, BlockPos const& bedBlockPos)
    : Cancellable(player),
      mBedBlockPos(bedBlockPos) {}

    BlockPos const& getBedBlockPos() const { return mBedBlockPos; }
};

class CATALYST_API PlayerStartSleepAfterEvent final : public ll::event::PlayerEvent {
    BlockPos          mBedBlockPos;
    BedSleepingResult mResult;

public:
    constexpr PlayerStartSleepAfterEvent(Player& player, BlockPos const& bedBlockPos, BedSleepingResult result)
    : PlayerEvent(player),
      mBedBlockPos(bedBlockPos),
      mResult(result) {}

    BlockPos const&   getBedBlockPos() const { return mBedBlockPos; }
    BedSleepingResult getResult() const { return mResult; }
};

} // namespace Catalyst
