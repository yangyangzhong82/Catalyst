
#include "PlayerEditSignEvent.h"

#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/network/ServerNetworkHandler.h"
#include "mc/network/packet/BlockActorDataPacket.h"
#include "mc/server/ServerPlayer.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/block/actor/SignBlockActor.h"

#include "catalyst/mod/Gloabl.h"

#include "mc/deps/nbt/CompoundTag.h"

namespace Catalyst {

void PlayerEditSignEvent::serialize(CompoundTag& nbt) const {
    ll::event::ServerPlayerEvent::serialize(nbt);
    nbt["pos"]          = ListTag{pos().x, pos().y, pos().z};
    nbt["oldFrontText"] = oldFrontText();
    nbt["oldBackText"]  = oldBackText();
    nbt["newFrontText"] = newFrontText();
    nbt["newBackText"]  = newBackText();
}


LL_AUTO_TYPE_INSTANCE_HOOK(
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

CATALYST_HOOKED_EVENT_PAIR(
    PlayerEditSignBeforeEvent,
    PlayerEditSignAfterEvent,
    PlayerEditSignEventHook
)

} // namespace Catalyst
