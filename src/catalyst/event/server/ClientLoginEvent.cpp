#include "ClientLoginEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "ll/api/service/Bedrock.h"
#include "mc/certificates/identity/PlayerAuthenticationInfo.h"
#include "mc/common/SubClientId.h"
#include "mc/deps/identity/edu_common/MessToken.h"
#include "mc/deps/raknet/RakPeer.h"
#include "mc/network/ConnectionRequest.h"
#include "mc/network/NetEventCallback.h"
#include "mc/network/NetworkIdentifier.h"
#include "mc/network/ServerNetworkHandler.h"
#include "mc/network/connection/DisconnectFailReason.h"
#include "mc/network/packet/LoginPacket.h"
#include "mc/network/packet/SetLocalPlayerAsInitializedPacket.h"
#include "mc/server/ServerPlayer.h"
#include "mc/world/actor/player/Player.h"

#include <unordered_map>

#ifdef LL_PLAT_S

#include "mc/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void ClientLoginBeforeEvent::serialize(CompoundTag& nbt) const {
    Cancellable::serialize(nbt);
    nbt["source"] = ll::event::serializeRefObj(source());
    nbt["packet"] = ll::event::serializePtrObj(packet().get());
}

void ClientLoginAfterEvent::serialize(CompoundTag& nbt) const {
    ll::event::Event::serialize(nbt);
    nbt["packet"]         = ll::event::serializePtrObj(packet().get());
    nbt["name"]           = name();
    nbt["ip"]             = ip();
    nbt["serverAuthXuid"] = serverAuthXuid();
    nbt["clientAuthXuid"] = clientAuthXuid();
    nbt["uuid"]           = ll::event::serializeRefObj(mUuid);
    nbt["deviceId"]       = deviceId();
    nbt["deviceModel"]    = deviceModel();
    nbt["networkIdHash"]  = (int64_t)mNetworkIdHash;
}


// Thread-local pointer to current NetworkIdentifier during event dispatch
static thread_local NetworkIdentifier const* sCurrentNetworkId = nullptr;

// Directly read IP from RakPeer's internal data, bypassing GetSystemAddressFromGuid's lock
static std::string getIPFromRakPeerUnsafe(RakNet::RakNetGUID const& guid) {
    auto peer = ll::service::getRakPeer();
    if (!peer) return {};

    // Iterate activeSystemList to find the matching GUID
    auto** activeList = peer->activeSystemList;
    uint   listSize   = peer->activeSystemListSize;

    for (uint i = 0; i < listSize; ++i) {
        auto* remote = activeList[i];
        if (!remote || !remote->isActive) continue;

        RakNet::RakNetGUID const& remoteGuid = remote->guid;
        if (remoteGuid.g == guid.g) {
            RakNet::SystemAddress const& addr = remote->systemAddress;
            return addr.GetIPString();
        }
    }
    return {};
}

// Before event hook - on $handle(LoginPacket)
LL_TYPE_INSTANCE_HOOK(
    ClientLoginBeforeEventHook,
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
}

// Capture player info and fire AfterEvent during async authorization
LL_TYPE_INSTANCE_HOOK(
    ClientLoginAfterEventHook,
    ll::memory::HookPriority::Normal,
    ServerNetworkHandler,
    &ServerNetworkHandler::$_onClientAsyncAuthorized,
    void,
    NetworkIdentifier const&        source,
    ConnectionRequest const&        request,
    PlayerAuthenticationInfo const& playerInfo,
    std::optional<MessToken> const& hostMessToken
) {
    // Store current NetworkIdentifier for disconnect() to use
    sCurrentNetworkId = &source;

    auto&                 bus = ll::event::EventBus::getInstance();
    ClientLoginAfterEvent afterEvent(
        nullptr,
        std::string(playerInfo.XboxLiveName),
        getIPFromRakPeerUnsafe(source.mGuid),
        std::string(playerInfo.Xuid),
        std::string(playerInfo.Xuid),
        playerInfo.AuthenticatedUuid,
        request.getDeviceId(),
        request.getDeviceModel(),
        source.getHash()
    );
    bus.publish(afterEvent);

    sCurrentNetworkId = nullptr;

    origin(source, request, playerInfo, hostMessToken);
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class ClientLoginBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, ClientLoginBeforeEvent> {
    ll::memory::HookRegistrar<ClientLoginBeforeEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<ClientLoginBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class ClientLoginAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, ClientLoginAfterEvent> {
    ll::memory::HookRegistrar<ClientLoginAfterEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<ClientLoginAfterEventEmitter>();
}

// ClientLoginAfterEvent::disconnect implementation
void ClientLoginAfterEvent::disconnect(std::string const& message) const {
    auto handler = ll::service::getServerNetworkHandler();
    if (!handler) return;

    // If called during event dispatch, use the thread-local NetworkIdentifier directly
    if (sCurrentNetworkId && sCurrentNetworkId->getHash() == mNetworkIdHash) {
        handler->disconnectClientWithMessage(
            *sCurrentNetworkId,
            SubClientId::PrimaryClient,
            Connection::DisconnectFailReason::Kicked,
            message,
            std::nullopt,
            false
        );
        return;
    }

    // Fallback: search in mClients
    using ClientMap = std::unordered_map<NetworkIdentifier, std::unique_ptr<ServerNetworkHandler::Client>>;
    auto& clients   = const_cast<ClientMap&>(static_cast<ClientMap const&>(handler->mClients));

    for (auto& [netId, client] : clients) {
        if (netId.getHash() == mNetworkIdHash) {
            handler->disconnectClientWithMessage(
                netId,
                SubClientId::PrimaryClient,
                Connection::DisconnectFailReason::Kicked,
                message,
                std::nullopt,
                false
            );
            break;
        }
    }
}

} // namespace Catalyst

#else

namespace Catalyst {

void ClientLoginAfterEvent::disconnect(std::string const&) const {}

} // namespace Catalyst

#endif
