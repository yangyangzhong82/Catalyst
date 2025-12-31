#include "BlockPistonEvent.h"

#include "catalyst/mod/Gloabl.h"
#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/block/Block.h"
#include "mc/world/level/block/VanillaStates.h"
#include "mc/world/level/block/actor/PistonBlockActor.h"
#include "mc/world/level/block/actor/PistonState.h"
#include "mc/world/level/dimension/Dimension.h"
#include "mc/world/redstone/circuit/CircuitSystem.h"


namespace Catalyst {

enum class PistonStateEx : char {
    ExpandingCancelled  = 4,
    RetractingCancelled = 5,
};

LL_AUTO_TYPE_INSTANCE_HOOK(
    PistonBlockEventHook,
    ll::memory::HookPriority::Normal,
    PistonBlockActor,
    &PistonBlockActor::$tick,
    void,
    ::BlockSource& region
) {
    bool  extending  = false;
    bool  retracting = false;
    auto& dimension  = region.getDimension();
    auto& circuit    = dimension.mCircuitSystem;
    auto  pos        = this->mPosition;
    auto  state      = this->mState;


    auto strength = circuit->getStrength(pos);
    /*
        logger.debug(
            "活塞位置: ({}, {}, {}), 当前状态: {}, 新状态: {}, 信号强度: {}",
            pos->x,
            pos->y,
            pos->z,
            (int)state,
            (int)this->mNewState,
            strength
        );
    */
    if (strength > 0) {

        if (state == PistonState::Retracted && this->mNewState == PistonState::Retracted) {
            extending = true;
            logger.debug("活塞准备伸展 - 位置: ({}, {}, {})", pos->x, pos->y, pos->z);
        } else if (state == PistonState::Expanded
                   && static_cast<PistonStateEx>(this->mNewState) == PistonStateEx::RetractingCancelled) {
            this->mNewState = PistonState::Expanded;
            logger.debug("重置活塞收缩取消状态");
        }
    } else {
        // 无红石信号时处理收缩 - 只在状态转换时触发
        if (state == PistonState::Expanded && this->mNewState == PistonState::Expanded) {
            retracting = true;
            logger.debug("活塞准备收缩 - 位置: ({}, {}, {})", pos->x, pos->y, pos->z);
        } else if (state == PistonState::Retracted
                   && static_cast<PistonStateEx>(this->mNewState) == PistonStateEx::ExpandingCancelled) {
            this->mNewState = PistonState::Retracted;
            logger.debug("重置活塞伸展取消状态");
        }
    }

    if (extending || retracting) {
        auto& bus       = ll::event::EventBus::getInstance();
        auto& block     = region.getBlock(pos);
        auto  facing    = block.getState<int>(VanillaStates::FacingDirection());
        int   direction = facing.has_value() ? facing.value() : 0;

        PistonAction action = extending ? PistonAction::Extend : PistonAction::Retract;


        BlockPistonBeforeEvent beforeEvent(region, pos, action, direction);
        bus.publish(beforeEvent);

        logger.debug(
            "活塞{}事件 - 位置: ({}, {}, {}), 方向: {}",
            extending ? "伸展" : "收缩",
            pos->x,
            pos->y,
            pos->z,
            direction
        );

        if (beforeEvent.isCancelled()) {
            // 设置取消状态
            if (extending) {
                this->mNewState = static_cast<PistonState>(PistonStateEx::ExpandingCancelled);
                logger.debug("活塞伸展被取消 - 位置: ({}, {}, {})", pos->x, pos->y, pos->z);
            } else {
                this->mNewState = static_cast<PistonState>(PistonStateEx::RetractingCancelled);
                logger.debug("活塞收缩被取消 - 位置: ({}, {}, {})", pos->x, pos->y, pos->z);
            }
            origin(region);
            return;
        }


        origin(region);

        BlockPistonAfterEvent afterEvent(region, pos, action, direction);
        bus.publish(afterEvent);

        logger.debug("活塞{}完成 - 位置: ({}, {}, {})", extending ? "伸展" : "收缩", pos->x, pos->y, pos->z);
    } else {
        origin(region);
    }
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class BlockPistonBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, BlockPistonBeforeEvent> {
    ll::memory::HookRegistrar<PistonBlockEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<BlockPistonBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class BlockPistonAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, BlockPistonAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<BlockPistonAfterEventEmitter>();
}

} // namespace Catalyst