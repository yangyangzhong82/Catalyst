
#include "PlayerEditSignEvent.h"

#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/network/ServerNetworkHandler.h"
#include "mc/network/packet/BlockActorDataPacket.h"
#include "mc/server/ServerPlayer.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/block/actor/SignBlockActor.h"

#include "mod/Gloabl.h"
namespace Catalyst {
LL_AUTO_TYPE_INSTANCE_HOOK(
    PlayerEditSignEventHook,
    HookPriority::Normal,
    ServerNetworkHandler,
    &ServerNetworkHandler::$handle,
    void,
    NetworkIdentifier const&              source,
    std::shared_ptr<BlockActorDataPacket> packet
) {
    {
        logger.debug("PlayerEditSignEventHook::hook: 尝试获取 NBT 数据.");
        auto& nbtData = packet->mData.get();
        if (nbtData.contains("id", Tag::Type::String)) {
            std::string id = nbtData["id"].get<StringTag>();
            logger.debug("PlayerEditSignEventHook::hook: NBT 数据包含 'id' 字段, id: {}", id);
            if (id == "Sign" || id == "HangingSign") {
                logger.debug("PlayerEditSignEventHook::hook: 'id' 为 'Sign' 或 'HangingSign'.");
                auto* player = thisFor<NetEventCallback>()->_getServerPlayer(source, packet->mSenderSubId);
                if (player) {
                    logger.debug("PlayerEditSignEventHook::hook: 成功获取到玩家对象.");
                    auto* signActor =
                        static_cast<SignBlockActor*>(player->getDimensionBlockSource().getBlockEntity(packet->mPos));
                    if (signActor) {
                        logger.debug("PlayerEditSignEventHook::hook: 成功获取到告示牌实体.");
                        const auto& oldFrontText = signActor->mTextFront->getMessage();
                        const auto& oldBackText  = signActor->mTextBack->getMessage();
                        logger.debug(
                            "PlayerEditSignEventHook::hook: 旧的前面文本: '{}', 旧的背面文本: '{}'",
                            oldFrontText,
                            oldBackText
                        );

                        const auto& newFrontTextData = nbtData["FrontText"].get<CompoundTag>();
                        const auto& newBackTextData  = nbtData["BackText"].get<CompoundTag>();

                        std::string newFrontText = newFrontTextData["Text"].get<StringTag>();
                        std::string newBackText  = newBackTextData["Text"].get<StringTag>();
                        logger.debug(
                            "PlayerEditSignEventHook::hook: 新的前面文本: '{}', 新的背面文本: '{}'",
                            newFrontText,
                            newBackText
                        );

                        PlayerEditSignEvent
                            event(*player, packet->mPos, oldFrontText, oldBackText, newFrontText, newBackText);
                        ll::event::EventBus::getInstance().publish(event);

                        origin(source, packet);
                        logger.debug("PlayerEditSignEventHook::hook: PlayerEditSignEvent 发布完成.");
                        return;
                    } else {
                        logger.warn("PlayerEditSignEventHook::hook: 未能获取到告示牌实体.");
                    }
                } else {
                    logger.warn("PlayerEditSignEventHook::hook: 未能获取到玩家对象.");
                }
            } else {
                logger.debug("PlayerEditSignEventHook::hook: 'id' 不是 'Sign' 或 'HangingSign'.");
            }
        } else {
            logger.debug("PlayerEditSignEventHook::hook: NBT 数据不包含 'id' 字段.");
        }

        logger.debug("PlayerEditSignEventHook::hook: 最终调用原始处理函数 (如果之前没有返回).");
        origin(source, packet);
    }
}
} // namespace Catalyst