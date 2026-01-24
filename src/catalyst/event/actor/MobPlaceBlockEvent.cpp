#include "MobPlaceBlockEvent.h"

#include "catalyst/mod/Gloabl.h"
#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/util/Random.h"
#include "mc/util/VariantParameterList.h"
#include "mc/util/VariantParameterListConst.h"
#include "mc/world/actor/ActorDefinitionDescriptor.h"
#include "mc/world/actor/Mob.h"
#include "mc/world/actor/ai/goal/PlaceBlockGoal.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/block/Block.h"

namespace Catalyst {


LL_TYPE_INSTANCE_HOOK(
    MobPlaceBlockEventHook,
    ll::memory::HookPriority::Normal,
    PlaceBlockGoal,
    &PlaceBlockGoal::$tick,
    void
) {
    auto& mob    = this->mMob;
    auto& level  = mob.getLevel();
    auto& random = level.getRandom();

    auto     pos = mob.getPosition();
    BlockPos targetPos(pos);

    // 获取范围配置
    auto& def     = this->mDefinition.get();
    auto& xzRange = def.mXZRange.get();
    auto& yRange  = def.mYRange.get();

    int xzMin = xzRange.rangeMin;
    int xzMax = xzRange.rangeMax;
    int yMin  = yRange.rangeMin;
    int yMax  = yRange.rangeMax;

    // 随机化X坐标
    if (xzMin < xzMax) {
        targetPos.x += xzMin + random.nextInt(xzMax - xzMin + 1);
    }

    // 随机化Y坐标
    if (yMin < yMax) {
        targetPos.y += yMin + random.nextInt(yMax - yMin + 1);
    }

    // 随机化Z坐标
    if (xzMin < xzMax) {
        targetPos.z += xzMin + random.nextInt(xzMax - xzMin + 1);
    }

    auto& blockSource = mob.getDimensionBlockSource();
    auto& targetBlock = blockSource.getBlock(targetPos);

    // 检查目标位置是否为空气
    if (!targetBlock.isAir()) {
        return;
    }

    // 检查下方方块
    BlockPos belowPos  = targetPos;
    belowPos.y        -= 1;
    auto& belowBlock   = blockSource.getBlock(belowPos);

    if (belowBlock.isAir() || !belowBlock._isSolid()) {
        return;
    }

    auto& bus = ll::event::EventBus::getInstance();

    // 区分两种放置模式
    auto& randomBlocks = def.mRandomlyPlaceableBlocks.get();

    if (randomBlocks.empty()) {
        // 模式A: 使用携带的方块 (_tryPlaceCarriedBlock)
        auto& beforeBlock = blockSource.getBlock(targetPos);

        // 发布 BeforeEvent
        MobPlaceBlockBeforeEvent beforeEvent(mob, targetPos, beforeBlock);
        bus.publish(beforeEvent);
        if (beforeEvent.isCancelled()) {
            return;
        }
        logger.debug("(_tryPlaceCarriedBlock ({}, {}, {})", targetPos.x, targetPos.y, targetPos.z);
        // 调用 _tryPlaceCarriedBlock
        VariantParameterList params;
        this->_tryPlaceCarriedBlock(blockSource, targetPos, params);

        // 发布 AfterEvent
        auto& afterBlock = blockSource.getBlock(targetPos);
        if (!afterBlock.isAir() && &afterBlock != &beforeBlock) {
            MobPlaceBlockAfterEvent afterEvent(mob, targetPos, afterBlock);
            bus.publish(afterEvent);
        }
    } else {
        // 模式B: 使用随机方块列表 (_tryGetRandomPlaceBlock)
        VariantParameterListConst params{};
        auto*                     randomBlock = this->_tryGetRandomPlaceBlock(params, random);

        if (randomBlock) {
            // 发布 BeforeEvent
            MobPlaceBlockBeforeEvent beforeEvent(mob, targetPos, *randomBlock);
            bus.publish(beforeEvent);
            if (beforeEvent.isCancelled()) {
                return;
            }

            // 调用 _placeBlock
            VariantParameterList placeParams;
            this->_placeBlock(blockSource, targetPos, placeParams, *randomBlock);
            logger.debug("_tryGetRandomPlaceBlock at ({}, {}, {})", targetPos.x, targetPos.y, targetPos.z);
            // 执行触发器 (_executeTrigger)
            auto& trigger = def.mOnPlace.get();
            ActorDefinitionDescriptor::executeTrigger(mob, trigger, placeParams);

            // 发布 AfterEvent
            MobPlaceBlockAfterEvent afterEvent(mob, targetPos, *randomBlock);
            bus.publish(afterEvent);
        }
    }
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class MobPlaceBlockBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, MobPlaceBlockBeforeEvent> {
    ll::memory::HookRegistrar<MobPlaceBlockEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<MobPlaceBlockBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class MobPlaceBlockAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, MobPlaceBlockAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<MobPlaceBlockAfterEventEmitter>();
}

} // namespace Catalyst
