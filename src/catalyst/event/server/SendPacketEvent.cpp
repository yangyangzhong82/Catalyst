#include "SendPacketEvent.h"

#include "catalyst/mod/Gloabl.h"
#include "ll/api/event/Emitter.h"
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

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class SendPacketBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, SendPacketBeforeEvent> {
    ll::memory::HookRegistrar<SendPacketEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<SendPacketBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class SendPacketAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, SendPacketAfterEvent> {
    ll::memory::HookRegistrar<SendPacketEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<SendPacketAfterEventEmitter>();
}

} // namespace Catalyst
