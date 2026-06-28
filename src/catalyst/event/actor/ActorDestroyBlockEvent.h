#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"

#include "catalyst/Macros.h"

class Vec3;
class Block;

namespace Catalyst {

class CATALYST_API ActorDestroyBlockEvent : public ll::event::entity::ActorEvent {
    Vec3 const&  mPos;
    Block const& mBlock;

public:
    constexpr ActorDestroyBlockEvent(Actor& actor, Vec3 const& pos, Block const& block)
    : ActorEvent(actor),
      mPos(pos),
      mBlock(block) {}

    void serialize(CompoundTag&) const override;

    Vec3 const&  pos() const { return mPos; }
    Block const& block() const { return mBlock; }
};

class CATALYST_API ActorDestroyBlockBeforeEvent final : public ll::event::Cancellable<ActorDestroyBlockEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API ActorDestroyBlockAfterEvent final : public ActorDestroyBlockEvent {
public:
    using ActorDestroyBlockEvent::ActorDestroyBlockEvent;
};

} // namespace Catalyst
