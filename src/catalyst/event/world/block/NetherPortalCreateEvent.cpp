#include "NetherPortalCreateEvent.h"

#include "catalyst/mod/Gloabl.h"
#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/PortalForcer.h"


namespace Catalyst {

LL_AUTO_TYPE_INSTANCE_HOOK(
    NetherPortalCreateEventHook,
    ll::memory::HookPriority::Normal,
    PortalForcer,
    &PortalForcer::createPortal,
    ::PortalRecord const&,
    ::Actor const& entity,
    int            radius
) {
    auto& bus = ll::event::EventBus::getInstance();
    auto  pos = entity.getPosition();


    NetherPortalCreateBeforeEvent beforeEvent(entity.getDimensionBlockSource(), pos, radius);
    bus.publish(beforeEvent);

    if (beforeEvent.isCancelled()) {
        static PortalRecord emptyRecord{};
        return emptyRecord;
    }


    auto const& result = origin(entity, radius);


    NetherPortalCreateAfterEvent afterEvent(entity.getDimensionBlockSource(), pos, radius);
    bus.publish(afterEvent);

    return result;
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class NetherPortalCreateBeforeEventEmitter
: public ll::event::Emitter<beforeEmitterFactory, NetherPortalCreateBeforeEvent> {
    ll::memory::HookRegistrar<NetherPortalCreateEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<NetherPortalCreateBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class NetherPortalCreateAfterEventEmitter
: public ll::event::Emitter<afterEmitterFactory, NetherPortalCreateAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<NetherPortalCreateAfterEventEmitter>();
}

} // namespace Catalyst