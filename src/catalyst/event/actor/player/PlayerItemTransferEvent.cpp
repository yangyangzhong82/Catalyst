#include "PlayerItemTransferEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/player/Inventory.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/actor/player/PlayerInventory.h"
#include "mc/world/inventory/network/ItemStackNetManagerBase.h"
#include "mc/world/inventory/network/ItemStackNetManagerServer.h"
#include "mc/world/inventory/network/ItemStackNetResult.h"
#include "mc/world/inventory/network/ItemStackRequestAction.h"
#include "mc/world/inventory/network/ItemStackRequestActionHandler.h"
#include "mc/world/inventory/network/ItemStackRequestActionTransferBase.h"
#include "mc/world/inventory/network/ItemStackRequestActionType.h"

namespace Catalyst {

LL_TYPE_INSTANCE_HOOK(
    PlayerItemTransferEventHook,
    HookPriority::Normal,
    ItemStackRequestActionHandler,
    &ItemStackRequestActionHandler::handleRequestAction,
    ItemStackNetResult,
    ItemStackRequestAction const& requestAction
) {
    auto actionType = requestAction.mActionType;

    // 只处理物品转移相关的操作
    if (actionType != ItemStackRequestActionType::Take && actionType != ItemStackRequestActionType::Place
        && actionType != ItemStackRequestActionType::Swap) {
        return origin(requestAction);
    }

    auto const& transferAction = static_cast<ItemStackRequestActionTransferBase const&>(requestAction);
    auto&       player         = mPlayer;

    // 获取屏幕上下文
    ContainerScreenContext screenContext = mItemStackNetManager.getScreenContext();

    // 获取源物品槽位信息 - 使用 .get() 访问 TypedStorage 内的数据
    auto const& srcSlotInfo = transferAction.mSrc.get();
    auto const& dstSlotInfo = transferAction.mDst.get();

    // 获取源槽位和目标槽位的物品
    ItemStack const& srcItem = player.mInventory->mInventory->getItem(srcSlotInfo.mSlot);
    ItemStack const& dstItem = player.mInventory->mInventory->getItem(dstSlotInfo.mSlot);

    auto& bus = ll::event::EventBus::getInstance();

    PlayerItemTransferBeforeEvent beforeEvent(
        player,
        actionType,
        srcSlotInfo.mFullContainerName,
        srcSlotInfo.mSlot,
        dstSlotInfo.mFullContainerName,
        dstSlotInfo.mSlot,
        transferAction.mAmount,
        srcItem,
        dstItem,
        screenContext
    );
    bus.publish(beforeEvent);

    if (beforeEvent.isCancelled()) {
        return ItemStackNetResult::Error;
    }

    auto result = origin(requestAction);

    if (result == ItemStackNetResult::Success) {
        // AfterEvent 获取转移后的物品状态
        ItemStack const& newSrcItem = player.mInventory->mInventory->getItem(srcSlotInfo.mSlot);
        ItemStack const& newDstItem = player.mInventory->mInventory->getItem(dstSlotInfo.mSlot);

        PlayerItemTransferAfterEvent afterEvent(
            player,
            actionType,
            srcSlotInfo.mFullContainerName,
            srcSlotInfo.mSlot,
            dstSlotInfo.mFullContainerName,
            dstSlotInfo.mSlot,
            transferAction.mAmount,
            newSrcItem,
            newDstItem,
            screenContext
        );
        bus.publish(afterEvent);
    }

    return result;
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class PlayerItemTransferBeforeEventEmitter
: public ll::event::Emitter<beforeEmitterFactory, PlayerItemTransferBeforeEvent> {
    ll::memory::HookRegistrar<PlayerItemTransferEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<PlayerItemTransferBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class PlayerItemTransferAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, PlayerItemTransferAfterEvent> {
    ll::memory::HookRegistrar<PlayerItemTransferEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<PlayerItemTransferAfterEventEmitter>();
}

} // namespace Catalyst
