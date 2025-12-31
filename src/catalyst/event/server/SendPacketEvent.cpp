#include "SendPacketEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "ll/api/service/Bedrock.h"
#include "mc/network/NetworkSystem.h"
#include "mc/network/ServerNetworkHandler.h"
#include "mc/server/ServerPlayer.h"

namespace Catalyst {

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
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory();
class SendPacketBeforeEventEmitter : public ll::event::Emitter<emitterFactory, SendPacketBeforeEvent> {
    ll::memory::HookRegistrar<SendPacketEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> emitterFactory() {
    return std::make_unique<SendPacketBeforeEventEmitter>();
}

} // namespace Catalyst