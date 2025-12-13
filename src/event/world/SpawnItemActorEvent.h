#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/deps/core/math/Vec3.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/actor/item/ItemActor.h"
#include "mc/world/item/ItemStack.h"


#include "Macros.h"

namespace Catalyst {

class CATALYST_API SpawnItemActorBeforeEvent final : public ll::event::Cancellable<ll::event::world::WorldEvent> {
    ItemStack const& mItem;
    Actor*           mSpawner;
    Vec3 const&      mPos;
    int              mThrowTime;

public:
    constexpr SpawnItemActorBeforeEvent(
        BlockSource&     blockSource,
        ItemStack const& item,
        Actor*           spawner,
        Vec3 const&      pos,
        int              throwTime
    )
    : Cancellable(blockSource),
      mItem(item),
      mSpawner(spawner),
      mPos(pos),
      mThrowTime(throwTime) {}

    ItemStack const& item() const { return mItem; }
    Actor*           spawner() const { return mSpawner; }
    Vec3 const&      pos() const { return mPos; }
    int              throwTime() const { return mThrowTime; }
};

class CATALYST_API SpawnItemActorAfterEvent final : public ll::event::world::WorldEvent {
    ItemStack const& mItem;
    Actor*           mSpawner;
    Vec3 const&      mPos;
    int              mThrowTime;
    ItemActor*       mItemActor;

public:
    constexpr SpawnItemActorAfterEvent(
        BlockSource&     blockSource,
        ItemStack const& item,
        Actor*           spawner,
        Vec3 const&      pos,
        int              throwTime,
        ItemActor*       itemActor
    )
    : WorldEvent(blockSource),
      mItem(item),
      mSpawner(spawner),
      mPos(pos),
      mThrowTime(throwTime),
      mItemActor(itemActor) {}

    ItemStack const& item() const { return mItem; }
    Actor*           spawner() const { return mSpawner; }
    Vec3 const&      pos() const { return mPos; }
    int              throwTime() const { return mThrowTime; }
    ItemActor*       itemActor() const { return mItemActor; }
};

} // namespace Catalyst