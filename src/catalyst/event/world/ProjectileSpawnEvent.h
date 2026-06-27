#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"

#include "catalyst/Macros.h"
class Actor;
class Vec3;
struct ActorDefinitionIdentifier;
namespace Catalyst {

class CATALYST_API ProjectileSpawnEvent : public ll::event::world::WorldEvent {
    ActorDefinitionIdentifier const& mId;
    Actor*                           mSpawner;
    Vec3 const&                      mPosition;
    Vec3 const&                      mDirection;

public:
    constexpr ProjectileSpawnEvent(
        BlockSource&                     blockSource,
        ActorDefinitionIdentifier const& id,
        Actor*                           spawner,
        Vec3 const&                      position,
        Vec3 const&                      direction
    )
    : WorldEvent(blockSource),
      mId(id),
      mSpawner(spawner),
      mPosition(position),
      mDirection(direction) {}

    void serialize(CompoundTag&) const override;

    ActorDefinitionIdentifier const& id() const { return mId; }
    Actor*                           spawner() const { return mSpawner; }
    Vec3 const&                      position() const { return mPosition; }
    Vec3 const&                      direction() const { return mDirection; }
};

class CATALYST_API ProjectileSpawnBeforeEvent final : public ll::event::Cancellable<ProjectileSpawnEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API ProjectileSpawnAfterEvent final : public ProjectileSpawnEvent {
    Actor* mProjectile;

public:
    constexpr ProjectileSpawnAfterEvent(
        BlockSource&                     blockSource,
        ActorDefinitionIdentifier const& id,
        Actor*                           spawner,
        Vec3 const&                      position,
        Vec3 const&                      direction,
        Actor*                           projectile
    )
    : ProjectileSpawnEvent(blockSource, id, spawner, position, direction),
      mProjectile(projectile) {}

    void serialize(CompoundTag&) const override;

    Actor* projectile() const { return mProjectile; }
};

} // namespace Catalyst
