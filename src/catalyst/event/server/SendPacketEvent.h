#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/Event.h"
#include "mc/common/SubClientId.h"
#include "catalyst/Macros.h"
class ServerPlayer;
class Packet;
class NetworkIdentifier;
namespace Catalyst {

class CATALYST_API SendPacketBeforeEvent final : public ll::event::Cancellable<ll::event::Event> {
    NetworkIdentifier const& mNetworkId;
    Packet const&            mPacket;
    SubClientId              mSubClientId;
    ServerPlayer*                  mPlayer;

public:
    constexpr SendPacketBeforeEvent(
        NetworkIdentifier const& networkId,
        Packet const&            packet,
        SubClientId              subClientId,
        ServerPlayer*                  player
    )
    : Cancellable(),
      mNetworkId(networkId),
      mPacket(packet),
      mSubClientId(subClientId),
      mPlayer(player) {}

    void serialize(CompoundTag&) const override;

    NetworkIdentifier const& networkId() const { return mNetworkId; }
    Packet const&            packet() const { return mPacket; }
    SubClientId              subClientId() const { return mSubClientId; }
    ServerPlayer*                  player() const { return mPlayer; }
};

class CATALYST_API SendPacketAfterEvent final : public ll::event::Event {
    NetworkIdentifier const& mNetworkId;
    Packet const&            mPacket;
    SubClientId              mSubClientId;
    ServerPlayer*                  mPlayer;

public:
    constexpr SendPacketAfterEvent(
        NetworkIdentifier const& networkId,
        Packet const&            packet,
        SubClientId              subClientId,
        ServerPlayer*                  player
    )
    : mNetworkId(networkId),
      mPacket(packet),
      mSubClientId(subClientId),
      mPlayer(player) {}

    void serialize(CompoundTag&) const override;

    NetworkIdentifier const& networkId() const { return mNetworkId; }
    Packet const&            packet() const { return mPacket; }
    SubClientId              subClientId() const { return mSubClientId; }
    ServerPlayer*                  player() const { return mPlayer; }
};

} // namespace Catalyst