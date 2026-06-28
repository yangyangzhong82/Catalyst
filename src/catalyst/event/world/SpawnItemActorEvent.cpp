#include "SpawnItemActorEvent.h"

#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/item/ItemActor.h"
#include "mc/world/level/BedrockSpawner.h"

#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void SpawnItemActorEvent::serialize(CompoundTag& nbt) const {
    ll::event::world::WorldEvent::serialize(nbt);
    nbt["item"]      = ll::event::serializeRefObj(item());
    nbt["spawner"]   = ll::event::serializePtrObj(spawner());
    nbt["pos"]       = ListTag{pos().x, pos().y, pos().z};
    nbt["throwTime"] = throwTime();
}

void SpawnItemActorAfterEvent::serialize(CompoundTag& nbt) const {
    SpawnItemActorEvent::serialize(nbt);
    nbt["itemActor"] = ll::event::serializePtrObj(itemActor());
}


LL_TYPE_INSTANCE_HOOK(
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

CATALYST_HOOKED_EVENT_PAIR(
    SpawnItemActorBeforeEvent,
    SpawnItemActorAfterEvent,
    SpawnItemActorEventHook
)

} // namespace Catalyst
