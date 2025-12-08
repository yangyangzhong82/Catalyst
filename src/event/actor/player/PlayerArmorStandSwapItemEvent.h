#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"
#include "mc/deps/shared_types/legacy/item/EquipmentSlot.h"

#include "Macros.h"

class ArmorStand;

namespace Catalyst {

class CATALYST_API PlayerArmorStandSwapItemBeforeEvent final : public ll::event::Cancellable<ll::event::PlayerEvent> {
    ArmorStand&                        mArmorStand;
    SharedTypes::Legacy::EquipmentSlot mSlot;

public:
    constexpr PlayerArmorStandSwapItemBeforeEvent(
        Player&                            player,
        ArmorStand&                        armorStand,
        SharedTypes::Legacy::EquipmentSlot slot
    )
    : Cancellable(player),
      mArmorStand(armorStand),
      mSlot(slot) {}

    ArmorStand&                        armorStand() const { return mArmorStand; }
    SharedTypes::Legacy::EquipmentSlot slot() const { return mSlot; }
};

class CATALYST_API PlayerArmorStandSwapItemAfterEvent final : public ll::event::PlayerEvent {
    ArmorStand&                        mArmorStand;
    SharedTypes::Legacy::EquipmentSlot mSlot;

public:
    constexpr PlayerArmorStandSwapItemAfterEvent(
        Player&                            player,
        ArmorStand&                        armorStand,
        SharedTypes::Legacy::EquipmentSlot slot
    )
    : PlayerEvent(player),
      mArmorStand(armorStand),
      mSlot(slot) {}

    ArmorStand&                        armorStand() const { return mArmorStand; }
    SharedTypes::Legacy::EquipmentSlot slot() const { return mSlot; }
};

} // namespace Catalyst