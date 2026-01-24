#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/ServerPlayerEvent.h"
#include "mc/deps/shared_types/legacy/ContainerType.h"
#include "mc/world/ContainerID.h"

#include "catalyst/Macros.h"
class ServerPlayer;
namespace Catalyst {

class CATALYST_API PlayerCloseContainerBeforeEvent final : public ll::event::Cancellable<ll::event::ServerPlayerEvent> {
    ContainerID                        mContainerId;
    SharedTypes::Legacy::ContainerType mContainerType;
    bool                               mServerInitiatedClose;

public:
    constexpr PlayerCloseContainerBeforeEvent(
        ServerPlayer&                      player,
        ContainerID                        containerId,
        SharedTypes::Legacy::ContainerType containerType,
        bool                               serverInitiatedClose
    )
    : Cancellable(player),
      mContainerId(containerId),
      mContainerType(containerType),
      mServerInitiatedClose(serverInitiatedClose) {}

    ContainerID                        containerId() const { return mContainerId; }
    SharedTypes::Legacy::ContainerType containerType() const { return mContainerType; }
    bool                               serverInitiatedClose() const { return mServerInitiatedClose; }
};

class CATALYST_API PlayerCloseContainerAfterEvent final : public ll::event::PlayerEvent {
    ContainerID                        mContainerId;
    SharedTypes::Legacy::ContainerType mContainerType;
    bool                               mServerInitiatedClose;

public:
    constexpr PlayerCloseContainerAfterEvent(
        ServerPlayer&                      player,
        ContainerID                        containerId,
        SharedTypes::Legacy::ContainerType containerType,
        bool                               serverInitiatedClose
    )
    : PlayerEvent(player),
      mContainerId(containerId),
      mContainerType(containerType),
      mServerInitiatedClose(serverInitiatedClose) {}

    ContainerID                        containerId() const { return mContainerId; }
    SharedTypes::Legacy::ContainerType containerType() const { return mContainerType; }
    bool                               serverInitiatedClose() const { return mServerInitiatedClose; }
};

} // namespace Catalyst
