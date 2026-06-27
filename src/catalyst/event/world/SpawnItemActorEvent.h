#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"


class Actor;
class Vec3;
class ItemStack;
class ItemActor;

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API SpawnItemActorEvent : public ll::event::world::WorldEvent {
    ItemStack const& mItem;
    Actor*           mSpawner;
    Vec3 const&      mPos;
    int              mThrowTime;

public:
    constexpr SpawnItemActorEvent(
        BlockSource&     blockSource,
        ItemStack const& item,
        Actor*           spawner,
        Vec3 const&      pos,
        int              throwTime
    )
    : WorldEvent(blockSource),
      mItem(item),
      mSpawner(spawner),
      mPos(pos),
      mThrowTime(throwTime) {}

    void serialize(CompoundTag&) const override;

    ItemStack const& item() const { return mItem; }
    Actor*           spawner() const { return mSpawner; }
    Vec3 const&      pos() const { return mPos; }
    int              throwTime() const { return mThrowTime; }
};

class CATALYST_API SpawnItemActorBeforeEvent final : public ll::event::Cancellable<SpawnItemActorEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API SpawnItemActorAfterEvent final : public SpawnItemActorEvent {
    ItemActor* mItemActor;

public:
    constexpr SpawnItemActorAfterEvent(
        BlockSource&     blockSource,
        ItemStack const& item,
        Actor*           spawner,
        Vec3 const&      pos,
        int              throwTime,
        ItemActor*       itemActor
    )
    : SpawnItemActorEvent(blockSource, item, spawner, pos, throwTime),
      mItemActor(itemActor) {}

    void serialize(CompoundTag&) const override;

    ItemActor* itemActor() const { return mItemActor; }
};

} // namespace Catalyst