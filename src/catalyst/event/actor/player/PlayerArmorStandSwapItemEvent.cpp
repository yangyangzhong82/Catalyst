#include "PlayerArmorStandSwapItemEvent.h"

#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/ArmorStand.h"
#include "mc/world/actor/player/Player.h"

#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void PlayerArmorStandSwapItemEvent::serialize(CompoundTag& nbt) const {
    ll::event::PlayerEvent::serialize(nbt);
    nbt["armorStand"] = ll::event::serializeRefObj(armorStand());
    nbt["slot"]       = magic_enum::enum_name(slot());
}


LL_TYPE_INSTANCE_HOOK(
    ArmorStandSwapItemHook,
    HookPriority::Normal,
    ArmorStand,
    &ArmorStand::_trySwapItem,
    bool,
    Player&                              player,
    ::SharedTypes::Legacy::EquipmentSlot slot
) {
    auto& bus = ll::event::EventBus::getInstance();

    PlayerArmorStandSwapItemBeforeEvent beforeEvent(player, *this, slot);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return false;
    }

    bool result = origin(player, slot);

    if (result) {
        PlayerArmorStandSwapItemAfterEvent afterEvent(player, *this, slot);
        bus.publish(afterEvent);
    }
    return result;
}

CATALYST_HOOKED_EVENT_PAIR(
    PlayerArmorStandSwapItemBeforeEvent,
    PlayerArmorStandSwapItemAfterEvent,
    ArmorStandSwapItemHook
)

} // namespace Catalyst
