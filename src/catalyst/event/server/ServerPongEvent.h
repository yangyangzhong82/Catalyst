#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "catalyst/Macros.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/Event.h"

namespace Catalyst {

class CATALYST_API ServerPongBeforeEvent final : public ll::event::Cancellable<ll::event::Event> {
    std::string              mMotd;
    int                      mProtocolVersion;
    std::string              mNetworkVersion;
    int                      mPlayerCount;
    int                      mMaxPlayerCount;
    std::string              mGuid;
    std::string              mLevelName;
    std::string              mGameMode;
    std::uint16_t            mLocalPort;
    std::uint16_t            mLocalPortV6;
    std::vector<std::string> mOther;
    std::string              mIpAndPort;

public:
    ServerPongBeforeEvent(
        std::string              motd,
        int                      protocolVersion,
        std::string              networkVersion,
        int                      playerCount,
        int                      maxPlayerCount,
        std::string              guid,
        std::string              levelName,
        std::string              gameMode,
        std::uint16_t            localPort,
        std::uint16_t            localPortV6,
        std::vector<std::string> other,
        std::string              ipAndPort
    )
    : Cancellable(),
      mMotd(std::move(motd)),
      mProtocolVersion(protocolVersion),
      mNetworkVersion(std::move(networkVersion)),
      mPlayerCount(playerCount),
      mMaxPlayerCount(maxPlayerCount),
      mGuid(std::move(guid)),
      mLevelName(std::move(levelName)),
      mGameMode(std::move(gameMode)),
      mLocalPort(localPort),
      mLocalPortV6(localPortV6),
      mOther(std::move(other)),
      mIpAndPort(std::move(ipAndPort)) {}

    void serialize(CompoundTag&) const override;

    std::string&              motd() { return mMotd; }
    std::string const&        motd() const { return mMotd; }
    int&                      protocolVersion() { return mProtocolVersion; }
    int                       protocolVersion() const { return mProtocolVersion; }
    std::string&              networkVersion() { return mNetworkVersion; }
    std::string const&        networkVersion() const { return mNetworkVersion; }
    int&                      playerCount() { return mPlayerCount; }
    int                       playerCount() const { return mPlayerCount; }
    int&                      maxPlayerCount() { return mMaxPlayerCount; }
    int                       maxPlayerCount() const { return mMaxPlayerCount; }
    std::string&              guid() { return mGuid; }
    std::string const&        guid() const { return mGuid; }
    std::string&              levelName() { return mLevelName; }
    std::string const&        levelName() const { return mLevelName; }
    std::string&              gameMode() { return mGameMode; }
    std::string const&        gameMode() const { return mGameMode; }
    std::uint16_t&            localPort() { return mLocalPort; }
    std::uint16_t             localPort() const { return mLocalPort; }
    std::uint16_t&            localPortV6() { return mLocalPortV6; }
    std::uint16_t             localPortV6() const { return mLocalPortV6; }
    std::vector<std::string>& other() { return mOther; }
    std::vector<std::string> const& other() const { return mOther; }
    std::string const&        ipAndPort() const { return mIpAndPort; }
    std::string               ip() const;
    std::uint16_t             port() const;
};

class CATALYST_API ServerPongAfterEvent final : public ll::event::Event {
    std::string              mMotd;
    int                      mProtocolVersion;
    std::string              mNetworkVersion;
    int                      mPlayerCount;
    int                      mMaxPlayerCount;
    std::string              mGuid;
    std::string              mLevelName;
    std::string              mGameMode;
    std::uint16_t            mLocalPort;
    std::uint16_t            mLocalPortV6;
    std::vector<std::string> mOther;
    std::string              mIpAndPort;

public:
    ServerPongAfterEvent(
        std::string              motd,
        int                      protocolVersion,
        std::string              networkVersion,
        int                      playerCount,
        int                      maxPlayerCount,
        std::string              guid,
        std::string              levelName,
        std::string              gameMode,
        std::uint16_t            localPort,
        std::uint16_t            localPortV6,
        std::vector<std::string> other,
        std::string              ipAndPort
    )
    : Event(),
      mMotd(std::move(motd)),
      mProtocolVersion(protocolVersion),
      mNetworkVersion(std::move(networkVersion)),
      mPlayerCount(playerCount),
      mMaxPlayerCount(maxPlayerCount),
      mGuid(std::move(guid)),
      mLevelName(std::move(levelName)),
      mGameMode(std::move(gameMode)),
      mLocalPort(localPort),
      mLocalPortV6(localPortV6),
      mOther(std::move(other)),
      mIpAndPort(std::move(ipAndPort)) {}

    void serialize(CompoundTag&) const override;

    std::string const&        motd() const { return mMotd; }
    int                       protocolVersion() const { return mProtocolVersion; }
    std::string const&        networkVersion() const { return mNetworkVersion; }
    int                       playerCount() const { return mPlayerCount; }
    int                       maxPlayerCount() const { return mMaxPlayerCount; }
    std::string const&        guid() const { return mGuid; }
    std::string const&        levelName() const { return mLevelName; }
    std::string const&        gameMode() const { return mGameMode; }
    std::uint16_t             localPort() const { return mLocalPort; }
    std::uint16_t             localPortV6() const { return mLocalPortV6; }
    std::vector<std::string> const& other() const { return mOther; }
    std::string const&        ipAndPort() const { return mIpAndPort; }
    std::string               ip() const;
    std::uint16_t             port() const;
};

} // namespace Catalyst
