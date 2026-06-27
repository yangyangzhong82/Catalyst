#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"

#include "mc/world/level/Explosion.h"

#include "catalyst/Macros.h"
class Vec3;
namespace Catalyst {

class CATALYST_API ExplosionEvent : public ll::event::world::WorldEvent {
    Explosion& mExplosion;

public:
    constexpr ExplosionEvent(BlockSource& blockSource, Explosion& explosion)
    : WorldEvent(blockSource),
      mExplosion(explosion) {}

    void serialize(CompoundTag&) const override;

    Explosion&  explosion() const { return mExplosion; }
    Vec3 const& pos() const { return mExplosion.mPos; }
    float       radius() const { return mExplosion.mRadius; }
    float&      radius() { return mExplosion.mRadius; }
    bool        fire() const { return mExplosion.mFire; }
    bool&       fire() { return mExplosion.mFire; }
    bool        breaking() const { return mExplosion.mBreaking; }
    bool&       breaking() { return mExplosion.mBreaking; }
};

class CATALYST_API ExplosionBeforeEvent final : public ll::event::Cancellable<ExplosionEvent> {
public:
    using Cancellable::Cancellable;

    void serialize(CompoundTag&) const override;
};

class CATALYST_API ExplosionAfterEvent final : public ExplosionEvent {
public:
    using ExplosionEvent::ExplosionEvent;
};

} // namespace Catalyst