#include "LeavesDecayEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/block/Block.h"
#include "mc/world/level/block/LeavesBlock.h"

#include "ll/api/event/EventRefObjSerializer.h"
#include "mc/deps/nbt/CompoundTag.h"
/*
namespace Catalyst {

void LeavesDecayBeforeEvent::serialize(CompoundTag& nbt) const {
    Cancellable::serialize(nbt);
    nbt["pos"]         = ListTag{pos().x, pos().y, pos().z};
    nbt["leavesBlock"] = ll::event::serializeRefObj(leavesBlock());
}

void LeavesDecayAfterEvent::serialize(CompoundTag& nbt) const {
    ll::event::world::WorldEvent::serialize(nbt);
    nbt["pos"]         = ListTag{pos().x, pos().y, pos().z};
    nbt["leavesBlock"] = ll::event::serializeRefObj(leavesBlock());
}


LL_TYPE_INSTANCE_HOOK(
    LeavesDecayHook,
    ll::memory::HookPriority::Normal,
    LeavesBlock,
    &LeavesBlock::_die,
    void,
    ::BlockSource&    region,
    ::BlockPos const& pos
) {
    auto&       bus         = ll::event::EventBus::getInstance();
    auto const& leavesBlock = region.getBlock(pos);

    LeavesDecayBeforeEvent beforeEvent(region, pos, leavesBlock);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(region, pos);

    LeavesDecayAfterEvent afterEvent(region, pos, leavesBlock);
    bus.publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class LeavesDecayBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, LeavesDecayBeforeEvent> {
    ll::memory::HookRegistrar<LeavesDecayHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<LeavesDecayBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class LeavesDecayAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, LeavesDecayAfterEvent> {
    ll::memory::HookRegistrar<LeavesDecayHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<LeavesDecayAfterEventEmitter>();
}

} // namespace Catalyst
*/