#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/Event.h"
#include "mc/network/NetworkIdentifierWithSubId.h"
#include "mc/network/Packet.h"
#include "mc/server/ServerPlayer.h"

#include "Macros.h"

namespace Catalyst {

class CATALYST_API SendMultiplePacketBeforeEvent final : public ll::event::Cancellable<ll::event::Event> {
    std::vector<NetworkIdentifierWithSubId> const& mIds;
    Packet const&                                  mPacket;
    std::vector<ServerPlayer*>                     mPlayers;

public:
    SendMultiplePacketBeforeEvent(
        std::vector<NetworkIdentifierWithSubId> const& ids,
        Packet const&                                  packet,
        std::vector<ServerPlayer*>                     players
    )
    : Cancellable(),
      mIds(ids),
      mPacket(packet),
      mPlayers(std::move(players)) {}

    std::vector<NetworkIdentifierWithSubId> const& ids() const { return mIds; }
    Packet const&                                  packet() const { return mPacket; }
    std::vector<ServerPlayer*> const&              players() const { return mPlayers; }
};

} // namespace Catalyst