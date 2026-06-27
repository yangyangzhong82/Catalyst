#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/ServerPlayerEvent.h"
#include "mc/deps/shared_types/legacy/ContainerType.h"
#include "mc/world/ContainerID.h"

#include "catalyst/Macros.h"
class ServerPlayer;
namespace Catalyst {

class CATALYST_API PlayerCloseContainerEvent : public ll::event::ServerPlayerEvent {
    ContainerID                        mContainerId;
    SharedTypes::Legacy::ContainerType mContainerType;
    bool                               mServerInitiatedClose;

public:
    constexpr PlayerCloseContainerEvent(
        ServerPlayer&                      player,
        ContainerID                        containerId,
        SharedTypes::Legacy::ContainerType containerType,
        bool                               serverInitiatedClose
    )
    : ServerPlayerEvent(player),
      mContainerId(containerId),
      mContainerType(containerType),
      mServerInitiatedClose(serverInitiatedClose) {}

    void serialize(CompoundTag&) const override;

    ContainerID                        containerId() const { return mContainerId; }
    SharedTypes::Legacy::ContainerType containerType() const { return mContainerType; }
    bool                               serverInitiatedClose() const { return mServerInitiatedClose; }
};

class CATALYST_API PlayerCloseContainerBeforeEvent final : public ll::event::Cancellable<PlayerCloseContainerEvent> {
public:
    using Cancellable::Cancellable;
};

class CATALYST_API PlayerCloseContainerAfterEvent final : public PlayerCloseContainerEvent {
public:
    using PlayerCloseContainerEvent::PlayerCloseContainerEvent;
};

} // namespace Catalyst
