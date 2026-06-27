#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/level/BlockPos.h"


#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API FarmChangeEvent : public ll::event::world::WorldEvent {
    BlockPos mPos;
    Actor*   mActor;
    bool     mToFarmland;

public:
    constexpr FarmChangeEvent(BlockSource& blockSource, BlockPos pos, Actor* actor, bool toFarmland)
    : WorldEvent(blockSource),
      mPos(pos),
      mActor(actor),
      mToFarmland(toFarmland) {}

    void serialize(CompoundTag&) const override;

    BlockPos const& pos() const { return mPos; }
    Actor*          actor() const { return mActor; }
    bool            toFarmland() const { return mToFarmland; }
};

class CATALYST_API FarmChangeBeforeEvent final : public ll::event::Cancellable<FarmChangeEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API FarmChangeAfterEvent final : public FarmChangeEvent {
public:
    using FarmChangeEvent::FarmChangeEvent;
};

} // namespace Catalyst