#include "BlockExplodedEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/events/ExplosionStartedEvent.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/Explosion.h"
#include "mc/world/level/block/Block.h"

namespace Catalyst {

std::unordered_set<::BlockPos>* affectedBlocks = nullptr;
BlockSource*                    region         = nullptr;

LL_TYPE_INSTANCE_HOOK(
    BlockExplodedHook1,
    ll::memory::HookPriority::Normal,
    Explosion,
    &Explosion::explode,
    bool,
    ::IRandom& random
) {
    affectedBlocks = &mAffectedBlocks.get();
    region         = &mRegion;
    auto res       = origin(random);
    affectedBlocks = nullptr;
    region         = nullptr;
    return res;
}

LL_TYPE_INSTANCE_HOOK(
    BlockExplodedHook2,
    ll::memory::HookPriority::Normal,
    ExplosionStartedEvent,
    &ExplosionStartedEvent::$dtor,
    void
) {
    if (!affectedBlocks || !region) {
        return origin();
    }

    auto& bus = ll::event::EventBus::getInstance();
    std::unordered_set<BlockPos> replaced;
    for (const auto& pos : *affectedBlocks) {
        BlockExplodedBeforeEvent beforeEvent(*region, pos);
        bus.publish(beforeEvent);
        if (!beforeEvent.isCancelled()) {
            replaced.emplace(pos);
        }
    }
    *affectedBlocks = replaced;

    origin();

    for (const auto& pos : replaced) {
        BlockExplodedAfterEvent afterEvent(*region, pos);
        bus.publish(afterEvent);
    }
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class BlockExplodedBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, BlockExplodedBeforeEvent> {
    ll::memory::HookRegistrar<BlockExplodedHook1> hook1;
    ll::memory::HookRegistrar<BlockExplodedHook2> hook2;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<BlockExplodedBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class BlockExplodedAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, BlockExplodedAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<BlockExplodedAfterEventEmitter>();
}

} // namespace Catalyst
