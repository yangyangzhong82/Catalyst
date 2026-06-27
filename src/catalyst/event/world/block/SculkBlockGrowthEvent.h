#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"


#include "catalyst/Macros.h"
class BlockPos;

namespace Catalyst {

class CATALYST_API SculkBlockGrowthEvent : public ll::event::world::WorldEvent {
    BlockPos mPos;

public:
    constexpr SculkBlockGrowthEvent(BlockSource& blockSource, BlockPos pos) : WorldEvent(blockSource), mPos(pos) {}

    void serialize(CompoundTag&) const override;

    BlockPos const& pos() const { return mPos; }
};

class CATALYST_API SculkBlockGrowthBeforeEvent final : public ll::event::Cancellable<SculkBlockGrowthEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API SculkBlockGrowthAfterEvent final : public SculkBlockGrowthEvent {
public:
    using SculkBlockGrowthEvent::SculkBlockGrowthEvent;
};

} // namespace Catalyst