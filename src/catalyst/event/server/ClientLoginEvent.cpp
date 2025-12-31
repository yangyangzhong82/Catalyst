#include "ClientLoginEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/certificates/identity/LegacyMultiplayerToken.h"

namespace Catalyst {

LL_TYPE_INSTANCE_HOOK(
    ClientLoginEventHook,
    ll::memory::HookPriority::Normal,
    ServerNetworkHandler,
    &ServerNetworkHandler::$handle,
    void,
    NetworkIdentifier const&     source,
    std::shared_ptr<LoginPacket> packet
) {
    auto& bus = ll::event::EventBus::getInstance();

    ClientLoginBeforeEvent beforeEvent(source, packet);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(source, packet);

    auto* connReq = packet->mConnectionRequest.get();
    if (connReq) {
        ClientLoginAfterEvent afterEvent(
            source,
            packet,
            connReq->mLegacyMultiplayerToken->getIdentityName(),
            source.getIPAndPort(),
            connReq->mLegacyMultiplayerToken->getXuid(false),
            connReq->mLegacyMultiplayerToken->getXuid(true),
            connReq->mLegacyMultiplayerToken->getIdentity().asString(),
            connReq->getDeviceId()
        );
        bus.publish(afterEvent);
    }
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class ClientLoginBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, ClientLoginBeforeEvent> {
    ll::memory::HookRegistrar<ClientLoginEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<ClientLoginBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class ClientLoginAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, ClientLoginAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<ClientLoginAfterEventEmitter>();
}

} // namespace Catalyst