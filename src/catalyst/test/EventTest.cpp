#include "catalyst/event/actor/player/PlayerArmorStandSwapItemEvent.h"
#include "catalyst/event/actor/player/PlayerAttackBlockEvent.h"
#include "catalyst/event/actor/player/PlayerChangeDimensionEvent.h"
#include "catalyst/event/actor/player/PlayerCompleteUsingItemEvent.h"
#include "catalyst/event/actor/player/PlayerDropItemEvent.h"
#include "catalyst/event/actor/player/PlayerEditSignEvent.h"
#include "catalyst/event/actor/player/PlayerUseFrameBlockEvent.h"

#include "catalyst/mod/Gloabl.h"
#include "catalyst/event/server/ReceivePacketEvent.h"
#include "catalyst/event/world/block/BlockPistonEvent.h"
#include "catalyst/event/world/block/MossGrowthEvent.h"
#include "catalyst/event/world/block/NetherPortalCreateEvent.h"
#include "catalyst/event/world/block/RedstoneUpdateEvent.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/event/Listener.h"
#include "mc/network/MinecraftPacketIds.h"
#include "mc/network/NetworkIdentifier.h"
#include "mc/network/Packet.h"
#include "mc/server/ServerPlayer.h"


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
            logger.warn("取消活塞动作 - 测试位置");
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
}

} // namespace Catalyst::Test