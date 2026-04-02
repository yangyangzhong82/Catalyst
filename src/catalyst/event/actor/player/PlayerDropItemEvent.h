#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"


#include "catalyst/Macros.h"

class ItemStack;


namespace Catalyst {

class CATALYST_API PlayerDropItemBeforeEvent final : public ll::event::Cancellable<ll::event::PlayerEvent> {
    ItemStack const& mItem;
    int              mSlot;

public:
    constexpr PlayerDropItemBeforeEvent(Player& player, ItemStack const& item, int slot)
    : Cancellable(player),
      mItem(item),
      mSlot(slot) {}

    void serialize(CompoundTag&) const override;

    ItemStack const& item() const { return mItem; }
    int              slot() const { return mSlot; }
};

class CATALYST_API PlayerDropItemAfterEvent final : public ll::event::PlayerEvent {
    ItemStack const& mItem;
    int              mSlot;

public:
    constexpr PlayerDropItemAfterEvent(Player& player, ItemStack const& item, int slot)
    : PlayerEvent(player),
      mItem(item),
      mSlot(slot) {}

    void serialize(CompoundTag&) const override;

    ItemStack const& item() const { return mItem; }
    int              slot() const { return mSlot; }
};

} // namespace Catalyst