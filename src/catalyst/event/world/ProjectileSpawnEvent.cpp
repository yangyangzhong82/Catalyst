#include "ProjectileSpawnEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/level/BedrockSpawner.h"

namespace Catalyst {

LL_TYPE_INSTANCE_HOOK(
    ProjectileSpawnEventHook,
    ll::memory::HookPriority::Normal,
    BedrockSpawner,
    &BedrockSpawner::$spawnProjectile,
    Actor*,
    ::BlockSource&                     region,
    ::ActorDefinitionIdentifier const& id,
    ::Actor*                           spawner,
    ::Vec3 const&                      position,
    ::Vec3 const&                      direction
) {
    auto& bus = ll::event::EventBus::getInstance();

    ProjectileSpawnBeforeEvent beforeEvent(region, id, spawner, position, direction);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return nullptr;
    }

    Actor* result = origin(region, id, spawner, position, direction);

    if (result) {
        ProjectileSpawnAfterEvent afterEvent(region, id, spawner, position, direction, result);
        bus.publish(afterEvent);
    }
    return result;
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class ProjectileSpawnBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, ProjectileSpawnBeforeEvent> {
    ll::memory::HookRegistrar<ProjectileSpawnEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<ProjectileSpawnBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class ProjectileSpawnAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, ProjectileSpawnAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<ProjectileSpawnAfterEventEmitter>();
}

} // namespace Catalyst