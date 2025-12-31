#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/deps/core/math/Vec3.h"
#include "mc/world/level/Explosion.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API ExplosionBeforeEvent final : public ll::event::Cancellable<ll::event::world::WorldEvent> {
    Explosion& mExplosion;

public:
    constexpr ExplosionBeforeEvent(BlockSource& blockSource, Explosion& explosion)
    : Cancellable(blockSource),
      mExplosion(explosion) {}

    Explosion&  explosion() const { return mExplosion; }
    Vec3 const& pos() const { return mExplosion.mPos; }
    float       radius() const { return mExplosion.mRadius; }
    float&      radius() { return mExplosion.mRadius; }
    bool        fire() const { return mExplosion.mFire; }
    bool&       fire() { return mExplosion.mFire; }
    bool        breaking() const { return mExplosion.mBreaking; }
    bool&       breaking() { return mExplosion.mBreaking; }
};

class CATALYST_API ExplosionAfterEvent final : public ll::event::world::WorldEvent {
    Explosion& mExplosion;

public:
    constexpr ExplosionAfterEvent(BlockSource& blockSource, Explosion& explosion)
    : WorldEvent(blockSource),
      mExplosion(explosion) {}

    Explosion&  explosion() const { return mExplosion; }
    Vec3 const& pos() const { return mExplosion.mPos; }
    float       radius() const { return mExplosion.mRadius; }
};

} // namespace Catalyst