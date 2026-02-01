#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/level/BlockPos.h"


#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API FarmChangeBeforeEvent final : public ll::event::Cancellable<ll::event::world::WorldEvent> {
    BlockPos mPos;
    Actor*   mActor;
    bool     mToFarmland;

public:
    constexpr FarmChangeBeforeEvent(BlockSource& blockSource, BlockPos pos, Actor* actor, bool toFarmland)
    : Cancellable(blockSource),
      mPos(pos),
      mActor(actor),
      mToFarmland(toFarmland) {}

    BlockPos const& pos() const { return mPos; }
    Actor*          actor() const { return mActor; }
    bool            toFarmland() const { return mToFarmland; }
};

class CATALYST_API FarmChangeAfterEvent final : public ll::event::world::WorldEvent {
    BlockPos mPos;
    Actor*   mActor;
    bool     mToFarmland;

public:
    constexpr FarmChangeAfterEvent(BlockSource& blockSource, BlockPos pos, Actor* actor, bool toFarmland)
    : WorldEvent(blockSource),
      mPos(pos),
      mActor(actor),
      mToFarmland(toFarmland) {}

    BlockPos const& pos() const { return mPos; }
    Actor*          actor() const { return mActor; }
    bool            toFarmland() const { return mToFarmland; }
};

} // namespace Catalyst