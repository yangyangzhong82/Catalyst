#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/deps/core/math/Vec3.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/actor/ActorDefinitionIdentifier.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API ProjectileSpawnBeforeEvent final : public ll::event::Cancellable<ll::event::world::WorldEvent> {
    ActorDefinitionIdentifier const& mId;
    Actor*                           mSpawner;
    Vec3 const&                      mPosition;
    Vec3 const&                      mDirection;

public:
    constexpr ProjectileSpawnBeforeEvent(
        BlockSource&                     blockSource,
        ActorDefinitionIdentifier const& id,
        Actor*                           spawner,
        Vec3 const&                      position,
        Vec3 const&                      direction
    )
    : Cancellable(blockSource),
      mId(id),
      mSpawner(spawner),
      mPosition(position),
      mDirection(direction) {}

    ActorDefinitionIdentifier const& id() const { return mId; }
    Actor*                           spawner() const { return mSpawner; }
    Vec3 const&                      position() const { return mPosition; }
    Vec3 const&                      direction() const { return mDirection; }
};

class CATALYST_API ProjectileSpawnAfterEvent final : public ll::event::world::WorldEvent {
    ActorDefinitionIdentifier const& mId;
    Actor*                           mSpawner;
    Vec3 const&                      mPosition;
    Vec3 const&                      mDirection;
    Actor*                           mProjectile;

public:
    constexpr ProjectileSpawnAfterEvent(
        BlockSource&                     blockSource,
        ActorDefinitionIdentifier const& id,
        Actor*                           spawner,
        Vec3 const&                      position,
        Vec3 const&                      direction,
        Actor*                           projectile
    )
    : WorldEvent(blockSource),
      mId(id),
      mSpawner(spawner),
      mPosition(position),
      mDirection(direction),
      mProjectile(projectile) {}

    ActorDefinitionIdentifier const& id() const { return mId; }
    Actor*                           spawner() const { return mSpawner; }
    Vec3 const&                      position() const { return mPosition; }
    Vec3 const&                      direction() const { return mDirection; }
    Actor*                           projectile() const { return mProjectile; }
};

} // namespace Catalyst
