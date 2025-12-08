#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"
#include "mc/world/item/ItemStack.h"

#include "Macros.h"

namespace Catalyst {

class CATALYST_API PlayerCompleteUsingItemBeforeEvent final : public ll::event::Cancellable<ll::event::PlayerEvent> {
    ItemStack const& mItem;

public:
    constexpr PlayerCompleteUsingItemBeforeEvent(Player& player, ItemStack const& item)
    : Cancellable(player),
      mItem(item) {}

    ItemStack const& item() const { return mItem; }
};

class CATALYST_API PlayerCompleteUsingItemAfterEvent final : public ll::event::PlayerEvent {
    ItemStack const& mItem;

public:
    constexpr PlayerCompleteUsingItemAfterEvent(Player& player, ItemStack const& item)
    : PlayerEvent(player),
      mItem(item) {}

    ItemStack const& item() const { return mItem; }
};

} // namespace Catalyst