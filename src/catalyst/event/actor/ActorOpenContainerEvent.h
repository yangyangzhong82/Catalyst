#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"

#include "catalyst/Macros.h"

class Actor;
class BlockActor;

namespace Catalyst {

enum class ContainerType { Hopper, Barrel, Chest, EnderChest, Furnace, ShulkerBox, BrewingStand, Unknown };

class CATALYST_API ActorOpenContainerEvent : public ll::event::ActorEvent {
    ContainerType mContainerType;
    BlockActor*   mBlockActor;

public:
    constexpr ActorOpenContainerEvent(Actor& actor, ContainerType containerType, BlockActor* blockActor)
    : ActorEvent(actor),
      mContainerType(containerType),
      mBlockActor(blockActor) {}

    void serialize(CompoundTag&) const override;

    ContainerType containerType() const { return mContainerType; }
    BlockActor*   blockActor() const { return mBlockActor; }
};

class CATALYST_API ActorOpenContainerBeforeEvent final : public ll::event::Cancellable<ActorOpenContainerEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API ActorOpenContainerAfterEvent final : public ActorOpenContainerEvent {
public:
    using ActorOpenContainerEvent::ActorOpenContainerEvent;
};

} // namespace Catalyst
