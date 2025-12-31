#include "ActorDestroyBlockEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/deps/ecs/gamerefs_entity/EntityContext.h"
#include "mc/deps/game_refs/WeakRef.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/events/ActorEventCoordinator.h"
#include "mc/world/events/ActorGameplayEvent.h"
#include "mc/world/events/ActorGriefingBlockEvent.h"
#include "mod/Gloabl.h"
namespace Catalyst {

LL_TYPE_INSTANCE_HOOK(
    ActorDestroyBlockHook,
    ll::memory::HookPriority::Normal,
    ::ActorEventCoordinator,
    &::ActorEventCoordinator::sendEvent,
    ::CoordinatorResult,
    ::EventRef<::ActorGameplayEvent<::CoordinatorResult>> const& event
) {
    try {
    const ActorGriefingBlockEvent* griefingEvent = nullptr;
    event.get().visit([&](auto&& arg) {
        using CurrentEventType = std::decay_t<decltype(arg.value())>;
        if constexpr (std::is_same_v<CurrentEventType, ActorGriefingBlockEvent>) {
            griefingEvent = &arg.value();
        }
    });

    if (griefingEvent) {
        WeakRef<EntityContext> actorContextWeakRef = griefingEvent->mActorContext.get();
        auto                   actorContextResult  = actorContextWeakRef.lock();
        if (actorContextResult) {
            ::Actor* actor = Actor::tryGetFromEntity(actorContextResult.value(), false);
            if (actor) {
                auto& bus = ll::event::EventBus::getInstance();

                ActorDestroyBlockBeforeEvent beforeEvent(
                    *actor,
                    griefingEvent->mPos.get(),
                    *griefingEvent->mBlock.get()
                );
                bus.publish(beforeEvent);
                if (beforeEvent.isCancelled()) {
                    return CoordinatorResult::Cancel;
                }

                auto result = origin(event);

                ActorDestroyBlockAfterEvent afterEvent(*actor, griefingEvent->mPos.get(), *griefingEvent->mBlock.get());
                bus.publish(afterEvent);

                return result;
            }
        }
    }
    return origin(event);
    }  catch (const std::exception& e) {
        logger.warn("ActorDestroyBlockHook 发生异常: {}", e.what());
        return origin(event);
    } catch (...) {
        logger.warn("ActorDestroyBlockHook 发生未知异常！");
        return origin(event);
    }
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class ActorDestroyBlockBeforeEventEmitter
: public ll::event::Emitter<beforeEmitterFactory, ActorDestroyBlockBeforeEvent> {
    ll::memory::HookRegistrar<ActorDestroyBlockHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<ActorDestroyBlockBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class ActorDestroyBlockAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, ActorDestroyBlockAfterEvent> {
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<ActorDestroyBlockAfterEventEmitter>();
}

} // namespace Catalyst
