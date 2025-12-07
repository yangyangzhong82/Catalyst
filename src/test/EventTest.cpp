#include "event/actor/player/PlayerEditSignEvent.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/event/Listener.h"
#include "mod/Gloabl.h"

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
}

} // namespace Catalyst::Test