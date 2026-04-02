#include "SendMultiplePacketEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "ll/api/service/Bedrock.h"
#include "mc/network/NetworkSystem.h"
#include "mc/network/ServerNetworkHandler.h"
#include "mc/server/ServerPlayer.h"
#include "mc/network/NetworkIdentifierWithSubId.h"
#include "mc/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void SendMultiplePacketBeforeEvent::serialize(CompoundTag& nbt) const {
    Cancellable::serialize(nbt);
    nbt["ids"]    = ll::event::serializeRefObj(ids());
    nbt["packet"] = ll::event::serializeRefObj(packet());
    ListTag playersList;
    for (auto* p : players()) {
        playersList.emplace_back(ll::event::serializePtrObj(p));
    }
    nbt["players"] = std::move(playersList);
}


LL_TYPE_INSTANCE_HOOK(
    SendMultiplePacketEventHook,
    ll::memory::HookPriority::Normal,
    NetworkSystem,
    &NetworkSystem::sendToMultiple,
    void,
    std::vector<NetworkIdentifierWithSubId> const& ids,
    Packet const&                                  packet
) {
    auto& bus            = ll::event::EventBus::getInstance();
    auto  networkHandler = ll::service::getServerNetworkHandler();

    std::vector<ServerPlayer*> players;
    if (networkHandler) {
        players.reserve(ids.size());
        for (auto const& idWithSub : ids) {
            players.push_back(networkHandler->_getServerPlayer(idWithSub.id, idWithSub.subClientId));
        }
    }

    SendMultiplePacketBeforeEvent beforeEvent(ids, packet, std::move(players));
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(ids, packet);
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory();
class SendMultiplePacketBeforeEventEmitter : public ll::event::Emitter<emitterFactory, SendMultiplePacketBeforeEvent> {
    ll::memory::HookRegistrar<SendMultiplePacketEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> emitterFactory() {
    return std::make_unique<SendMultiplePacketBeforeEventEmitter>();
}

} // namespace Catalyst