#include "ll/api/memory/Hook.h"
#include "mc/world/level/BedrockSpawner.h"
#include "mc/world/actor/Actor.h"

LL_AUTO_TYPE_INSTANCE_HOOK(
    ProjectileSpawnHook1,
    HookPriority::Normal,
    BedrockSpawner,
    &BedrockSpawner::$spawnProjectile,
    Actor*,
    BlockSource&                     region,
    ActorDefinitionIdentifier const& id,
    Actor*                           spawner,
    Vec3 const&                      position,
    Vec3 const&                      direction
) {
    return nullptr;
}