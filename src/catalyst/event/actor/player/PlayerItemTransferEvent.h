#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"
#include "mc/world/containers/ContainerEnumName.h"
#include "mc/world/containers/FullContainerName.h"
#include "mc/world/inventory/network/ContainerScreenContext.h"
#include "mc/world/inventory/network/ItemStackRequestActionType.h"
#include "mc/world/item/ItemStack.h"
#include "mc/world/level/BlockPos.h"

#include "catalyst/Macros.h"

class Player;

namespace Catalyst {

class CATALYST_API PlayerItemTransferBeforeEvent final : public ll::event::Cancellable<ll::event::PlayerEvent> {
    ItemStackRequestActionType mActionType;
    FullContainerName          mSrcContainer;
    uchar                      mSrcSlot;
    FullContainerName          mDstContainer;
    uchar                      mDstSlot;
    uchar                      mAmount;
    ItemStack const&           mItem;
    ContainerScreenContext     mScreenContext;

public:
    PlayerItemTransferBeforeEvent(
        Player&                       player,
        ItemStackRequestActionType    actionType,
        FullContainerName const&      srcContainer,
        uchar                         srcSlot,
        FullContainerName const&      dstContainer,
        uchar                         dstSlot,
        uchar                         amount,
        ItemStack const&              item,
        ContainerScreenContext const& screenContext
    )
    : Cancellable(player),
      mActionType(actionType),
      mSrcContainer(srcContainer),
      mSrcSlot(srcSlot),
      mDstContainer(dstContainer),
      mDstSlot(dstSlot),
      mAmount(amount),
      mItem(item),
      mScreenContext(screenContext) {}

    ItemStackRequestActionType actionType() const { return mActionType; }
    FullContainerName const&   srcContainer() const { return mSrcContainer; }
    uchar                      srcSlot() const { return mSrcSlot; }
    FullContainerName const&   dstContainer() const { return mDstContainer; }
    uchar                      dstSlot() const { return mDstSlot; }
    uchar                      amount() const { return mAmount; }
    ItemStack const&           item() const { return mItem; }

    ContainerScreenContext const& screenContext() const { return mScreenContext; }

    std::optional<BlockPos> getContainerBlockPos() const {
        auto const& owner = mScreenContext.mOwner.get();
        if (std::holds_alternative<BlockPos>(owner)) {
            return std::get<BlockPos>(owner);
        }
        return std::nullopt;
    }

    std::optional<ActorUniqueID> getContainerActorId() const {
        auto const& owner = mScreenContext.mOwner.get();
        if (std::holds_alternative<ActorUniqueID>(owner)) {
            return std::get<ActorUniqueID>(owner);
        }
        return std::nullopt;
    }
};

class CATALYST_API PlayerItemTransferAfterEvent final : public ll::event::PlayerEvent {
    ItemStackRequestActionType mActionType;
    FullContainerName          mSrcContainer;
    uchar                      mSrcSlot;
    FullContainerName          mDstContainer;
    uchar                      mDstSlot;
    uchar                      mAmount;
    ItemStack const&           mItem;
    ContainerScreenContext     mScreenContext;

public:
    PlayerItemTransferAfterEvent(
        Player&                       player,
        ItemStackRequestActionType    actionType,
        FullContainerName const&      srcContainer,
        uchar                         srcSlot,
        FullContainerName const&      dstContainer,
        uchar                         dstSlot,
        uchar                         amount,
        ItemStack const&              item,
        ContainerScreenContext const& screenContext
    )
    : PlayerEvent(player),
      mActionType(actionType),
      mSrcContainer(srcContainer),
      mSrcSlot(srcSlot),
      mDstContainer(dstContainer),
      mDstSlot(dstSlot),
      mAmount(amount),
      mItem(item),
      mScreenContext(screenContext) {}

    ItemStackRequestActionType actionType() const { return mActionType; }
    FullContainerName const&   srcContainer() const { return mSrcContainer; }
    uchar                      srcSlot() const { return mSrcSlot; }
    FullContainerName const&   dstContainer() const { return mDstContainer; }
    uchar                      dstSlot() const { return mDstSlot; }
    uchar                      amount() const { return mAmount; }
    ItemStack const&           item() const { return mItem; }

    ContainerScreenContext const& screenContext() const { return mScreenContext; }

    std::optional<BlockPos> getContainerBlockPos() const {
        auto const& owner = mScreenContext.mOwner.get();
        if (std::holds_alternative<BlockPos>(owner)) {
            return std::get<BlockPos>(owner);
        }
        return std::nullopt;
    }

    std::optional<ActorUniqueID> getContainerActorId() const {
        auto const& owner = mScreenContext.mOwner.get();
        if (std::holds_alternative<ActorUniqueID>(owner)) {
            return std::get<ActorUniqueID>(owner);
        }
        return std::nullopt;
    }
};

} // namespace Catalyst
