#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/LevelEvent.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/level/Level.h"
#include "mc/world/level/block/actor/SculkCatalystBlockActor.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API SculkCatalystAbsorbExperienceEvent : public ll::event::world::LevelEvent {
    SculkCatalystBlockActor& mBlockActor;
    Actor&                   mActor;

public:
    constexpr SculkCatalystAbsorbExperienceEvent(SculkCatalystBlockActor& blockActor, Level& level, Actor& actor)
    : LevelEvent(level),
      mBlockActor(blockActor),
      mActor(actor) {}

    void serialize(CompoundTag&) const override;

    SculkCatalystBlockActor& blockActor() const { return mBlockActor; }
    Actor&                   actor() const { return mActor; }
};

class CATALYST_API SculkCatalystAbsorbExperienceBeforeEvent final
: public ll::event::Cancellable<SculkCatalystAbsorbExperienceEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API SculkCatalystAbsorbExperienceAfterEvent final : public SculkCatalystAbsorbExperienceEvent {
public:
    using SculkCatalystAbsorbExperienceEvent::SculkCatalystAbsorbExperienceEvent;
};

} // namespace Catalyst
