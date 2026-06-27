#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/Event.h"
#include "mc/common/SubClientId.h"

class Player;
class Packet;
class NetworkIdentifier;

#include "catalyst/Macros.h"

namespace Catalyst {

class CATALYST_API ReceivePacketEvent : public ll::event::Event {
    NetworkIdentifier const& mNetworkId;
    Packet const&            mPacket;
    SubClientId              mSubClientId;
    Player*                  mPlayer;

public:
    constexpr ReceivePacketEvent(
        NetworkIdentifier const& networkId,
        Packet const&            packet,
        SubClientId              subClientId,
        Player*                  player
    )
    : mNetworkId(networkId),
      mPacket(packet),
      mSubClientId(subClientId),
      mPlayer(player) {}

    void serialize(CompoundTag&) const override;

    NetworkIdentifier const& networkId() const { return mNetworkId; }
    Packet const&            packet() const { return mPacket; }
    SubClientId              subClientId() const { return mSubClientId; }
    Player*                  player() const { return mPlayer; }
};

class CATALYST_API ReceivePacketBeforeEvent final : public ll::event::Cancellable<ReceivePacketEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API ReceivePacketAfterEvent final : public ReceivePacketEvent {
public:
    using ReceivePacketEvent::ReceivePacketEvent;
};

} // namespace Catalyst
