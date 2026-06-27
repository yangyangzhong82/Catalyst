#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/world/level/BlockPos.h"


#include "catalyst/Macros.h"
class BaseCircuitComponent;

namespace Catalyst {

class CATALYST_API RedstoneUpdateEvent : public ll::event::world::WorldEvent {
    BlockPos const&       mPos;
    int                   mStrength;
    bool                  mIsFirstTime;
    BaseCircuitComponent* mComponent;

public:
    constexpr RedstoneUpdateEvent(
        BlockSource&          region,
        BlockPos const&       pos,
        int                   strength,
        bool                  isFirstTime,
        BaseCircuitComponent* component
    )
    : WorldEvent(region),
      mPos(pos),
      mStrength(strength),
      mIsFirstTime(isFirstTime),
      mComponent(component) {}

    void serialize(CompoundTag&) const override;

    BlockPos const&       pos() const { return mPos; }
    int                   strength() const { return mStrength; }
    bool                  isFirstTime() const { return mIsFirstTime; }
    BaseCircuitComponent* component() const { return mComponent; }
};

class CATALYST_API RedstoneUpdateBeforeEvent final : public ll::event::Cancellable<RedstoneUpdateEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API RedstoneUpdateAfterEvent final : public RedstoneUpdateEvent {
public:
    using RedstoneUpdateEvent::RedstoneUpdateEvent;
};

} // namespace Catalyst