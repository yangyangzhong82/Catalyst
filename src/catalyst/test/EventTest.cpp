#include "catalyst/event/actor/MobPlaceBlockEvent.h"
#include "catalyst/event/actor/MobTakeBlockEvent.h"
#include "catalyst/event/actor/player/PlayerArmorStandSwapItemEvent.h"
#include "catalyst/event/actor/player/PlayerAttackBlockEvent.h"
#include "catalyst/event/actor/player/PlayerChangeDimensionEvent.h"
#include "catalyst/event/actor/player/PlayerCompleteUsingItemEvent.h"
#include "catalyst/event/actor/player/PlayerDropItemEvent.h"
#include "catalyst/event/actor/player/PlayerEditSignEvent.h"
#include "catalyst/event/actor/player/PlayerItemTransferEvent.h"
#include "catalyst/event/actor/player/PlayerOpenContainerEvent.h"
#include "catalyst/event/actor/player/PlayerShieldBlockEvent.h"
#include "catalyst/event/actor/player/PlayerUseFrameBlockEvent.h"

#include "fmt/format.h"


#include "catalyst/event/server/ClientLoginEvent.h"
#include "catalyst/event/server/ReceivePacketEvent.h"
#include "catalyst/event/world/block/BlockExplodedEvent.h"
#include "catalyst/event/world/block/BlockFallEvent.h"
#include "catalyst/event/world/block/BlockPistonEvent.h"
#include "catalyst/event/world/block/FireBurnBlockEvent.h"
#include "catalyst/event/world/block/FireSpreadEvent.h"
#include "catalyst/event/world/block/MossGrowthEvent.h"
#include "catalyst/event/world/block/NetherPortalCreateEvent.h"
#include "catalyst/event/world/block/RedstoneUpdateEvent.h"
#include "catalyst/mod/Gloabl.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/event/Listener.h"
#include "mc/network/MinecraftPacketIds.h"
#include "mc/network/NetworkIdentifier.h"
#include "mc/network/Packet.h"
#include "mc/server/ServerPlayer.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/level/Block/Block.h"

namespace Catalyst::Test {

void registerEventTests() {
    auto& bus = ll::event::EventBus::getInstance();

    bus.emplaceListener<PlayerEditSignBeforeEvent>([](PlayerEditSignBeforeEvent& event) {
        logger.info(
            "PlayerEditSignBeforeEvent: player={}, pos=({},{},{}), oldFront='{}', newFront='{}'",
            event.self().getRealName(),
            event.pos().x,
            event.pos().y,
            event.pos().z,
            event.oldFrontText(),
            event.newFrontText()
        );
    });

    bus.emplaceListener<PlayerEditSignAfterEvent>([](PlayerEditSignAfterEvent& event) {
        logger.info(
            "PlayerEditSignAfterEvent: player={}, pos=({},{},{})",
            event.self().getRealName(),
            event.pos().x,
            event.pos().y,
            event.pos().z
        );
    });

    bus.emplaceListener<PlayerAttackBlockBeforeEvent>([](PlayerAttackBlockBeforeEvent& event) {
        logger.info(
            "PlayerAttackBlockBeforeEvent: player={}, pos=({},{},{}), face={}",
            event.self().getRealName(),
            event.pos().x,
            event.pos().y,
            event.pos().z,
            event.face()
        );
    });

    bus.emplaceListener<PlayerAttackBlockAfterEvent>([](PlayerAttackBlockAfterEvent& event) {
        logger.info(
            "PlayerAttackBlockAfterEvent: player={}, pos=({},{},{})",
            event.self().getRealName(),
            event.pos().x,
            event.pos().y,
            event.pos().z
        );
    });

    bus.emplaceListener<PlayerChangeDimensionBeforeEvent>([](PlayerChangeDimensionBeforeEvent& event) {
        logger.info(
            "PlayerChangeDimensionBeforeEvent: player={}, from={}, to={}, respawn={}, usePortal={}",
            event.self().getRealName(),
            event.fromDimensionId(),
            event.toDimensionId(),
            event.isRespawn(),
            event.isUsePortal()
        );
    });

    bus.emplaceListener<PlayerChangeDimensionAfterEvent>([](PlayerChangeDimensionAfterEvent& event) {
        logger.info(
            "PlayerChangeDimensionAfterEvent: player={}, from={}, to={}, respawn={}, usePortal={}",
            event.self().getRealName(),
            event.fromDimensionId(),
            event.toDimensionId(),
            event.isRespawn(),
            event.isUsePortal()
        );
    });

    bus.emplaceListener<PlayerDropItemBeforeEvent>([](PlayerDropItemBeforeEvent& event) {
        logger.info(
            "PlayerDropItemBeforeEvent: player={}, item={}, slot={}",
            event.self().getRealName(),
            event.item().getTypeName(),
            event.slot()
        );
    });

    bus.emplaceListener<PlayerDropItemAfterEvent>([](PlayerDropItemAfterEvent& event) {
        logger.info(
            "PlayerDropItemAfterEvent: player={}, item={}, slot={}",
            event.self().getRealName(),
            event.item().getTypeName(),
            event.slot()
        );
    });

    bus.emplaceListener<PlayerArmorStandSwapItemBeforeEvent>([](PlayerArmorStandSwapItemBeforeEvent& event) {
        logger.info(
            "PlayerArmorStandSwapItemBeforeEvent: player={}, slot={}",
            event.self().getRealName(),
            static_cast<int>(event.slot())
        );
    });

    bus.emplaceListener<PlayerArmorStandSwapItemAfterEvent>([](PlayerArmorStandSwapItemAfterEvent& event) {
        logger.info(
            "PlayerArmorStandSwapItemAfterEvent: player={}, slot={}",
            event.self().getRealName(),
            static_cast<int>(event.slot())
        );
    });

    bus.emplaceListener<PlayerCompleteUsingItemBeforeEvent>([](PlayerCompleteUsingItemBeforeEvent& event) {
        logger.info(
            "PlayerCompleteUsingItemBeforeEvent: player={}, item={}",
            event.self().getRealName(),
            event.item().getTypeName()
        );
        event.cancel();
    });

    bus.emplaceListener<PlayerCompleteUsingItemAfterEvent>([](PlayerCompleteUsingItemAfterEvent& event) {
        logger.info(
            "PlayerCompleteUsingItemAfterEvent: player={}, item={}",
            event.self().getRealName(),
            event.item().getTypeName()
        );
    });

    bus.emplaceListener<PlayerShieldBlockBeforeEvent>([](PlayerShieldBlockBeforeEvent& event) {
        auto* damager = event.damager();
        logger.info(
            "PlayerShieldBlockBeforeEvent: player={}, damage={}, damager={}",
            event.self().getRealName(),
            event.damage(),
            damager ? damager->getNameTag() : "<null>"
        );
    });

    bus.emplaceListener<PlayerShieldBlockAfterEvent>([](PlayerShieldBlockAfterEvent& event) {
        auto* damager = event.damager();
        logger.info(
            "PlayerShieldBlockAfterEvent: player={}, damage={}, damager={}, result={}",
            event.self().getRealName(),
            event.damage(),
            damager ? damager->getNameTag() : "<null>",
            event.result()
        );
    });

    bus.emplaceListener<PlayerUseFrameBlockBeforeEvent>([](PlayerUseFrameBlockBeforeEvent& event) {
        logger.info(
            "PlayerUseFrameBlockBeforeEvent: player={}, pos=({},{},{})",
            event.self().getRealName(),
            event.pos().x,
            event.pos().y,
            event.pos().z
        );
    });

    bus.emplaceListener<PlayerUseFrameBlockAfterEvent>([](PlayerUseFrameBlockAfterEvent& event) {
        logger.info(
            "PlayerUseFrameBlockAfterEvent: player={}, pos=({},{},{})",
            event.self().getRealName(),
            event.pos().x,
            event.pos().y,
            event.pos().z
        );
    });

    bus.emplaceListener<BlockPistonBeforeEvent>([](BlockPistonBeforeEvent& event) {
        logger.info(
            "BlockPistonBeforeEvent: pos=({},{},{}), action={}, direction={}",
            event.pos().x,
            event.pos().y,
            event.pos().z,
            event.action() == PistonAction::Extend ? "Extend" : "Retract",
            event.direction()
        );

        if (event.pos().x == -10 && event.pos().z == 85) {
            logger.warn("鍙栨秷娲诲鍔ㄤ綔 - 娴嬭瘯浣嶇疆");
            event.cancel();
        }
    });

    bus.emplaceListener<BlockPistonAfterEvent>([](BlockPistonAfterEvent& event) {
        logger.info(
            "BlockPistonAfterEvent: pos=({},{},{}), action={}, direction={}",
            event.pos().x,
            event.pos().y,
            event.pos().z,
            event.action() == PistonAction::Extend ? "Extend" : "Retract",
            event.direction()
        );
    });


    bus.emplaceListener<NetherPortalCreateBeforeEvent>([](NetherPortalCreateBeforeEvent& event) {
        logger.info(
            "NetherPortalCreateBeforeEvent: pos=({},{},{}), radius={}",
            event.pos().x,
            event.pos().y,
            event.pos().z,
            event.radius()
        );
    });

    bus.emplaceListener<NetherPortalCreateAfterEvent>([](NetherPortalCreateAfterEvent& event) {
        logger.info(
            "NetherPortalCreateAfterEvent: pos=({},{},{}), radius={}",
            event.pos().x,
            event.pos().y,
            event.pos().z,
            event.radius()
        );
    });

    bus.emplaceListener<MossGrowthBeforeEvent>([](MossGrowthBeforeEvent& event) {
        logger.info(
            "MossGrowthBeforeEvent: origin=({},{},{}), xRadius={}, zRadius={}",
            event.origin().x,
            event.origin().y,
            event.origin().z,
            event.xRadius(),
            event.zRadius()
        );
        event.cancel();
    });

    bus.emplaceListener<MossGrowthAfterEvent>([](MossGrowthAfterEvent& event) {
        logger.info(
            "MossGrowthAfterEvent: origin=({},{},{}), xRadius={}, zRadius={}",
            event.origin().x,
            event.origin().y,
            event.origin().z,
            event.xRadius(),
            event.zRadius()
        );
    });

    bus.emplaceListener<MobPlaceBlockBeforeEvent>([](MobPlaceBlockBeforeEvent& event) {
        auto& pos = event.pos();
        logger.info(
            "MobPlaceBlockBeforeEvent: mob={}, pos=({},{},{}), block={}",
            event.self().getTypeName(),
            pos.x,
            pos.y,
            pos.z,
            event.block().getTypeName()
        );

        // 鍦?(0,0,0) 鍒?(50,80,50) 鑼冨洿鍐呮嫤鎴?
        if (pos.x >= 0 && pos.x <= 50 && pos.y >= -64 && pos.y <= 80 && pos.z >= 0 && pos.z <= 50) {
            logger.warn("鎷︽埅鐢熺墿鏀剧疆鏂瑰潡 - 娴嬭瘯淇濇姢鍖哄煙");
            event.cancel();
        }
    });

    bus.emplaceListener<MobPlaceBlockAfterEvent>([](MobPlaceBlockAfterEvent& event) {
        logger.info(
            "MobPlaceBlockAfterEvent: mob={}, pos=({},{},{}), block={}",
            event.self().getTypeName(),
            event.pos().x,
            event.pos().y,
            event.pos().z,
            event.block().getTypeName()
        );
    });

    bus.emplaceListener<MobTakeBlockBeforeEvent>([](MobTakeBlockBeforeEvent& event) {
        auto& pos = event.pos();
        logger.info(
            "MobTakeBlockBeforeEvent: mob={}, pos=({},{},{}), block={}",
            event.self().getTypeName(),
            pos.x,
            pos.y,
            pos.z,
            event.block().getTypeName()
        );

        // 鍦?(100,0,100) 鍒?(150,80,150) 鑼冨洿鍐呮嫤鎴?
        if (pos.x >= 100 && pos.x <= 150 && pos.y >= -64 && pos.y <= 80 && pos.z >= 100 && pos.z <= 150) {
            logger.warn("鎷︽埅鐢熺墿鎷惧彇鏂瑰潡 - 娴嬭瘯淇濇姢鍖哄煙");
            event.cancel();
        }
    });

    bus.emplaceListener<MobTakeBlockAfterEvent>([](MobTakeBlockAfterEvent& event) {
        logger.info(
            "MobTakeBlockAfterEvent: mob={}, pos=({},{},{}), block={}",
            event.self().getTypeName(),
            event.pos().x,
            event.pos().y,
            event.pos().z,
            event.block().getTypeName()
        );
    });

    bus.emplaceListener<BlockExplodedBeforeEvent>([](BlockExplodedBeforeEvent& event) {
        logger.info("BlockExplodedBeforeEvent: pos=({},{},{})", event.pos().x, event.pos().y, event.pos().z);
        event.cancel();
    });

    bus.emplaceListener<BlockExplodedAfterEvent>([](BlockExplodedAfterEvent& event) {
        logger.info("BlockExplodedAfterEvent: pos=({},{},{})", event.pos().x, event.pos().y, event.pos().z);
    });

    bus.emplaceListener<BlockFallBeforeEvent>([](BlockFallBeforeEvent& event) {
        auto& pos = event.pos();
        logger.info(
            "BlockFallBeforeEvent: pos=({},{},{}), block={}, creative={}",
            pos.x,
            pos.y,
            pos.z,
            event.block().getTypeName(),
            event.isCreative()
        );

        // 鍦?y >= 100 鐨勯珮搴︽嫤鎴柟鍧楀潬钀斤紙娴嬭瘯鎮诞鏂瑰潡锛?
        if (pos.y >= 100) {
            logger.warn("鎷︽埅鏂瑰潡鍧犺惤 - 娴嬭瘯楂樺害淇濇姢锛坹 >= 100锛?);
            event.cancel();
        }

        // 鍦ㄧ壒瀹氬尯鍩熸嫤鎴紙渚嬪锛歺: -50~-40, z: -50~-40锛?
        if (pos.x >= -50 && pos.x <= -40 && pos.z >= -50 && pos.z <= -40) {
            logger.warn("鎷︽埅鏂瑰潡鍧犺惤 - 娴嬭瘯鍖哄煙淇濇姢");
            event.cancel();
        }
    });

    bus.emplaceListener<BlockFallAfterEvent>([](BlockFallAfterEvent& event) {
        logger.info(
            "BlockFallAfterEvent: pos=({},{},{}), block={}, creative={}",
            event.pos().x,
            event.pos().y,
            event.pos().z,
            event.block().getTypeName(),
            event.isCreative()
        );
    });

    bus.emplaceListener<ClientLoginBeforeEvent>([](ClientLoginBeforeEvent& event) {
        logger.info("ClientLoginBeforeEvent: source={}", event.source().toString());
    });

    bus.emplaceListener<ClientLoginAfterEvent>([](ClientLoginAfterEvent& event) {
        logger.info(
            "ClientLoginAfterEvent: name='{}', ip='{}', uuid='{}', deviceId='{}', deviceModel='{}', xuid='{}'/'{}'",
            event.name(),
            event.ip(),
            event.uuid().asString(),
            event.deviceId(),
            event.deviceModel(),
            event.serverAuthXuid(),
            event.clientAuthXuid()
        );
    });

    bus.emplaceListener<RedstoneUpdateBeforeEvent>([](RedstoneUpdateBeforeEvent& event) {
        logger.info(
            "RedstoneUpdateBeforeEvent: pos=({},{},{}), strength={}, isFirstTime={}",
            event.pos().x,
            event.pos().y,
            event.pos().z,
            event.strength(),
            event.isFirstTime()
        );
        if (event.pos().x == 100 && event.pos().y >= -50 && event.pos().z == 100) {
            logger.warn("鎷︽埅鐗瑰畾浣嶇疆鐨勭孩鐭虫洿鏂颁簨浠?);
            event.cancel();
        }
    });

    bus.emplaceListener<RedstoneUpdateAfterEvent>([](RedstoneUpdateAfterEvent& event) {
        logger.info(
            "RedstoneUpdateAfterEvent: pos=({},{},{}), strength={}, isFirstTime={}",
            event.pos().x,
            event.pos().y,
            event.pos().z,
            event.strength(),
            event.isFirstTime()
        );
    });

    bus.emplaceListener<PlayerItemTransferBeforeEvent>([](PlayerItemTransferBeforeEvent& event) {
        auto actionStr = [](ItemStackRequestActionType type) -> std::string_view {
            switch (type) {
            case ItemStackRequestActionType::Take:
                return "Take";
            case ItemStackRequestActionType::Place:
                return "Place";
            case ItemStackRequestActionType::Swap:
                return "Swap";
            default:
                return "Unknown";
            }
        };

        std::string containerPosStr = "N/A";
        if (auto pos = event.getContainerBlockPos()) {
            containerPosStr = fmt::format("({},{},{})", pos->x, pos->y, pos->z);
        }

        logger.info(
            "PlayerItemTransferBeforeEvent: player={}, action={}, srcSlot={}, dstSlot={}, amount={}, srcItem={}, "
            "dstItem={}, containerPos={}",
            event.self().getRealName(),
            actionStr(event.actionType()),
            event.srcSlot(),
            event.dstSlot(),
            event.amount(),
            event.srcItem().getTypeName(),
            event.dstItem().getTypeName(),
            containerPosStr
        );
    });

    bus.emplaceListener<PlayerItemTransferAfterEvent>([](PlayerItemTransferAfterEvent& event) {
        auto actionStr = [](ItemStackRequestActionType type) -> std::string_view {
            switch (type) {
            case ItemStackRequestActionType::Take:
                return "Take";
            case ItemStackRequestActionType::Place:
                return "Place";
            case ItemStackRequestActionType::Swap:
                return "Swap";
            default:
                return "Unknown";
            }
        };

        std::string containerPosStr = "N/A";
        if (auto pos = event.getContainerBlockPos()) {
            containerPosStr = fmt::format("({},{},{})", pos->x, pos->y, pos->z);
        }

        logger.info(
            "PlayerItemTransferAfterEvent: player={}, action={}, srcSlot={}, dstSlot={}, amount={}, srcItem={}, "
            "dstItem={}, containerPos={}",
            event.self().getRealName(),
            actionStr(event.actionType()),
            event.srcSlot(),
            event.dstSlot(),
            event.amount(),
            event.srcItem().getTypeName(),
            event.dstItem().getTypeName(),
            containerPosStr
        );
    });

    bus.emplaceListener<FireBurnBlockBeforeEvent>([](FireBurnBlockBeforeEvent& event) {
        logger.info(
            "FireBurnBlockBeforeEvent: burnPos=({},{},{}), firePos=({},{},{}), age={}",
            event.burnPos().x,
            event.burnPos().y,
            event.burnPos().z,
            event.firePos().x,
            event.firePos().y,
            event.firePos().z,
            event.age()
        );
        // 濡傛灉鏂瑰潡鍦?y=100 浠ヤ笂锛屾嫤鎴儳姣?
        if (event.burnPos().y > 100) {
            logger.warn("鎷︽埅鐏劙鐑ф瘉 - 楂樺害淇濇姢");
            event.cancel();
        }
    });

    bus.emplaceListener<FireBurnBlockAfterEvent>([](FireBurnBlockAfterEvent& event) {
        logger.info(
            "FireBurnBlockAfterEvent: burnPos=({},{},{}), firePos=({},{},{}), age={}",
            event.burnPos().x,
            event.burnPos().y,
            event.burnPos().z,
            event.firePos().x,
            event.firePos().y,
            event.firePos().z,
            event.age()
        );
    });

    bus.emplaceListener<FireSpreadBeforeEvent>([](FireSpreadBeforeEvent& event) {
        logger.info(
            "FireSpreadBeforeEvent: spreadPos=({},{},{}), firePos=({},{},{}), newAge={}, sourceAge={}",
            event.spreadPos().x,
            event.spreadPos().y,
            event.spreadPos().z,
            event.firePos().x,
            event.firePos().y,
            event.firePos().z,
            event.newAge(),
            event.sourceAge()
        );
        // 濡傛灉鏂瑰潡鍦?y=100 浠ヤ笂锛屾嫤鎴敁寤?
        if (event.spreadPos().y > 100) {
            logger.warn("鎷︽埅鐏劙钄撳欢 - 楂樺害淇濇姢");
            event.cancel();
        }
    });

    bus.emplaceListener<FireSpreadAfterEvent>([](FireSpreadAfterEvent& event) {
        logger.info(
            "FireSpreadAfterEvent: spreadPos=({},{},{}), firePos=({},{},{}), newAge={}, sourceAge={}",
            event.spreadPos().x,
            event.spreadPos().y,
            event.spreadPos().z,
            event.firePos().x,
            event.firePos().y,
            event.firePos().z,
            event.newAge(),
            event.sourceAge()
        );
    });
}

} // namespace Catalyst::Test

