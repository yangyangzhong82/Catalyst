#include "PlayerItemTransferEvent.h"

#include "catalyst/event/EmitterRegistration.h"
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

#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void PlayerItemTransferEvent::serialize(CompoundTag& nbt) const {
    ll::event::PlayerEvent::serialize(nbt);
    nbt["actionType"]    = magic_enum::enum_name(actionType());
    nbt["srcContainer"]  = ll::event::serializeRefObj(srcContainer());
    nbt["srcSlot"]       = (int)srcSlot();
    nbt["dstContainer"]  = ll::event::serializeRefObj(dstContainer());
    nbt["dstSlot"]       = (int)dstSlot();
    nbt["amount"]        = (int)amount();
    nbt["srcItem"]       = ll::event::serializeRefObj(srcItem());
    nbt["dstItem"]       = ll::event::serializeRefObj(dstItem());
    nbt["screenContext"] = ll::event::serializeRefObj(screenContext());
}


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

CATALYST_HOOKED_EVENT_PAIR(
    PlayerItemTransferBeforeEvent,
    PlayerItemTransferAfterEvent,
    PlayerItemTransferEventHook
)

} // namespace Catalyst
