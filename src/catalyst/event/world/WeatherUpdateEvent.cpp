#include "WeatherUpdateEvent.h"

#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/Level.h"

#include "mc/deps/nbt/CompoundTag.h"

namespace Catalyst {

void WeatherUpdateEvent::serialize(CompoundTag& nbt) const {
    ll::event::world::LevelEvent::serialize(nbt);
    nbt["rainLevel"]      = rainLevel();
    nbt["rainTime"]       = rainTime();
    nbt["lightningLevel"] = lightningLevel();
    nbt["lightningTime"]  = lightningTime();
}
LL_TYPE_INSTANCE_HOOK(
    WeatherUpdateHook,
    ll::memory::HookPriority::Normal,
    Level,
    &Level::$updateWeather,
    void,
    float rainLevel,
    int   rainTime,
    float lightningLevel,
    int   lightningTime
) {
    auto& bus = ll::event::EventBus::getInstance();

    WeatherUpdateBeforeEvent beforeEvent(*this, rainLevel, rainTime, lightningLevel, lightningTime);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(beforeEvent.rainLevel(), beforeEvent.rainTime(), beforeEvent.lightningLevel(), beforeEvent.lightningTime());

    WeatherUpdateAfterEvent afterEvent(*this, rainLevel, rainTime, lightningLevel, lightningTime);
    bus.publish(afterEvent);
}

CATALYST_HOOKED_EVENT_PAIR(
    WeatherUpdateBeforeEvent,
    WeatherUpdateAfterEvent,
    WeatherUpdateHook
)

} // namespace Catalyst
