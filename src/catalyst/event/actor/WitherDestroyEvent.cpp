#include "WitherDestroyEvent.h"

#include "catalyst/mod/Gloabl.h"
#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"


#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void WitherDestroyBlocksEvent::serialize(CompoundTag& nbt) const {
    ll::event::entity::MobEvent::serialize(nbt);
    nbt["aabb"]       = ll::event::serializeRefObj(aabb());
    nbt["region"]     = ll::event::serializeRefObj(region());
    nbt["range"]      = range();
    nbt["attackType"] = magic_enum::enum_name(attackType());
}


LL_TYPE_INSTANCE_HOOK(
    WitherDestroyBlocksEventHook,
    ll::memory::HookPriority::Normal,
    WitherBoss,
    &WitherBoss::_destroyBlocks,
    void,
    ::Level&                       level,
    ::AABB const&                  bb,
    ::BlockSource&                 region,
    int                            range,
    ::WitherBoss::WitherAttackType attackType
) {
    auto& bus = ll::event::EventBus::getInstance();

    WitherDestroyBlocksBeforeEvent beforeEvent(*this, bb, region, range, attackType);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(level, bb, region, range, attackType);

    WitherDestroyBlocksAfterEvent afterEvent(*this, bb, region, range, attackType);
    bus.publish(afterEvent);
}

CATALYST_HOOKED_EVENT_PAIR(
    WitherDestroyBlocksBeforeEvent,
    WitherDestroyBlocksAfterEvent,
    WitherDestroyBlocksEventHook
)

} // namespace Catalyst
