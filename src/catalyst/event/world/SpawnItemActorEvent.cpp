#include "SpawnItemActorEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/item/ItemActor.h"
#include "mc/world/level/BedrockSpawner.h"

namespace Catalyst {

LL_AUTO_TYPE_INSTANCE_HOOK(
    SpawnItemActorEventHook,
    ll::memory::HookPriority::Normal,
    BedrockSpawner,
    &BedrockSpawner::$spawnItem,
    ItemActor*,
    ::BlockSource&     region,
    ::ItemStack const& inst,
    ::Actor*           spawner,
    ::Vec3 const&      pos,
    int                throwTime
) {
    auto& bus = ll::event::EventBus::getInstance();

    SpawnItemActorBeforeEvent beforeEvent(region, inst, spawner, pos, throwTime);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return nullptr;
    }

    ItemActor* result = origin(region, inst, spawner, pos, throwTime);

    if (result) {
        SpawnItemActorAfterEvent afterEvent(region, inst, spawner, pos, throwTime, result);
        bus.publish(afterEvent);
    }
    return result;
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class SpawnItemActorBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, SpawnItemActorBeforeEvent> {
    ll::memory::HookRegistrar<SpawnItemActorEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<SpawnItemActorBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class SpawnItemActorAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, SpawnItemActorAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<SpawnItemActorAfterEventEmitter>();
}

} // namespace Catalyst