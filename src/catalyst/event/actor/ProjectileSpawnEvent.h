#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"

class Vec3;
struct ActorDefinitionIdentifier;
class BlockSource;
#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API ProjectileSpawnBeforeEvent final : public ll::event::Cancellable<ll::event::entity::ActorEvent> {
    BlockSource&                     mRegion;
    ActorDefinitionIdentifier const& mId;
    Vec3 const&                      mPosition;
    Vec3 const&                      mDirection;

public:
    constexpr ProjectileSpawnBeforeEvent(
        Actor&                           spawner,
        BlockSource&                     region,
        ActorDefinitionIdentifier const& id,
        Vec3 const&                      position,
        Vec3 const&                      direction
    )
    : Cancellable(spawner),
      mRegion(region),
      mId(id),
      mPosition(position),
      mDirection(direction) {}

    BlockSource&                     region() const { return mRegion; }
    ActorDefinitionIdentifier const& id() const { return mId; }
    Vec3 const&                      position() const { return mPosition; }
    Vec3 const&                      direction() const { return mDirection; }
};

class CATALYST_API ProjectileSpawnAfterEvent final : public ll::event::entity::ActorEvent {
    BlockSource&                     mRegion;
    ActorDefinitionIdentifier const& mId;
    Vec3 const&                      mPosition;
    Vec3 const&                      mDirection;
    Actor*                           mProjectile;

public:
    constexpr ProjectileSpawnAfterEvent(
        Actor&                           spawner,
        BlockSource&                     region,
        ActorDefinitionIdentifier const& id,
        Vec3 const&                      position,
        Vec3 const&                      direction,
        Actor*                           projectile
    )
    : ActorEvent(spawner),
      mRegion(region),
      mId(id),
      mPosition(position),
      mDirection(direction),
      mProjectile(projectile) {}

    BlockSource&                     region() const { return mRegion; }
    ActorDefinitionIdentifier const& id() const { return mId; }
    Vec3 const&                      position() const { return mPosition; }
    Vec3 const&                      direction() const { return mDirection; }
    Actor*                           projectile() const { return mProjectile; }
};

} // namespace Catalyst
