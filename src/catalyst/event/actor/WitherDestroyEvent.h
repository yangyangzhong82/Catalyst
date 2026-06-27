#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/MobEvent.h"
#include "mc/world/actor/boss/WitherBoss.h"
#include "mc/world/phys/AABB.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API WitherDestroyBlocksEvent : public ll::event::entity::MobEvent {
    AABB const&                  mAABB;
    BlockSource&                 mRegion;
    int                          mRange;
    WitherBoss::WitherAttackType mAttackType;

public:
    constexpr WitherDestroyBlocksEvent(
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

    void serialize(CompoundTag&) const override;

    AABB const&                  aabb() const { return mAABB; }
    BlockSource&                 region() const { return mRegion; }
    int                          range() const { return mRange; }
    WitherBoss::WitherAttackType attackType() const { return mAttackType; }
};

class CATALYST_API WitherDestroyBlocksBeforeEvent final : public ll::event::Cancellable<WitherDestroyBlocksEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API WitherDestroyBlocksAfterEvent final : public WitherDestroyBlocksEvent {
public:
    using WitherDestroyBlocksEvent::WitherDestroyBlocksEvent;
};

} // namespace Catalyst