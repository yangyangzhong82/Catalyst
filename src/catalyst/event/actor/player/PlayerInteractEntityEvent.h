#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"
#include "mc/world/gamemode/InteractionResult.h"

#include "catalyst/Macros.h"

class Actor;
class Vec3;

namespace Catalyst {

class CATALYST_API PlayerInteractEntityEvent : public ll::event::PlayerEvent {
    Actor&      mActor;
    Vec3 const& mLocation;

public:
    constexpr PlayerInteractEntityEvent(Player& player, Actor& actor, Vec3 const& location)
    : PlayerEvent(player),
      mActor(actor),
      mLocation(location) {}

    void serialize(CompoundTag&) const override;

    Actor&      actor() const { return mActor; }
    Vec3 const& location() const { return mLocation; }
};

class CATALYST_API PlayerInteractEntityBeforeEvent final : public ll::event::Cancellable<PlayerInteractEntityEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API PlayerInteractEntityAfterEvent final : public PlayerInteractEntityEvent {
    InteractionResult mResult;

public:
    constexpr PlayerInteractEntityAfterEvent(
        Player&           player,
        Actor&            actor,
        Vec3 const&       location,
        InteractionResult result
    )
    : PlayerInteractEntityEvent(player, actor, location),
      mResult(result) {}

    void serialize(CompoundTag&) const override;

    InteractionResult result() const { return mResult; }
};

} // namespace Catalyst
