#pragma once

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/player/ServerPlayerEvent.h"
#include "mc/deps/shared_types/legacy/ContainerType.h"
#include "mc/legacy/ActorUniqueID.h"
#include "mc/world/ContainerID.h"
#include "mc/world/level/BlockPos.h"

#include "catalyst/Macros.h"
class ServerPlayer;
namespace Catalyst {

class CATALYST_API PlayerOpenContainerBeforeEvent final : public ll::event::Cancellable<ll::event::ServerPlayerEvent> {
    ContainerID                        mContainerId;
    SharedTypes::Legacy::ContainerType mType;
    BlockPos                           mPos;
    ActorUniqueID                      mEntityUniqueID;

public:
    constexpr PlayerOpenContainerBeforeEvent(
        ServerPlayer&                            player,
        ContainerID                        containerId,
        SharedTypes::Legacy::ContainerType type,
        BlockPos const&                    pos,
        ActorUniqueID                      entityUniqueID
    )
    : Cancellable(player),
      mContainerId(containerId),
      mType(type),
      mPos(pos),
      mEntityUniqueID(entityUniqueID) {}

    void serialize(CompoundTag&) const override;

    ContainerID                        containerId() const { return mContainerId; }
    SharedTypes::Legacy::ContainerType type() const { return mType; }
    BlockPos const&                    pos() const { return mPos; }
    ActorUniqueID                      entityUniqueID() const { return mEntityUniqueID; }
};

class CATALYST_API PlayerOpenContainerAfterEvent final : public ll::event::PlayerEvent {
    ContainerID                        mContainerId;
    SharedTypes::Legacy::ContainerType mType;
    BlockPos                           mPos;
    ActorUniqueID                      mEntityUniqueID;

public:
    constexpr PlayerOpenContainerAfterEvent(
        ServerPlayer&                            player,
        ContainerID                        containerId,
        SharedTypes::Legacy::ContainerType type,
        BlockPos const&                    pos,
        ActorUniqueID                      entityUniqueID
    )
    : PlayerEvent(player),
      mContainerId(containerId),
      mType(type),
      mPos(pos),
      mEntityUniqueID(entityUniqueID) {}

    void serialize(CompoundTag&) const override;

    ContainerID                        containerId() const { return mContainerId; }
    SharedTypes::Legacy::ContainerType type() const { return mType; }
    BlockPos const&                    pos() const { return mPos; }
    ActorUniqueID                      entityUniqueID() const { return mEntityUniqueID; }
};

} // namespace Catalyst
