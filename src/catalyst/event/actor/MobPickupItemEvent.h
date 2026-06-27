#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"

#include "catalyst/Macros.h"

class ItemActor;

namespace Catalyst {

class CATALYST_API MobPickupItemEvent : public ll::event::entity::ActorEvent {
    ItemActor& mItemActor;

public:
    constexpr MobPickupItemEvent(Actor& mob, ItemActor& itemActor) : ActorEvent(mob), mItemActor(itemActor) {}

    void serialize(CompoundTag&) const override;

    ItemActor& itemActor() const { return mItemActor; }
};

class CATALYST_API MobPickupItemBeforeEvent final : public ll::event::Cancellable<MobPickupItemEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API MobPickupItemAfterEvent final : public MobPickupItemEvent {
public:
    using MobPickupItemEvent::MobPickupItemEvent;
};

} // namespace Catalyst
