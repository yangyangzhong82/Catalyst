#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"


#include "catalyst/Macros.h"
// 很奇怪，这个事件刚开始加载时也会被调用
class BlockPos;

namespace Catalyst {

class CATALYST_API MossGrowthEvent : public ll::event::world::WorldEvent {
    BlockPos mOrigin;
    int      mXRadius;
    int      mZRadius;

public:
    constexpr MossGrowthEvent(BlockSource& blockSource, BlockPos origin, int xRadius, int zRadius)
    : WorldEvent(blockSource),
      mOrigin(origin),
      mXRadius(xRadius),
      mZRadius(zRadius) {}

    void serialize(CompoundTag&) const override;

    BlockPos const& origin() const { return mOrigin; }
    int             xRadius() const { return mXRadius; }
    int             zRadius() const { return mZRadius; }
};

class CATALYST_API MossGrowthBeforeEvent final : public ll::event::Cancellable<MossGrowthEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API MossGrowthAfterEvent final : public MossGrowthEvent {
public:
    using MossGrowthEvent::MossGrowthEvent;
};

} // namespace Catalyst