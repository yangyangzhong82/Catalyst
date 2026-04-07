#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"

#include "catalyst/Macros.h"
#include "mc/world/phys/HitResult.h"

namespace Catalyst {

class CATALYST_API ProjectileHitBeforeEvent final : public ll::event::Cancellable<ll::event::entity::ActorEvent> {
    HitResult const& mHitResult;

public:
    constexpr ProjectileHitBeforeEvent(Actor& projectile, HitResult const& hitResult)
    : Cancellable(projectile),
      mHitResult(hitResult) {}

    void serialize(CompoundTag&) const override;

    HitResult const& hitResult() const { return mHitResult; }
    HitResultType    type() const { return hitResult().mType; }
    uchar            face() const { return hitResult().mFacing; }
    BlockPos const&  blockPos() const { return hitResult().mBlock; }
    Vec3 const&      pos() const { return hitResult().mPos; }
    bool             isHitLiquid() const { return hitResult().mIsHitLiquid; }
    Actor*           hitActor() const { return hitResult().getEntity(); }
};

class CATALYST_API ProjectileHitAfterEvent final : public ll::event::entity::ActorEvent {
    HitResult const& mHitResult;

public:
    constexpr ProjectileHitAfterEvent(Actor& projectile, HitResult const& hitResult)
    : ActorEvent(projectile),
      mHitResult(hitResult) {}

    void serialize(CompoundTag&) const override;

    HitResult const& hitResult() const { return mHitResult; }
    HitResultType    type() const { return hitResult().mType; }
    uchar            face() const { return hitResult().mFacing; }
    BlockPos const&  blockPos() const { return hitResult().mBlock; }
    Vec3 const&      pos() const { return hitResult().mPos; }
    bool             isHitLiquid() const { return hitResult().mIsHitLiquid; }
    Actor*           hitActor() const { return hitResult().getEntity(); }
};

} // namespace Catalyst
