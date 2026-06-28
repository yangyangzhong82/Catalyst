#include "SendPacketEvent.h"

#include "catalyst/mod/Gloabl.h"
#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "ll/api/service/Bedrock.h"
#include "mc/network/NetworkSystem.h"
#include "mc/network/ServerNetworkHandler.h"
#include "mc/server/ServerPlayer.h"


#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void SendPacketEvent::serialize(CompoundTag& nbt) const {
    ll::event::Event::serialize(nbt);
    nbt["networkId"]   = ll::event::serializeRefObj(networkId());
    nbt["packet"]      = ll::event::serializeRefObj(packet());
    nbt["subClientId"] = (int)subClientId();
    nbt["player"]      = ll::event::serializePtrObj(player());
}


LL_TYPE_INSTANCE_HOOK(
    SendPacketEventHook,
    ll::memory::HookPriority::Normal,
    NetworkSystem,
    &NetworkSystem::send,
    void,
    ::NetworkIdentifier const& id,
    ::Packet const&            packet,
    ::SubClientId              recipientSubId
) {
    auto& bus            = ll::event::EventBus::getInstance();
    auto  networkHandler = ll::service::getServerNetworkHandler();
    auto  player         = networkHandler ? networkHandler->_getServerPlayer(id, recipientSubId) : nullptr;

    SendPacketBeforeEvent beforeEvent(id, packet, recipientSubId, player);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }
    origin(id, packet, recipientSubId);

    SendPacketAfterEvent afterEvent(id, packet, recipientSubId, player);
    bus.publish(afterEvent);
}

CATALYST_HOOKED_EVENT_PAIR(
    SendPacketBeforeEvent,
    SendPacketAfterEvent,
    SendPacketEventHook
)

} // namespace Catalyst
