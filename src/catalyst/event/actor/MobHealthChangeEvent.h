#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/MobEvent.h"

#include "catalyst/Macros.h"

class AttributeBuff;

namespace Catalyst {

class CATALYST_API MobHealthChangeEvent : public ll::event::entity::MobEvent {
    float                mOldValue;
    float                mNewValue;
    AttributeBuff const& mBuff;

public:
    MobHealthChangeEvent(Mob& mob, float oldValue, float newValue, AttributeBuff const& buff)
    : MobEvent(mob),
      mOldValue(oldValue),
      mNewValue(newValue),
      mBuff(buff) {}

    void serialize(CompoundTag&) const override;

    float                oldValue() const { return mOldValue; }
    float                newValue() const { return mNewValue; }
    AttributeBuff const& buff() const { return mBuff; }
};

class CATALYST_API MobHealthChangeBeforeEvent final : public ll::event::Cancellable<MobHealthChangeEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API MobHealthChangeAfterEvent final : public MobHealthChangeEvent {
public:
    using MobHealthChangeEvent::MobHealthChangeEvent;
};

} // namespace Catalyst
