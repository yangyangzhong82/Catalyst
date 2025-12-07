#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"

#include "Macros.h"

namespace Catalyst {

class CATALYST_API PlayerChangeDimensionBeforeEvent final : public ll::event::Cancellable<ll::event::PlayerEvent> {
    int  mFromDimensionId;
    int  mToDimensionId;
    bool mRespawn;
    bool mUsePortal;

public:
    constexpr PlayerChangeDimensionBeforeEvent(
        Player& player,
        int     fromDimensionId,
        int     toDimensionId,
        bool    respawn,
        bool    usePortal
    )
    : Cancellable(player),
      mFromDimensionId(fromDimensionId),
      mToDimensionId(toDimensionId),
      mRespawn(respawn),
      mUsePortal(usePortal) {}

    int  fromDimensionId() const { return mFromDimensionId; }
    int  toDimensionId() const { return mToDimensionId; }
    bool isRespawn() const { return mRespawn; }
    bool isUsePortal() const { return mUsePortal; }
};

class CATALYST_API PlayerChangeDimensionAfterEvent final : public ll::event::PlayerEvent {
    int  mFromDimensionId;
    int  mToDimensionId;
    bool mRespawn;
    bool mUsePortal;

public:
    constexpr PlayerChangeDimensionAfterEvent(
        Player& player,
        int     fromDimensionId,
        int     toDimensionId,
        bool    respawn,
        bool    usePortal
    )
    : PlayerEvent(player),
      mFromDimensionId(fromDimensionId),
      mToDimensionId(toDimensionId),
      mRespawn(respawn),
      mUsePortal(usePortal) {}

    int  fromDimensionId() const { return mFromDimensionId; }
    int  toDimensionId() const { return mToDimensionId; }
    bool isRespawn() const { return mRespawn; }
    bool isUsePortal() const { return mUsePortal; }
};

} // namespace Catalyst