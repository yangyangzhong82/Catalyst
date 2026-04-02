#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"

#include "catalyst/Macros.h"

class ItemActor;

namespace Catalyst {

class CATALYST_API MobPickupItemBeforeEvent final : public ll::event::Cancellable<ll::event::entity::ActorEvent> {
    ItemActor& mItemActor;

public:
    constexpr MobPickupItemBeforeEvent(Actor& mob, ItemActor& itemActor) : Cancellable(mob), mItemActor(itemActor) {}

    void serialize(CompoundTag&) const override;

    ItemActor& itemActor() const { return mItemActor; }
};

class CATALYST_API MobPickupItemAfterEvent final : public ll::event::entity::ActorEvent {
    ItemActor& mItemActor;

public:
    constexpr MobPickupItemAfterEvent(Actor& mob, ItemActor& itemActor) : ActorEvent(mob), mItemActor(itemActor) {}

    void serialize(CompoundTag&) const override;

    ItemActor& itemActor() const { return mItemActor; }
};

} // namespace Catalyst
