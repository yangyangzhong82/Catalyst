#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"

#include "catalyst/Macros.h"

class Actor;
class Vec3;

namespace Catalyst {

class CATALYST_API PlayerInteractEntityBeforeEvent final : public ll::event::Cancellable<ll::event::PlayerEvent> {
    Actor&      mActor;
    Vec3 const& mLocation;

public:
    constexpr PlayerInteractEntityBeforeEvent(Player& player, Actor& actor, Vec3 const& location)
    : Cancellable(player),
      mActor(actor),
      mLocation(location) {}

    void serialize(CompoundTag&) const override;

    Actor&      actor() const { return mActor; }
    Vec3 const& location() const { return mLocation; }
};

class CATALYST_API PlayerInteractEntityAfterEvent final : public ll::event::PlayerEvent {
    Actor&      mActor;
    Vec3 const& mLocation;
    bool        mResult;

public:
    constexpr PlayerInteractEntityAfterEvent(Player& player, Actor& actor, Vec3 const& location, bool result)
    : PlayerEvent(player),
      mActor(actor),
      mLocation(location),
      mResult(result) {}

    void serialize(CompoundTag&) const override;

    Actor&      actor() const { return mActor; }
    Vec3 const& location() const { return mLocation; }
    bool        result() const { return mResult; }
};

} // namespace Catalyst
