#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/world/WorldEvent.h"
#include "mc/world/level/BlockPos.h"

#include "catalyst/Macros.h"

class Block;

namespace Catalyst {

class CATALYST_API IceBlockMeltBeforeEvent final : public ll::event::Cancellable<ll::event::world::WorldEvent> {
    BlockPos       mPos;
    ::Block const& mSourceBlock;
    ::Block const& mMeltedBlock;
    bool           mInNether;

public:
    constexpr IceBlockMeltBeforeEvent(
        BlockSource&      region,
        BlockPos const&   pos,
        ::Block const&    sourceBlock,
        ::Block const&    meltedBlock,
        bool              inNether
    )
    : Cancellable(region),
      mPos(pos),
      mSourceBlock(sourceBlock),
      mMeltedBlock(meltedBlock),
      mInNether(inNether) {}

    BlockPos const& pos() const { return mPos; }
    ::Block const&  sourceBlock() const { return mSourceBlock; }
    ::Block const&  meltedBlock() const { return mMeltedBlock; }
    bool            isInNether() const { return mInNether; }
};

class CATALYST_API IceBlockMeltAfterEvent final : public ll::event::world::WorldEvent {
    BlockPos       mPos;
    ::Block const& mSourceBlock;
    ::Block const& mMeltedBlock;
    bool           mInNether;

public:
    constexpr IceBlockMeltAfterEvent(
        BlockSource&      region,
        BlockPos const&   pos,
        ::Block const&    sourceBlock,
        ::Block const&    meltedBlock,
        bool              inNether
    )
    : WorldEvent(region),
      mPos(pos),
      mSourceBlock(sourceBlock),
      mMeltedBlock(meltedBlock),
      mInNether(inNether) {}

    BlockPos const& pos() const { return mPos; }
    ::Block const&  sourceBlock() const { return mSourceBlock; }
    ::Block const&  meltedBlock() const { return mMeltedBlock; }
    bool            isInNether() const { return mInNether; }
};

} // namespace Catalyst
