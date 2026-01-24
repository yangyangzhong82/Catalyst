#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/PlayerEvent.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API PlayerStopSleepBeforeEvent final : public ll::event::Cancellable<ll::event::PlayerEvent> {
    bool mForcefulWakeUp;
    bool mUpdateLevelList;

public:
    constexpr PlayerStopSleepBeforeEvent(Player& player, bool forcefulWakeUp, bool updateLevelList)
    : Cancellable(player),
      mForcefulWakeUp(forcefulWakeUp),
      mUpdateLevelList(updateLevelList) {}

    bool isForcefulWakeUp() const { return mForcefulWakeUp; }
    bool isUpdateLevelList() const { return mUpdateLevelList; }
};

class CATALYST_API PlayerStopSleepAfterEvent final : public ll::event::PlayerEvent {
    bool mForcefulWakeUp;
    bool mUpdateLevelList;

public:
    constexpr PlayerStopSleepAfterEvent(Player& player, bool forcefulWakeUp, bool updateLevelList)
    : PlayerEvent(player),
      mForcefulWakeUp(forcefulWakeUp),
      mUpdateLevelList(updateLevelList) {}

    bool isForcefulWakeUp() const { return mForcefulWakeUp; }
    bool isUpdateLevelList() const { return mUpdateLevelList; }
};

} // namespace Catalyst
