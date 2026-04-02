#include "SculkCatalystAbsorbExperienceEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/block/actor/SculkCatalystBlockActor.h"

#include "mc/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void SculkCatalystAbsorbExperienceBeforeEvent::serialize(CompoundTag& nbt) const {
    Cancellable::serialize(nbt);
    nbt["blockActor"] = ll::event::serializeRefObj(blockActor());
    nbt["actor"]      = ll::event::serializeRefObj(actor());
}

void SculkCatalystAbsorbExperienceAfterEvent::serialize(CompoundTag& nbt) const {
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

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class SculkCatalystAbsorbExperienceBeforeEventEmitter
: public ll::event::Emitter<beforeEmitterFactory, SculkCatalystAbsorbExperienceBeforeEvent> {
    ll::memory::HookRegistrar<SculkCatalystAbsorbExperienceEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<SculkCatalystAbsorbExperienceBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class SculkCatalystAbsorbExperienceAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, SculkCatalystAbsorbExperienceAfterEvent> {
    ll::memory::HookRegistrar<SculkCatalystAbsorbExperienceEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<SculkCatalystAbsorbExperienceAfterEventEmitter>();
}

} // namespace Catalyst