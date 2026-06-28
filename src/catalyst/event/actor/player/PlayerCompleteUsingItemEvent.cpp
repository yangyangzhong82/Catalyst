#include "PlayerCompleteUsingItemEvent.h"

#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/item/ItemStack.h"

#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void PlayerCompleteUsingItemEvent::serialize(CompoundTag& nbt) const {
    ll::event::PlayerEvent::serialize(nbt);
    nbt["item"] = ll::event::serializeRefObj(item());
}


LL_TYPE_INSTANCE_HOOK(
    PlayerCompleteUsingItemEventHook,
    HookPriority::Normal,
    Player,
    &Player::completeUsingItem,
    void
) {
    ItemStack const& item = getCarriedItem();
    auto&            bus  = ll::event::EventBus::getInstance();

    PlayerCompleteUsingItemBeforeEvent beforeEvent(*this, item);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin();

    PlayerCompleteUsingItemAfterEvent afterEvent(*this, item);
    bus.publish(afterEvent);
}

CATALYST_HOOKED_EVENT_PAIR(
    PlayerCompleteUsingItemBeforeEvent,
    PlayerCompleteUsingItemAfterEvent,
    PlayerCompleteUsingItemEventHook
)

} // namespace Catalyst
