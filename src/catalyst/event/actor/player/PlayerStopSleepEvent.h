#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API PlayerStopSleepEvent : public ll::event::PlayerEvent {
    bool mForcefulWakeUp;
    bool mUpdateLevelList;

public:
    constexpr PlayerStopSleepEvent(Player& player, bool forcefulWakeUp, bool updateLevelList)
    : PlayerEvent(player),
      mForcefulWakeUp(forcefulWakeUp),
      mUpdateLevelList(updateLevelList) {}

    void serialize(CompoundTag&) const override;

    bool isForcefulWakeUp() const { return mForcefulWakeUp; }
    bool isUpdateLevelList() const { return mUpdateLevelList; }
};

class CATALYST_API PlayerStopSleepBeforeEvent final : public ll::event::Cancellable<PlayerStopSleepEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API PlayerStopSleepAfterEvent final : public PlayerStopSleepEvent {
public:
    using PlayerStopSleepEvent::PlayerStopSleepEvent;
};

} // namespace Catalyst
