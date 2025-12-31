
#include "PlayerEditSignEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/network/ServerNetworkHandler.h"
#include "mc/network/packet/BlockActorDataPacket.h"
#include "mc/server/ServerPlayer.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/block/actor/SignBlockActor.h"

#include "catalyst/mod/Gloabl.h"

namespace Catalyst {

LL_TYPE_INSTANCE_HOOK(
    PlayerEditSignEventHook,
    HookPriority::Normal,
    ServerNetworkHandler,
    &ServerNetworkHandler::$handle,
    void,
    NetworkIdentifier const&              source,
    std::shared_ptr<BlockActorDataPacket> packet
) {
    auto& nbtData = packet->mData.get();
    if (!nbtData.contains("id", Tag::Type::String)) {
        return origin(source, packet);
    }

    std::string id = nbtData["id"].get<StringTag>();
    if (id != "Sign" && id != "HangingSign") {
        return origin(source, packet);
    }

    auto* player = thisFor<NetEventCallback>()->_getServerPlayer(source, packet->mSenderSubId);
    if (!player) {
        return origin(source, packet);
    }

    auto* signActor = static_cast<SignBlockActor*>(player->getDimensionBlockSource().getBlockEntity(packet->mPos));
    if (!signActor) {
        return origin(source, packet);
    }

    const auto& oldFrontText     = signActor->mTextFront->getMessage();
    const auto& oldBackText      = signActor->mTextBack->getMessage();
    const auto& newFrontTextData = nbtData["FrontText"].get<CompoundTag>();
    const auto& newBackTextData  = nbtData["BackText"].get<CompoundTag>();
    std::string newFrontText     = newFrontTextData["Text"].get<StringTag>();
    std::string newBackText      = newBackTextData["Text"].get<StringTag>();

    if (oldFrontText == newFrontText && oldBackText == newBackText) {
        return origin(source, packet);
    }

    auto& bus = ll::event::EventBus::getInstance();

    PlayerEditSignBeforeEvent beforeEvent(*player, packet->mPos, oldFrontText, oldBackText, newFrontText, newBackText);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(source, packet);

    PlayerEditSignAfterEvent afterEvent(*player, packet->mPos, oldFrontText, oldBackText, newFrontText, newBackText);
    bus.publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class PlayerEditSignBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, PlayerEditSignBeforeEvent> {
    ll::memory::HookRegistrar<PlayerEditSignEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<PlayerEditSignBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class PlayerEditSignAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, PlayerEditSignAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<PlayerEditSignAfterEventEmitter>();
}

} // namespace Catalyst