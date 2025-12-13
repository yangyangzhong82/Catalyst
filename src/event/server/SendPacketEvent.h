#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/Event.h"
#include "mc/network/NetworkIdentifier.h"
#include "mc/network/Packet.h"
#include "mc/world/actor/player/Player.h"

#include "Macros.h"

namespace Catalyst {

class CATALYST_API SendPacketBeforeEvent final : public ll::event::Cancellable<ll::event::Event> {
    NetworkIdentifier const& mNetworkId;
    Packet const&            mPacket;
    SubClientId              mSubClientId;
    Player*                  mPlayer;

public:
    constexpr SendPacketBeforeEvent(
        NetworkIdentifier const& networkId,
        Packet const&            packet,
        SubClientId              subClientId,
        Player*                  player
    )
    : Cancellable(),
      mNetworkId(networkId),
      mPacket(packet),
      mSubClientId(subClientId),
      mPlayer(player) {}

    NetworkIdentifier const& networkId() const { return mNetworkId; }
    Packet const&            packet() const { return mPacket; }
    SubClientId              subClientId() const { return mSubClientId; }
    Player*                  player() const { return mPlayer; }
};

} // namespace Catalyst