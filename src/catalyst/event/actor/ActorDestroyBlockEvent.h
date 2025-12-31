#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/ActorEvent.h"
#include "mc/deps/core/math/Vec3.h"
#include "mc/world/level/block/Block.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API ActorDestroyBlockBeforeEvent final : public ll::event::Cancellable<ll::event::entity::ActorEvent> {
    Vec3 const&  mPos;
    Block const& mBlock;

public:
    constexpr ActorDestroyBlockBeforeEvent(Actor& actor, Vec3 const& pos, Block const& block)
    : Cancellable(actor),
      mPos(pos),
      mBlock(block) {}

    Vec3 const&  pos() const { return mPos; }
    Block const& block() const { return mBlock; }
};

class CATALYST_API ActorDestroyBlockAfterEvent final : public ll::event::entity::ActorEvent {
    Vec3 const&  mPos;
    Block const& mBlock;

public:
    constexpr ActorDestroyBlockAfterEvent(Actor& actor, Vec3 const& pos, Block const& block)
    : ActorEvent(actor),
      mPos(pos),
      mBlock(block) {}

    Vec3 const&  pos() const { return mPos; }
    Block const& block() const { return mBlock; }
};

} // namespace Catalyst
