#include "WitherDestroyEvent.h"

#include "catalyst/mod/Gloabl.h"
#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/block/components/BlockComponentDescription.h"
#include "mc/world/level/block/components/StickyType.h"
#include "mc\world\inventory\network\SparseContainer.h"
#include "mc\world\level\block\BlockType.h"
#include "mc\world\level\block\actor\BeaconBlockActor.h"
#include "mc\world\level\block\actor\CauldronBlockActor.h"
#include "mc\world\level\block\components\BlockMovableDescription.h"

namespace Catalyst {

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

LL_AUTO_TYPE_INSTANCE_HOOK(
    Hook1112,
    ll::memory::HookPriority::Normal,
    BlockType,
    &BlockType::addComponent,
    ::BlockType&,
    ::BlockComponentDescription const& blockComponentDescription
) {
    BlockMovableDescription blockMovableDesc(MovementType::Immovable, StickyType::None);
    auto                    a = blockComponentDescription.getName();
    if (a == "minecraft:movable") {
        auto& b =
            const_cast<BlockMovableDescription&>(static_cast<const BlockMovableDescription&>(blockComponentDescription)
            );
        b.mMovementType = MovementType::Immovable;
        //logger.info("Hooked BlockType::addComponent{}", this->getTypeName());
    }
    //logger.info("{}", blockComponentDescription.getName());
    origin(blockMovableDesc);
    return origin(blockComponentDescription);
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class WitherDestroyBlocksBeforeEventEmitter
: public ll::event::Emitter<beforeEmitterFactory, WitherDestroyBlocksBeforeEvent> {
    ll::memory::HookRegistrar<WitherDestroyBlocksEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<WitherDestroyBlocksBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class WitherDestroyBlocksAfterEventEmitter
: public ll::event::Emitter<afterEmitterFactory, WitherDestroyBlocksAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<WitherDestroyBlocksAfterEventEmitter>();
}

} // namespace Catalyst