#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/ServerPlayerEvent.h"
#include "mc/world/level/BlockPos.h"

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API PlayerEditSignBeforeEvent final : public ll::event::Cancellable<ll::event::ServerPlayerEvent> {
    BlockPos    mPos;
    std::string mOldFrontText;
    std::string mOldBackText;
    std::string mNewFrontText;
    std::string mNewBackText;

public:
    constexpr PlayerEditSignBeforeEvent(
        ServerPlayer& player,
        BlockPos      pos,
        std::string   oldFrontText,
        std::string   oldBackText,
        std::string   newFrontText,
        std::string   newBackText
    )
    : Cancellable(player),
      mPos(pos),
      mOldFrontText(std::move(oldFrontText)),
      mOldBackText(std::move(oldBackText)),
      mNewFrontText(std::move(newFrontText)),
      mNewBackText(std::move(newBackText)) {}

    BlockPos const&    pos() const { return mPos; }
    std::string const& oldFrontText() const { return mOldFrontText; }
    std::string const& oldBackText() const { return mOldBackText; }
    std::string const& newFrontText() const { return mNewFrontText; }
    std::string const& newBackText() const { return mNewBackText; }
};

class CATALYST_API PlayerEditSignAfterEvent final : public ll::event::ServerPlayerEvent {
    BlockPos    mPos;
    std::string mOldFrontText;
    std::string mOldBackText;
    std::string mNewFrontText;
    std::string mNewBackText;

public:
    constexpr PlayerEditSignAfterEvent(
        ServerPlayer& player,
        BlockPos      pos,
        std::string   oldFrontText,
        std::string   oldBackText,
        std::string   newFrontText,
        std::string   newBackText
    )
    : ServerPlayerEvent(player),
      mPos(pos),
      mOldFrontText(std::move(oldFrontText)),
      mOldBackText(std::move(oldBackText)),
      mNewFrontText(std::move(newFrontText)),
      mNewBackText(std::move(newBackText)) {}

    BlockPos const&    pos() const { return mPos; }
    std::string const& oldFrontText() const { return mOldFrontText; }
    std::string const& oldBackText() const { return mOldBackText; }
    std::string const& newFrontText() const { return mNewFrontText; }
    std::string const& newBackText() const { return mNewBackText; }
};

} // namespace Catalyst