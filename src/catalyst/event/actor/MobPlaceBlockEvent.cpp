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
#include "mc/deps/core/math/Vec3.h"
#include "mc/world/events/gameevents/GameEventRegistry.h"
#include "mc/world/item/ItemStack.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/block/ActorChangeContext.h"
#include "mc/world/level/block/BlockChangeContext.h"
#include "mc/world/level/block/Block.h"
#include "mc/world/level/dimension/Dimension.h"
#include "mc/network/packet/MobEquipmentPacket.h"

#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void MobPlaceBlockBeforeEvent::serialize(CompoundTag& nbt) const {
    Cancellable::serialize(nbt);
    nbt["pos"]   = ListTag{pos().x, pos().y, pos().z};
    nbt["block"] = ll::event::serializeRefObj(block());
}

void MobPlaceBlockAfterEvent::serialize(CompoundTag& nbt) const {
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
    auto& level  = mob.getLevel();
    auto& random = level.getThreadRandom();

    BlockPos targetPos(mob.getPosition());

    // 获取范围配置
    auto& def     = this->mDefinition.get();
    auto& xzRange = def.mXZRange.get();
    auto& yRange  = def.mYRange.get();

    int xzMin = xzRange.rangeMin;
    int xzMax = xzRange.rangeMax;
    int yMin  = yRange.rangeMin;
    int yMax  = yRange.rangeMax;

    // 随机化X坐标
    targetPos.x += (xzMin < xzMax) ? (random.nextInt(xzMax - xzMin + 1) + xzMin) : xzMin;

    // 随机化Y坐标
    targetPos.y += (yMin < yMax) ? (random.nextInt(yMax - yMin + 1) + yMin) : yMin;

    // 随机化Z坐标
    targetPos.z += (xzMin < xzMax) ? (random.nextInt(xzMax - xzMin + 1) + xzMin) : xzMin;

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

    if (belowBlock.isAir() || !blockSource.isSolidBlockingBlock(belowPos)) {
        return;
    }

    auto& bus = ll::event::EventBus::getInstance();

    // 区分两种放置模式
    VariantParameterList triggerParams{};
    triggerParams.mSelf   = &mob;
    triggerParams.mTarget = mob.getTarget();
    triggerParams.mBlock  = &targetPos;

    VariantParameterListConst blockPickParams{};
    blockPickParams.mSelf   = &mob;
    blockPickParams.mTarget = mob.getTarget();
    blockPickParams.mBlock  = &targetPos;

    BlockChangeContext changeContext;
    changeContext.mContextSource = ActorChangeContext{&mob};

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
        // 调用 _tryPlaceCarriedBlock
        if (!blockSource.mayPlace(*toPlace, targetPos, 1, &mob, false, Vec3(0.0f, 0.0f, 0.0f))) {
            return;
        }

        mob.setCarriedItem(ItemStack::EMPTY_ITEM());
        {
            MobEquipmentPacket pkt(mob.getRuntimeID(), ItemStack::EMPTY_ITEM(), 0, 0, ContainerID::Inventory);
            mob.getDimension().sendPacketForEntity(mob, pkt, nullptr);
        }

        if (!blockSource.setBlock(targetPos, *toPlace, 3, nullptr, changeContext)) {
            return;
        }

        blockSource.postGameEvent(&mob, GameEventRegistry::blockPlace(), targetPos, toPlace);

        std::vector<std::pair<std::string const, std::string const>> eventStack;
        ActorDefinitionDescriptor::_executeTrigger(mob, def.mOnPlace.get(), eventStack, triggerParams);

        // 发布 AfterEvent
        auto& afterBlock = blockSource.getBlock(targetPos);
        if (!afterBlock.isAir()) {
            MobPlaceBlockAfterEvent afterEvent(mob, targetPos, afterBlock);
            bus.publish(afterEvent);
        }
    } else {
        // 模式B:  (_tryGetRandomPlaceBlock)
        auto const* randomBlock = this->_tryGetRandomPlaceBlock(blockPickParams, random);

        if (randomBlock) {
            // 发布 BeforeEvent
            MobPlaceBlockBeforeEvent beforeEvent(mob, targetPos, *randomBlock);
            bus.publish(beforeEvent);
            if (beforeEvent.isCancelled()) {
                return;
            }

            // 调用 _placeBlock
            if (!blockSource.setBlock(targetPos, *randomBlock, 3, nullptr, changeContext)) {
                return;
            }

            blockSource.postGameEvent(&mob, GameEventRegistry::blockPlace(), targetPos, randomBlock);

            std::vector<std::pair<std::string const, std::string const>> eventStack;
            ActorDefinitionDescriptor::_executeTrigger(mob, def.mOnPlace.get(), eventStack, triggerParams);
            // 执行触发器 (_executeTrigger)

            // 发布 AfterEvent
            auto& afterBlock = blockSource.getBlock(targetPos);
            if (!afterBlock.isAir()) {
                MobPlaceBlockAfterEvent afterEvent(mob, targetPos, afterBlock);
                bus.publish(afterEvent);
            }
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
class MobPlaceBlockAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, MobPlaceBlockAfterEvent> {
    ll::memory::HookRegistrar<MobPlaceBlockEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<MobPlaceBlockAfterEventEmitter>();
}

} // namespace Catalyst
