#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"


#include "catalyst/Macros.h"

class ItemStack;

namespace Catalyst {

class CATALYST_API PlayerCompleteUsingItemEvent : public ll::event::PlayerEvent {
    ItemStack const& mItem;

public:
    constexpr PlayerCompleteUsingItemEvent(Player& player, ItemStack const& item)
    : PlayerEvent(player),
      mItem(item) {}

    void serialize(CompoundTag&) const override;

    ItemStack const& item() const { return mItem; }
};

class CATALYST_API PlayerCompleteUsingItemBeforeEvent final
: public ll::event::Cancellable<PlayerCompleteUsingItemEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API PlayerCompleteUsingItemAfterEvent final : public PlayerCompleteUsingItemEvent {
public:
    using PlayerCompleteUsingItemEvent::PlayerCompleteUsingItemEvent;
};

} // namespace Catalyst