#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/world/level/BlockPos.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API FireBurnBlockEvent : public ll::event::world::WorldEvent {
    BlockPos mBurnPos;  // Position of the block being burned
    BlockPos mFirePos;  // Position of the fire causing the burn
    int      mAge;      // Age of the fire

public:
    constexpr FireBurnBlockEvent(
        BlockSource&      blockSource,
        BlockPos const&   burnPos,
        BlockPos const&   firePos,
        int               age
    )
    : WorldEvent(blockSource),
      mBurnPos(burnPos),
      mFirePos(firePos),
      mAge(age) {}

    void serialize(CompoundTag&) const override;

    BlockPos const& burnPos() const { return mBurnPos; }
    BlockPos const& firePos() const { return mFirePos; }
    int             age() const { return mAge; }
};

// Event fired when fire burns (destroys) a block
class CATALYST_API FireBurnBlockBeforeEvent final : public ll::event::Cancellable<FireBurnBlockEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API FireBurnBlockAfterEvent final : public FireBurnBlockEvent {
public:
    using FireBurnBlockEvent::FireBurnBlockEvent;
};

} // namespace Catalyst
