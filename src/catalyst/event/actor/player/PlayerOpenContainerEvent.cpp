#include "PlayerOpenContainerEvent.h"

#include "catalyst/event/server/SendPacketEvent.h"
#include "catalyst/mod/Gloabl.h"
#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/event/Listener.h"
#include "mc/network/packet/ContainerOpenPacket.h"
#include "mc/world/actor/player/Player.h"


#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void PlayerOpenContainerBeforeEvent::serialize(CompoundTag& nbt) const {
    Cancellable::serialize(nbt);
    nbt["containerId"]    = (int)containerId();
    nbt["type"]           = magic_enum::enum_name(type());
    nbt["pos"]            = ListTag{pos().x, pos().y, pos().z};
    nbt["entityUniqueID"] = ll::event::serializeRefObj(mEntityUniqueID);
}

void PlayerOpenContainerAfterEvent::serialize(CompoundTag& nbt) const {
    ll::event::PlayerEvent::serialize(nbt);
    nbt["containerId"]    = (int)containerId();
    nbt["type"]           = magic_enum::enum_name(type());
    nbt["pos"]            = ListTag{pos().x, pos().y, pos().z};
    nbt["entityUniqueID"] = ll::event::serializeRefObj(mEntityUniqueID);
}


static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class PlayerOpenContainerBeforeEventEmitter
: public ll::event::Emitter<beforeEmitterFactory, PlayerOpenContainerBeforeEvent> {
    ll::event::ListenerPtr listener;

public:
    PlayerOpenContainerBeforeEventEmitter() {
        listener = ll::event::EventBus::getInstance().emplaceListener<SendPacketBeforeEvent>(
            [](SendPacketBeforeEvent& ev) {
                // 检查是否是ContainerOpenPacket
                if (ev.packet().getId() != MinecraftPacketIds::ContainerOpen) {
                    return;
                }
                logger.debug("捕获到ContainerOpenPacket发送事件");
                // 获取玩家
                auto player = ev.player();
                if (!player) {
                    return;
                }

                // 转换为ContainerOpenPacket
                auto& containerPacket = static_cast<ContainerOpenPacket const&>(ev.packet());

                // 发布PlayerOpenContainerBeforeEvent
                auto&                          bus = ll::event::EventBus::getInstance();
                PlayerOpenContainerBeforeEvent beforeEvent(
                    *player,
                    containerPacket.mContainerId,
                    containerPacket.mType,
                    containerPacket.mPos,
                    containerPacket.mEntityUniqueID
                );
                bus.publish(beforeEvent);

                // 如果事件被取消，则取消发包
                if (beforeEvent.isCancelled()) {
                    ev.cancel();
                    ev.player()->doDeleteContainerManager(false);
                }
            },
            ll::event::EventPriority::Normal
        );
    }

    ~PlayerOpenContainerBeforeEventEmitter() override {
        if (listener) {
            ll::event::EventBus::getInstance().removeListener(listener);
        }
    }
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<PlayerOpenContainerBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class PlayerOpenContainerAfterEventEmitter
: public ll::event::Emitter<afterEmitterFactory, PlayerOpenContainerAfterEvent> {
    ll::event::ListenerPtr listener;

public:
    PlayerOpenContainerAfterEventEmitter() {
        listener = ll::event::EventBus::getInstance().emplaceListener<SendPacketAfterEvent>(
            [](SendPacketAfterEvent& ev) {
                // 检查是否是ContainerOpenPacket
                if (ev.packet().getId() != MinecraftPacketIds::ContainerOpen) {
                    return;
                }

                // 获取玩家
                auto player = ev.player();
                if (!player) {
                    return;
                }

                // 转换为ContainerOpenPacket
                auto& containerPacket = static_cast<ContainerOpenPacket const&>(ev.packet());

                // 发布PlayerOpenContainerAfterEvent
                auto&                         bus = ll::event::EventBus::getInstance();
                PlayerOpenContainerAfterEvent afterEvent(
                    *player,
                    containerPacket.mContainerId,
                    containerPacket.mType,
                    containerPacket.mPos,
                    containerPacket.mEntityUniqueID
                );
                bus.publish(afterEvent);
            },
            ll::event::EventPriority::Normal
        );
    }

    ~PlayerOpenContainerAfterEventEmitter() override {
        if (listener) {
            ll::event::EventBus::getInstance().removeListener(listener);
        }
    }
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<PlayerOpenContainerAfterEventEmitter>();
}

} // namespace Catalyst
