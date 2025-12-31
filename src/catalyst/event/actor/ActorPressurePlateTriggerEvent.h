#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/block/BasePressurePlateBlock.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API ActorPressurePlateTriggerBeforeEvent final
: public ll::event::Cancellable<ll::event::entity::ActorEvent> {
    BasePressurePlateBlock const& mBlock;
    BlockSource&                  mRegion;
    BlockPos const&               mPos;

public:
    constexpr ActorPressurePlateTriggerBeforeEvent(
        Actor&                        actor,
        BasePressurePlateBlock const& block,
        BlockSource&                  region,
        BlockPos const&               pos
    )
    : Cancellable(actor),
      mBlock(block),
      mRegion(region),
      mPos(pos) {}

    BasePressurePlateBlock const& block() const { return mBlock; }
    BlockSource&                  region() const { return mRegion; }
    BlockPos const&               pos() const { return mPos; }
};

} // namespace Catalyst