#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"

#include "catalyst/Macros.h"

class BasePressurePlateBlock;
class BlockSource;
class BlockPos;
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