#include "PlayerDropItemEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/player/Inventory.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/actor/player/PlayerInventory.h"
#include "mc/world/inventory/transaction/ComplexInventoryTransaction.h"
#include "mc/world/inventory/transaction/InventoryAction.h"
#include "mc/world/inventory/transaction/InventorySourceType.h"
#include "mc/world/inventory/transaction/InventoryTransaction.h"

namespace Catalyst {

/*
LL_AUTO_TYPE_INSTANCE_HOOK(
    PlayerDropItemEventHook1,
    HookPriority::Normal,
    Player,
    &Player::$drop,
    bool,
    ItemStack const& item,
    bool             randomly
) {
    auto& bus  = ll::event::EventBus::getInstance();
    int   slot = getSelectedItemSlot();

    PlayerDropItemBeforeEvent beforeEvent(*this, item, slot);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return false;
    }

    bool result = origin(item, randomly);

    if (result) {
        PlayerDropItemAfterEvent afterEvent(*this, item, slot);
        bus.publish(afterEvent);
    }
    return result;
}
*/
// Hook for inventory drag drop
LL_TYPE_INSTANCE_HOOK(
    PlayerDropItemEventHook2,
    HookPriority::Normal,
    ComplexInventoryTransaction,
    &ComplexInventoryTransaction::$handle,
    InventoryTransactionError,
    Player& player,
    bool    isSenderAuthority
) {
    if (mType == ComplexInventoryTransaction::Type::NormalTransaction) {
        InventorySource source{
            InventorySourceType::ContainerInventory,
            ContainerID::Inventory,
            InventorySource::InventorySourceFlags::NoFlag
        };
        auto& actions = mTransaction->getActions(source);
        if (actions.size() == 1) {
            int              slot = actions[0].mSlot;
            ItemStack const& item = player.mInventory->mInventory->getItem(slot);

            auto& bus = ll::event::EventBus::getInstance();

            PlayerDropItemBeforeEvent beforeEvent(player, item, slot);
            bus.publish(beforeEvent);
            if (beforeEvent.isCancelled()) {
                return InventoryTransactionError::AuthorityMismatch;
            }

            InventoryTransactionError result = origin(player, isSenderAuthority);

            if (result == InventoryTransactionError::NoError) {
                PlayerDropItemAfterEvent afterEvent(player, item, slot);
                bus.publish(afterEvent);
            }
            return result;
        }
    }
    return origin(player, isSenderAuthority);
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class PlayerDropItemBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, PlayerDropItemBeforeEvent> {
    ll::memory::HookRegistrar<PlayerDropItemEventHook2> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<PlayerDropItemBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class PlayerDropItemAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, PlayerDropItemAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<PlayerDropItemAfterEventEmitter>();
}

} // namespace Catalyst