#include "ProjectileHitEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/entity/components_json_legacy/ProjectileComponent.h"
#include "mc/world/actor/Actor.h"

namespace Catalyst {

LL_TYPE_INSTANCE_HOOK(
    ProjectileHitEventHook,
    ll::memory::HookPriority::Normal,
    ProjectileComponent,
    &ProjectileComponent::onHit,
    void,
    ::Actor&           owner,
    ::HitResult const& hitResult
) {
    auto& bus = ll::event::EventBus::getInstance();

    ProjectileHitBeforeEvent beforeEvent(owner, *this, hitResult);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(owner, hitResult);

    ProjectileHitAfterEvent afterEvent(owner, *this, hitResult);
    bus.publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class ProjectileHitBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, ProjectileHitBeforeEvent> {
    ll::memory::HookRegistrar<ProjectileHitEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<ProjectileHitBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class ProjectileHitAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, ProjectileHitAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<ProjectileHitAfterEventEmitter>();
}

} // namespace Catalyst