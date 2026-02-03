#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/world/level/BlockPos.h"

#include "catalyst/Macros.h"

namespace Catalyst {

// Event fired when fire tries to spread to a new position
class CATALYST_API FireSpreadBeforeEvent final : public ll::event::Cancellable<ll::event::world::WorldEvent> {
    BlockPos mSpreadPos;  // Position where fire is trying to spread to
    BlockPos mFirePos;    // Position of the source fire
    int      mNewAge;     // Age of the new fire block
    int      mSourceAge;  // Age of the source fire

public:
    constexpr FireSpreadBeforeEvent(
        BlockSource&      blockSource,
        BlockPos const&   spreadPos,
        BlockPos const&   firePos,
        int               newAge,
        int               sourceAge
    )
    : Cancellable(blockSource),
      mSpreadPos(spreadPos),
      mFirePos(firePos),
      mNewAge(newAge),
      mSourceAge(sourceAge) {}

    BlockPos const& spreadPos() const { return mSpreadPos; }
    BlockPos const& firePos() const { return mFirePos; }
    int             newAge() const { return mNewAge; }
    int             sourceAge() const { return mSourceAge; }
};

class CATALYST_API FireSpreadAfterEvent final : public ll::event::world::WorldEvent {
    BlockPos mSpreadPos;
    BlockPos mFirePos;
    int      mNewAge;
    int      mSourceAge;

public:
    constexpr FireSpreadAfterEvent(
        BlockSource&      blockSource,
        BlockPos const&   spreadPos,
        BlockPos const&   firePos,
        int               newAge,
        int               sourceAge
    )
    : WorldEvent(blockSource),
      mSpreadPos(spreadPos),
      mFirePos(firePos),
      mNewAge(newAge),
      mSourceAge(sourceAge) {}

    BlockPos const& spreadPos() const { return mSpreadPos; }
    BlockPos const& firePos() const { return mFirePos; }
    int             newAge() const { return mNewAge; }
    int             sourceAge() const { return mSourceAge; }
};

} // namespace Catalyst
