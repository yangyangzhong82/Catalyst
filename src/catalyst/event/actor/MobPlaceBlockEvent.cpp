#include "MobPlaceBlockEvent.h"

#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/util/BlockUtils.h"
#include "mc/util/Random.h"
#include "mc/util/VariantParameterList.h"
#include "mc/util/VariantParameterListConst.h"
#include "mc/world/actor/ActorDefinitionDescriptor.h"
#include "mc/world/actor/Mob.h"
#include "mc/world/actor/ai/goal/PlaceBlockGoal.h"
#include "mc/world/events/gameevents/GameEventRegistry.h"
#include "mc/world/item/ItemStack.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/block/BlockChangeContext.h"
#include "mc/world/level/block/Block.h"

#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void MobPlaceBlockEvent::serialize(CompoundTag& nbt) const {
    ll::event::entity::MobEvent::serialize(nbt);
    nbt["pos"]   = ListTag{pos().x, pos().y, pos().z};
    nbt["block"] = ll::event::serializeRefObj(block());
}



LL_TYPE_INSTANCE_HOOK(
    MobPlaceBlockEventHook,
    ll::memory::HookPriority::Normal,
    PlaceBlockGoal,
    &PlaceBlockGoal::$tick,
    void
) {
    auto& mob    = this->mMob;
    auto& random = mob.getRandom();

    // 获取范围配置
    auto&    def       = this->mDefinition.get();
    BlockPos targetPos = BlockUtils::getRandomPos(
        random,
        BlockPos(mob.getPosition()),
        def.mXZRange.get(),
        def.mYRange.get()
    );

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

    if (belowBlock.isAir() || !belowBlock.isSolidBlockingBlock()) {
        return;
    }

    auto& bus = ll::event::EventBus::getInstance();

    // 区分两种放置模式
    VariantParameterList triggerParams{};
    mob.initParams(triggerParams);
    triggerParams.mBlock = &targetPos;

    auto& randomBlocks = def.mRandomlyPlaceableBlocks.get();

    if (randomBlocks.empty()) {
        // 模式A: 使用携带的方块 (_tryPlaceCarriedBlock)
        auto const& carried = mob.getCarriedItem();
        auto const* toPlace = carried.mBlock;
        if (!toPlace || toPlace->isAir()) {
            return;
        }

        // 发布 BeforeEvent
        MobPlaceBlockBeforeEvent beforeEvent(mob, targetPos, *toPlace);
        bus.publish(beforeEvent);
        if (beforeEvent.isCancelled()) {
            return;
        }

        this->_tryPlaceCarriedBlock(blockSource, targetPos, triggerParams);

        // 发布 AfterEvent
        auto& afterBlock = blockSource.getBlock(targetPos);
        if (!afterBlock.isAir()) {
            MobPlaceBlockAfterEvent afterEvent(mob, targetPos, afterBlock);
            bus.publish(afterEvent);
        }
    } else {
        // 模式B:  (_tryGetRandomPlaceBlock)
        VariantParameterListConst blockPickParams = static_cast<VariantParameterListConst>(triggerParams);
        auto const* randomBlock = this->_tryGetRandomPlaceBlock(blockPickParams, random);

        if (randomBlock) {
            // 发布 BeforeEvent
            MobPlaceBlockBeforeEvent beforeEvent(mob, targetPos, *randomBlock);
            bus.publish(beforeEvent);
            if (beforeEvent.isCancelled()) {
                return;
            }

            // 调用 _placeBlock
            BlockChangeContext changeContext{};
            if (!blockSource.setBlock(targetPos, *randomBlock, 3, nullptr, changeContext)) {
                return;
            }

            blockSource.postGameEvent(&mob, GameEventRegistry::blockPlace(), targetPos, randomBlock);

            ActorDefinitionDescriptor::executeTrigger(mob, def.mOnPlace.get(), triggerParams);

            // 发布 AfterEvent
            auto& afterBlock = blockSource.getBlock(targetPos);
            if (!afterBlock.isAir()) {
                MobPlaceBlockAfterEvent afterEvent(mob, targetPos, afterBlock);
                bus.publish(afterEvent);
            }
        }
    }
}

CATALYST_HOOKED_EVENT_PAIR(
    MobPlaceBlockBeforeEvent,
    MobPlaceBlockAfterEvent,
    MobPlaceBlockEventHook
)

} // namespace Catalyst

