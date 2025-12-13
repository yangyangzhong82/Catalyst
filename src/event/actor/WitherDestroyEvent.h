#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/MobEvent.h"
#include "mc/world/actor/boss/WitherBoss.h"
#include "mc/world/phys/AABB.h"

#include "Macros.h"

namespace Catalyst {

class CATALYST_API WitherDestroyBlocksBeforeEvent final : public ll::event::Cancellable<ll::event::entity::MobEvent> {
    AABB const&                  mAABB;
    BlockSource&                 mRegion;
    int                          mRange;
    WitherBoss::WitherAttackType mAttackType;

public:
    constexpr WitherDestroyBlocksBeforeEvent(
        WitherBoss&                  wither,
        AABB const&                  aabb,
        BlockSource&                 region,
        int                          range,
        WitherBoss::WitherAttackType attackType
    )
    : Cancellable(wither),
      mAABB(aabb),
      mRegion(region),
      mRange(range),
      mAttackType(attackType) {}

    AABB const&                  aabb() const { return mAABB; }
    BlockSource&                 region() const { return mRegion; }
    int                          range() const { return mRange; }
    WitherBoss::WitherAttackType attackType() const { return mAttackType; }
};

class CATALYST_API WitherDestroyBlocksAfterEvent final : public ll::event::entity::MobEvent {
    AABB const&                  mAABB;
    BlockSource&                 mRegion;
    int                          mRange;
    WitherBoss::WitherAttackType mAttackType;

public:
    constexpr WitherDestroyBlocksAfterEvent(
        WitherBoss&                  wither,
        AABB const&                  aabb,
        BlockSource&                 region,
        int                          range,
        WitherBoss::WitherAttackType attackType
    )
    : MobEvent(wither),
      mAABB(aabb),
      mRegion(region),
      mRange(range),
      mAttackType(attackType) {}

    AABB const&                  aabb() const { return mAABB; }
    BlockSource&                 region() const { return mRegion; }
    int                          range() const { return mRange; }
    WitherBoss::WitherAttackType attackType() const { return mAttackType; }
};

} // namespace Catalyst