#include "WeatherUpdateEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/Level.h"

#include "mc/deps/nbt/CompoundTag.h"

namespace Catalyst {

void WeatherUpdateBeforeEvent::serialize(CompoundTag& nbt) const {
    Cancellable::serialize(nbt);
    nbt["rainLevel"]      = rainLevel();
    nbt["rainTime"]       = rainTime();
    nbt["lightningLevel"] = lightningLevel();
    nbt["lightningTime"]  = lightningTime();
}

void WeatherUpdateAfterEvent::serialize(CompoundTag& nbt) const {
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

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class WeatherUpdateBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, WeatherUpdateBeforeEvent> {
    ll::memory::HookRegistrar<WeatherUpdateHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<WeatherUpdateBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class WeatherUpdateAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, WeatherUpdateAfterEvent> {
    ll::memory::HookRegistrar<WeatherUpdateHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<WeatherUpdateAfterEventEmitter>();
}

} // namespace Catalyst
