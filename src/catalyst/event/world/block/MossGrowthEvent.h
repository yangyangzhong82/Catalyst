#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/world/level/BlockPos.h"

#include "catalyst/Macros.h"
// 很奇怪，这个事件刚开始加载时也会被调用

namespace Catalyst {

class CATALYST_API MossGrowthBeforeEvent final : public ll::event::Cancellable<ll::event::world::WorldEvent> {
    BlockPos mOrigin;
    int      mXRadius;
    int      mZRadius;

public:
    constexpr MossGrowthBeforeEvent(BlockSource& blockSource, BlockPos origin, int xRadius, int zRadius)
    : Cancellable(blockSource),
      mOrigin(origin),
      mXRadius(xRadius),
      mZRadius(zRadius) {}

    BlockPos const& origin() const { return mOrigin; }
    int             xRadius() const { return mXRadius; }
    int             zRadius() const { return mZRadius; }
};

class CATALYST_API MossGrowthAfterEvent final : public ll::event::world::WorldEvent {
    BlockPos mOrigin;
    int      mXRadius;
    int      mZRadius;

public:
    constexpr MossGrowthAfterEvent(BlockSource& blockSource, BlockPos origin, int xRadius, int zRadius)
    : WorldEvent(blockSource),
      mOrigin(origin),
      mXRadius(xRadius),
      mZRadius(zRadius) {}

    BlockPos const& origin() const { return mOrigin; }
    int             xRadius() const { return mXRadius; }
    int             zRadius() const { return mZRadius; }
};

} // namespace Catalyst