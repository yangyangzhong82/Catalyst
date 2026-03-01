#include "PlayerStartSleepEvent.h"

#include "catalyst/mod/Gloabl.h"
#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/player/Player.h"

namespace Catalyst {

LL_TYPE_INSTANCE_HOOK(
    PlayerStartSleepEventHook,
    ll::memory::HookPriority::Normal,
    Player,
    &Player::$startSleepInBed,
    ::BedSleepingResult,
    ::BlockPos const& bedBlockPos
) {
    auto& bus           = ll::event::EventBus::getInstance();
    bool  canStartSleep = this->canStartSleepInBed();
    if (!canStartSleep) {
        return origin(bedBlockPos);
    }
    logger.info(
        "PlayerStartSleepEvent: player={}, bedPos=({},{},{})",
        this->getRealName(),
        bedBlockPos.x,
        bedBlockPos.y,
        bedBlockPos.z
    );
    PlayerStartSleepBeforeEvent beforeEvent(*this, bedBlockPos);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return BedSleepingResult::OtherProblem;
    }

    BedSleepingResult result = origin(bedBlockPos);

    PlayerStartSleepAfterEvent afterEvent(*this, bedBlockPos, result);
    bus.publish(afterEvent);

    return result;
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class PlayerStartSleepBeforeEventEmitter
: public ll::event::Emitter<beforeEmitterFactory, PlayerStartSleepBeforeEvent> {
    ll::memory::HookRegistrar<PlayerStartSleepEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<PlayerStartSleepBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class PlayerStartSleepAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, PlayerStartSleepAfterEvent> {
    ll::memory::HookRegistrar<PlayerStartSleepEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<PlayerStartSleepAfterEventEmitter>();
}

} // namespace Catalyst
