#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/Event.h"
#include <vector>
#include "catalyst/Macros.h"

struct NetworkIdentifierWithSubId;
class ServerPlayer;
class Packet;
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

    void serialize(CompoundTag&) const override;

    std::vector<NetworkIdentifierWithSubId> const& ids() const { return mIds; }
    Packet const&                                  packet() const { return mPacket; }
    std::vector<ServerPlayer*> const&              players() const { return mPlayers; }
};

} // namespace Catalyst