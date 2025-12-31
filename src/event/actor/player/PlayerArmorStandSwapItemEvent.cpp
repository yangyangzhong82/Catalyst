#include "PlayerArmorStandSwapItemEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/ArmorStand.h"
#include "mc/world/actor/player/Player.h"

namespace Catalyst {

LL_TYPE_INSTANCE_HOOK(
    ArmorStandSwapItemHook,
    HookPriority::Normal,
    ArmorStand,
    &ArmorStand::_trySwapItem,
    bool,
    Player&                              player,
    ::SharedTypes::Legacy::EquipmentSlot slot
) {
    auto& bus = ll::event::EventBus::getInstance();

    PlayerArmorStandSwapItemBeforeEvent beforeEvent(player, *this, slot);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return false;
    }

    bool result = origin(player, slot);

    if (result) {
        PlayerArmorStandSwapItemAfterEvent afterEvent(player, *this, slot);
        bus.publish(afterEvent);
    }
    return result;
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class PlayerArmorStandSwapItemBeforeEventEmitter
: public ll::event::Emitter<beforeEmitterFactory, PlayerArmorStandSwapItemBeforeEvent> {
    ll::memory::HookRegistrar<ArmorStandSwapItemHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<PlayerArmorStandSwapItemBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class PlayerArmorStandSwapItemAfterEventEmitter
: public ll::event::Emitter<afterEmitterFactory, PlayerArmorStandSwapItemAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<PlayerArmorStandSwapItemAfterEventEmitter>();
}

} // namespace Catalyst