#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API PlayerChangeDimensionEvent : public ll::event::PlayerEvent {
    int  mFromDimensionId;
    int  mToDimensionId;
    bool mRespawn;
    bool mUsePortal;

public:
    constexpr PlayerChangeDimensionEvent(
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

    void serialize(CompoundTag&) const override;

    int  fromDimensionId() const { return mFromDimensionId; }
    int  toDimensionId() const { return mToDimensionId; }
    bool isRespawn() const { return mRespawn; }
    bool isUsePortal() const { return mUsePortal; }
};

class CATALYST_API PlayerChangeDimensionBeforeEvent final : public ll::event::Cancellable<PlayerChangeDimensionEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API PlayerChangeDimensionAfterEvent final : public PlayerChangeDimensionEvent {
public:
    using PlayerChangeDimensionEvent::PlayerChangeDimensionEvent;
};

} // namespace Catalyst