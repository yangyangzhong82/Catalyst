#include "FarmChangeEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/block/FarmBlock.h"

namespace Catalyst {

LL_AUTO_TYPE_INSTANCE_HOOK(
    FarmChangeEventHook,
    ll::memory::HookPriority::Normal,
    FarmBlock,
    &FarmBlock::$transformOnFall,
    void,
    ::BlockSource&    region,
    ::BlockPos const& pos,
    ::Actor*          actor,
    float             fallDistance
) {
    auto& bus = ll::event::EventBus::getInstance();

    FarmChangeBeforeEvent beforeEvent(region, pos, false);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(region, pos, actor, fallDistance);

    FarmChangeAfterEvent afterEvent(region, pos, false);
    bus.publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class FarmChangeBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, FarmChangeBeforeEvent> {
    ll::memory::HookRegistrar<FarmChangeEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<FarmChangeBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class FarmChangeAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, FarmChangeAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<FarmChangeAfterEventEmitter>();
}

} // namespace Catalyst