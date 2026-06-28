#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"
#include "mc/deps/shared_types/legacy/EquipmentSlot.h"

#include "catalyst/Macros.h"

class ArmorStand;

namespace Catalyst {

class CATALYST_API PlayerArmorStandSwapItemEvent : public ll::event::PlayerEvent {
    ArmorStand&                        mArmorStand;
    SharedTypes::Legacy::EquipmentSlot mSlot;

public:
    constexpr PlayerArmorStandSwapItemEvent(
        Player&                            player,
        ArmorStand&                        armorStand,
        SharedTypes::Legacy::EquipmentSlot slot
    )
    : PlayerEvent(player),
      mArmorStand(armorStand),
      mSlot(slot) {}

    void serialize(CompoundTag&) const override;

    ArmorStand&                        armorStand() const { return mArmorStand; }
    SharedTypes::Legacy::EquipmentSlot slot() const { return mSlot; }
};

class CATALYST_API PlayerArmorStandSwapItemBeforeEvent final
: public ll::event::Cancellable<PlayerArmorStandSwapItemEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API PlayerArmorStandSwapItemAfterEvent final : public PlayerArmorStandSwapItemEvent {
public:
    using PlayerArmorStandSwapItemEvent::PlayerArmorStandSwapItemEvent;
};

} // namespace Catalyst