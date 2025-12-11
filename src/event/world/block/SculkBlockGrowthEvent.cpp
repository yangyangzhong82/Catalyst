#include "SculkBlockGrowthEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/block/SculkBlockBehavior.h"

namespace Catalyst {

LL_AUTO_STATIC_HOOK(
    SculkBlockGrowthHook,
    ll::memory::HookPriority::Normal,
    &SculkBlockBehavior::_placeGrowthAt,
    void,
    ::IBlockWorldGenAPI& target,
    ::BlockSource*       region,
    ::BlockPos const&    pos,
    ::Random&            random,
    ::SculkSpreader&     spreader
) {
    auto& bus = ll::event::EventBus::getInstance();

    SculkBlockGrowthBeforeEvent beforeEvent(*region, pos);
    bus.publish(beforeEvent);

    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(target, region, pos, random, spreader);

    SculkBlockGrowthAfterEvent afterEvent(*region, pos);
    bus.publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class SculkBlockGrowthBeforeEventEmitter
: public ll::event::Emitter<beforeEmitterFactory, SculkBlockGrowthBeforeEvent> {
    ll::memory::HookRegistrar<SculkBlockGrowthHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<SculkBlockGrowthBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class SculkBlockGrowthAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, SculkBlockGrowthAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<SculkBlockGrowthAfterEventEmitter>();
}

} // namespace Catalyst