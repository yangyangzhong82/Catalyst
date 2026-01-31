#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/Event.h"
#include "ll/api/service/Bedrock.h"
#include "mc/network/ServerNetworkHandler.h"
#include "mc/network/connection/DisconnectFailReason.h"
#include "mc/network/packet/LoginPacket.h"


#include "catalyst/Macros.h"
class NetworkIdentifier;
namespace Catalyst {

class CATALYST_API ClientLoginBeforeEvent final : public ll::event::Cancellable<ll::event::Event> {
    NetworkIdentifier const&     mSource;
    std::shared_ptr<LoginPacket> mPacket;

public:
    ClientLoginBeforeEvent(NetworkIdentifier const& source, std::shared_ptr<LoginPacket> packet)
    : Cancellable(),
      mSource(source),
      mPacket(std::move(packet)) {}

    NetworkIdentifier const&     source() const { return mSource; }
    std::shared_ptr<LoginPacket> packet() const { return mPacket; }
};

class CATALYST_API ClientLoginAfterEvent final : public ll::event::Event {
    std::shared_ptr<LoginPacket> mPacket;
    std::string                  mName;
    std::string                  mIp;
    std::string                  mServerAuthXuid;
    std::string                  mClientAuthXuid;
    ::mce::UUID                  mUuid;
    std::string                  mDeviceId;
    uint64                       mNetworkIdHash;

public:
    ClientLoginAfterEvent(
        std::shared_ptr<LoginPacket> packet,
        std::string                  name,
        std::string                  ip,
        std::string                  serverAuthXuid,
        std::string                  clientAuthXuid,
        ::mce::UUID                  uuid,
        std::string                  deviceId,
        uint64                       networkIdHash
    )
    : Event(),
      mPacket(std::move(packet)),
      mName(std::move(name)),
      mIp(std::move(ip)),
      mServerAuthXuid(std::move(serverAuthXuid)),
      mClientAuthXuid(std::move(clientAuthXuid)),
      mUuid(std::move(uuid)),
      mDeviceId(std::move(deviceId)),
      mNetworkIdHash(networkIdHash) {}

    std::shared_ptr<LoginPacket> packet() const { return mPacket; }
    std::string const&           name() const { return mName; }
    std::string const&           ip() const { return mIp; }
    std::string const&           serverAuthXuid() const { return mServerAuthXuid; }
    std::string const&           clientAuthXuid() const { return mClientAuthXuid; }
    ::mce::UUID const&           uuid() const { return mUuid; }
    std::string const&           deviceId() const { return mDeviceId; }

    void disconnect(std::string const& message) const;
};

} // namespace Catalyst
