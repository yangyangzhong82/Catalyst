#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/world/level/BlockPos.h"

#include "Macros.h"

namespace Catalyst {

class CATALYST_API NetherPortalCreateBeforeEvent final : public ll::event::Cancellable<ll::event::world::WorldEvent> {
    BlockPos mPos;
    int      mRadius;

public:
    constexpr NetherPortalCreateBeforeEvent(BlockSource& blockSource, BlockPos pos, int radius)
    : Cancellable(blockSource),
      mPos(pos),
      mRadius(radius) {}

    BlockPos const& pos() const { return mPos; }
    int             radius() const { return mRadius; }
};

class CATALYST_API NetherPortalCreateAfterEvent final : public ll::event::world::WorldEvent {
    BlockPos mPos;
    int      mRadius;

public:
    constexpr NetherPortalCreateAfterEvent(BlockSource& blockSource, BlockPos pos, int radius)
    : WorldEvent(blockSource),
      mPos(pos),
      mRadius(radius) {}

    BlockPos const& pos() const { return mPos; }
    int             radius() const { return mRadius; }
};

} // namespace Catalyst