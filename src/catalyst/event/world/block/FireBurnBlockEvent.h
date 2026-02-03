#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/world/level/BlockPos.h"

#include "catalyst/Macros.h"

namespace Catalyst {

// Event fired when fire burns (destroys) a block
class CATALYST_API FireBurnBlockBeforeEvent final : public ll::event::Cancellable<ll::event::world::WorldEvent> {
    BlockPos mBurnPos;  // Position of the block being burned
    BlockPos mFirePos;  // Position of the fire causing the burn
    int      mAge;      // Age of the fire

public:
    constexpr FireBurnBlockBeforeEvent(
        BlockSource&      blockSource,
        BlockPos const&   burnPos,
        BlockPos const&   firePos,
        int               age
    )
    : Cancellable(blockSource),
      mBurnPos(burnPos),
      mFirePos(firePos),
      mAge(age) {}

    BlockPos const& burnPos() const { return mBurnPos; }
    BlockPos const& firePos() const { return mFirePos; }
    int             age() const { return mAge; }
};

class CATALYST_API FireBurnBlockAfterEvent final : public ll::event::world::WorldEvent {
    BlockPos mBurnPos;
    BlockPos mFirePos;
    int      mAge;

public:
    constexpr FireBurnBlockAfterEvent(
        BlockSource&      blockSource,
        BlockPos const&   burnPos,
        BlockPos const&   firePos,
        int               age
    )
    : WorldEvent(blockSource),
      mBurnPos(burnPos),
      mFirePos(firePos),
      mAge(age) {}

    BlockPos const& burnPos() const { return mBurnPos; }
    BlockPos const& firePos() const { return mFirePos; }
    int             age() const { return mAge; }
};

} // namespace Catalyst
