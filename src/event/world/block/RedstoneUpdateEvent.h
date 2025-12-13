#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/redstone/circuit/components/BaseCircuitComponent.h"

#include "Macros.h"

namespace Catalyst {

class CATALYST_API RedstoneUpdateBeforeEvent final : public ll::event::Cancellable<ll::event::world::WorldEvent> {
    BlockPos const&       mPos;
    int                   mStrength;
    bool                  mIsFirstTime;
    BaseCircuitComponent* mComponent;

public:
    constexpr RedstoneUpdateBeforeEvent(
        BlockSource&          region,
        BlockPos const&       pos,
        int                   strength,
        bool                  isFirstTime,
        BaseCircuitComponent* component
    )
    : Cancellable(region),
      mPos(pos),
      mStrength(strength),
      mIsFirstTime(isFirstTime),
      mComponent(component) {}

    BlockPos const&       pos() const { return mPos; }
    int                   strength() const { return mStrength; }
    bool                  isFirstTime() const { return mIsFirstTime; }
    BaseCircuitComponent* component() const { return mComponent; }
};

class CATALYST_API RedstoneUpdateAfterEvent final : public ll::event::world::WorldEvent {
    BlockPos const&       mPos;
    int                   mStrength;
    bool                  mIsFirstTime;
    BaseCircuitComponent* mComponent;

public:
    constexpr RedstoneUpdateAfterEvent(
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

    BlockPos const&       pos() const { return mPos; }
    int                   strength() const { return mStrength; }
    bool                  isFirstTime() const { return mIsFirstTime; }
    BaseCircuitComponent* component() const { return mComponent; }
};

} // namespace Catalyst