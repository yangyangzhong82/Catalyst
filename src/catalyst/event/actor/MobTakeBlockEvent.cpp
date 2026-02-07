#include "MobTakeBlockEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/gameplayhandlers/CoordinatorResult.h"
#include "mc/util/Random.h"
#include "mc/util/Randomize.h"
#include "mc/util/VariantParameterList.h"
#include "mc/world/actor/ActorDefinitionDescriptor.h"
#include "mc/world/actor/Mob.h"
#include "mc/world/actor/ai/goal/TakeBlockGoal.h"
#include "mc/world/events/ActorEventCoordinator.h"
#include "mc/world/events/ActorGameplayEvent.h"
#include "mc/world/events/ActorGriefingBlockEvent.h"
#include "mc/world/events/EventRef.h"
#include "mc/world/events/MutableActorGameplayEvent.h"
#include "mc/world/events/gameevents/GameEventRegistry.h"
#include "mc/world/item/ItemStack.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/ShapeType.h"
#include "mc/world/level/block/ActorChangeContext.h"
#include "mc/world/level/block/Block.h"
#include "mc/world/level/block/BlockChangeContext.h"
#include "mc/world/level/block/BlockDescriptor.h"
#include "mc/world/level/dimension/Dimension.h"
BlockChangeContext::BlockChangeContext() : mContextSource(std::monostate{}) {}
namespace Catalyst {


LL_TYPE_INSTANCE_HOOK(TakeBlockGoalTickHook, HookPriority::Normal, TakeBlockGoal, &TakeBlockGoal::$tick, void) {
    auto&     level  = mMob.getLevel();
    auto&     random = level.getRandom();
    Randomize randomize(random);
    auto&     mobPos = mMob.getPosition();
    BlockPos  targetPos(mobPos);

    auto& def     = mDefinition.get();
    auto& xzRange = def.mXZRange.get();
    auto& yRange  = def.mYRange.get();

    int xzMin = xzRange.rangeMin;
    int xzMax = xzRange.rangeMax;
    int yMin  = yRange.rangeMin;
    int yMax  = yRange.rangeMax;

    if (xzMin < xzMax) {
        targetPos.x += random.nextInt(xzMax - xzMin + 1) + xzMin;
    }
    if (yMin < yMax) {
        targetPos.y += random.nextInt(yMax - yMin + 1) + yMin;
    }
    if (xzMin < xzMax) {
        targetPos.z += random.nextInt(xzMax - xzMin + 1) + xzMin;
    }

    auto& blockSource = mMob.getDimensionBlockSource();
    auto& block       = blockSource.getBlock(targetPos);

    if (block.isAir()) {
        return;
    }

    auto& validBlocks = def.mValidBlocks.get();
    if (!validBlocks.empty()) {
        if (!BlockDescriptor::anyMatch(validBlocks, block)) {
            return;
        }
    }

    if (def.mRequiresLineOfSight) {
        Vec3 blockCenter((float)targetPos.x, (float)targetPos.y, (float)targetPos.z);
        if (!mMob.canSee(blockCenter, ShapeType::Collision)) {
            return;
        }
    }

    auto& bus = ll::event::EventBus::getInstance();

    // 发布 BeforeEvent
    MobTakeBlockBeforeEvent beforeEvent(mMob, targetPos, block);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    // 获取 ActorEventCoordinator 并发送事件
    auto& eventCoordinator = level.getActorEventCoordinator();

    ActorGriefingBlockEvent const event{
        mMob.getEntityContext().getWeakRef(),
        &block,
        Vec3((float)targetPos.x, (float)targetPos.y, (float)targetPos.z),
        nullptr
    };

    EventRef<ActorGameplayEvent<CoordinatorResult>> const eventRef(event);
    using SendEventFunc =
        CoordinatorResult (ActorEventCoordinator::*)(EventRef<ActorGameplayEvent<CoordinatorResult>> const&);
    CoordinatorResult result =
        (eventCoordinator.*static_cast<SendEventFunc>(&ActorEventCoordinator::sendEvent))(eventRef);
    if (result != CoordinatorResult::Continue) {
        return;
    }

    // 创建物品并添加到生物背包
    ItemStack item(block, 1, nullptr);
    mMob.add(item);

    // 移除方块
    BlockChangeContext changeContext;
    changeContext.mContextSource = ActorChangeContext{&mMob};
    blockSource.removeBlock(targetPos, changeContext);

    // 发送游戏事件
    blockSource.postGameEvent(&mMob, GameEventRegistry::blockDestroy(), targetPos, &block);

    // 执行触发器
    VariantParameterList params;
    params.mSelf  = &mMob;
    params.mBlock = &targetPos;
    auto& trigger = def.mOnTake.get();
    ActorDefinitionDescriptor::executeTrigger(mMob, trigger, params);

    // 发布 AfterEvent
    MobTakeBlockAfterEvent afterEvent(mMob, targetPos, block);
    bus.publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class MobTakeBlockBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, MobTakeBlockBeforeEvent> {
    ll::memory::HookRegistrar<TakeBlockGoalTickHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<MobTakeBlockBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class MobTakeBlockAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, MobTakeBlockAfterEvent> {
    ll::memory::HookRegistrar<TakeBlockGoalTickHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<MobTakeBlockAfterEventEmitter>();
}

} // namespace Catalyst
