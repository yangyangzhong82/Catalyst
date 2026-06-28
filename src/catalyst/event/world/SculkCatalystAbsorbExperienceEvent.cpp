#include "SculkCatalystAbsorbExperienceEvent.h"

#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/block/actor/SculkCatalystBlockActor.h"

#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void SculkCatalystAbsorbExperienceEvent::serialize(CompoundTag& nbt) const {
    ll::event::world::LevelEvent::serialize(nbt);
    nbt["blockActor"] = ll::event::serializeRefObj(blockActor());
    nbt["actor"]      = ll::event::serializeRefObj(actor());
}


LL_TYPE_INSTANCE_HOOK(
    SculkCatalystAbsorbExperienceEventHook,
    ll::memory::HookPriority::Normal,
    SculkCatalystBlockActor,
    &SculkCatalystBlockActor::_tryConsumeOnDeathExperience,
    void,
    Level& level,
    Actor& actor
) {
    auto& bus = ll::event::EventBus::getInstance();

    SculkCatalystAbsorbExperienceBeforeEvent beforeEvent(*this, level, actor);
    bus.publish(beforeEvent);

    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(level, actor);

    SculkCatalystAbsorbExperienceAfterEvent afterEvent(*this, level, actor);
    bus.publish(afterEvent);
}

CATALYST_HOOKED_EVENT_PAIR(
    SculkCatalystAbsorbExperienceBeforeEvent,
    SculkCatalystAbsorbExperienceAfterEvent,
    SculkCatalystAbsorbExperienceEventHook
)

} // namespace Catalyst
