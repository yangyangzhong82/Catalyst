#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"
#include "mc/entity/components_json_legacy/ProjectileComponent.h"
#include "mc/world/phys/HitResult.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API ProjectileHitBeforeEvent final : public ll::event::Cancellable<ll::event::entity::ActorEvent> {
    ProjectileComponent& mComponent;
    HitResult const&     mHitResult;

public:
    constexpr ProjectileHitBeforeEvent(Actor& owner, ProjectileComponent& component, HitResult const& hitResult)
    : Cancellable(owner),
      mComponent(component),
      mHitResult(hitResult) {}

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

    ProjectileComponent& component() const { return mComponent; }
    HitResult const&     hitResult() const { return mHitResult; }
};

} // namespace Catalyst