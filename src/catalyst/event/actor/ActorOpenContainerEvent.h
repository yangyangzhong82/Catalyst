#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"

#include "catalyst/Macros.h"

class Actor;
class BlockActor;

namespace Catalyst {

enum class ContainerType { Hopper, Barrel, Chest, EnderChest, Furnace, ShulkerBox, BrewingStand, Unknown };

class CATALYST_API ActorOpenContainerBeforeEvent final : public ll::event::Cancellable<ll::event::ActorEvent> {
    ContainerType mContainerType;
    BlockActor*   mBlockActor;

public:
    constexpr ActorOpenContainerBeforeEvent(Actor& actor, ContainerType containerType, BlockActor* blockActor)
    : Cancellable(actor),
      mContainerType(containerType),
      mBlockActor(blockActor) {}

    void serialize(CompoundTag&) const override;

    ContainerType containerType() const { return mContainerType; }
    BlockActor*   blockActor() const { return mBlockActor; }
};

class CATALYST_API ActorOpenContainerAfterEvent final : public ll::event::ActorEvent {
    ContainerType mContainerType;
    BlockActor*   mBlockActor;

public:
    constexpr ActorOpenContainerAfterEvent(Actor& actor, ContainerType containerType, BlockActor* blockActor)
    : ActorEvent(actor),
      mContainerType(containerType),
      mBlockActor(blockActor) {}

    void serialize(CompoundTag&) const override;

    ContainerType containerType() const { return mContainerType; }
    BlockActor*   blockActor() const { return mBlockActor; }
};

} // namespace Catalyst
