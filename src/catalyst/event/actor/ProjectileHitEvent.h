#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"


#include "catalyst/Macros.h"

class HitResult;
class ProjectileComponent;

namespace Catalyst {

class CATALYST_API ProjectileHitBeforeEvent final : public ll::event::Cancellable<ll::event::entity::ActorEvent> {
    ProjectileComponent& mComponent;
    HitResult const&     mHitResult;

public:
    constexpr ProjectileHitBeforeEvent(Actor& owner, ProjectileComponent& component, HitResult const& hitResult)
    : Cancellable(owner),
      mComponent(component),
      mHitResult(hitResult) {}

    void serialize(CompoundTag&) const override;

    ProjectileComponent& component() const { return mComponent; }
    HitResult const&     hitResult() const { return mHitResult; }
};

class CATALYST_API ProjectileHitAfterEvent final : public ll::event::entity::ActorEvent {
    ProjectileComponent& mComponent;
    HitResult const&     mHitResult;

public:
    constexpr ProjectileHitAfterEvent(Actor& owner, ProjectileComponent& component, HitResult const& hitResult)
    : ActorEvent(owner),
      mComponent(component),
      mHitResult(hitResult) {}

    void serialize(CompoundTag&) const override;

    ProjectileComponent& component() const { return mComponent; }
    HitResult const&     hitResult() const { return mHitResult; }
};

} // namespace Catalyst