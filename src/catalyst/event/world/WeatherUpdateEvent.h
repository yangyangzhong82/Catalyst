#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/LevelEvent.h"


#include "catalyst/Macros.h"

class Level;

namespace Catalyst {

class CATALYST_API WeatherUpdateEvent : public ll::event::world::LevelEvent {
    float mRainLevel;
    int   mRainTime;
    float mLightningLevel;
    int   mLightningTime;

public:
    constexpr WeatherUpdateEvent(
        Level& level,
        float  rainLevel,
        int    rainTime,
        float  lightningLevel,
        int    lightningTime
    )
    : LevelEvent(level),
      mRainLevel(rainLevel),
      mRainTime(rainTime),
      mLightningLevel(lightningLevel),
      mLightningTime(lightningTime) {}

    void serialize(CompoundTag&) const override;

    float  rainLevel() const { return mRainLevel; }
    float& rainLevel() { return mRainLevel; }
    int    rainTime() const { return mRainTime; }
    int&   rainTime() { return mRainTime; }
    float  lightningLevel() const { return mLightningLevel; }
    float& lightningLevel() { return mLightningLevel; }
    int    lightningTime() const { return mLightningTime; }
    int&   lightningTime() { return mLightningTime; }
};

class CATALYST_API WeatherUpdateBeforeEvent final : public ll::event::Cancellable<WeatherUpdateEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API WeatherUpdateAfterEvent final : public WeatherUpdateEvent {
public:
    using WeatherUpdateEvent::WeatherUpdateEvent;
};

} // namespace Catalyst
