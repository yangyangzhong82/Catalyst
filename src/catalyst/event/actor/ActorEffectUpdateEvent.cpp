#include "ActorEffectUpdateEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/effect/MobEffectInstance.h"
#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void ActorEffectAddBeforeEvent::serialize(CompoundTag& nbt) const {
    Cancellable::serialize(nbt);
    nbt["effect"] = ll::event::serializeRefObj(effect());
}

void ActorEffectAddAfterEvent::serialize(CompoundTag& nbt) const {
    ll::event::entity::ActorEvent::serialize(nbt);
    nbt["effect"] = ll::event::serializeRefObj(effect());
}

void ActorEffectUpdateBeforeEvent::serialize(CompoundTag& nbt) const {
    Cancellable::serialize(nbt);
    nbt["effect"] = ll::event::serializeRefObj(effect());
}

void ActorEffectUpdateAfterEvent::serialize(CompoundTag& nbt) const {
    ll::event::entity::ActorEvent::serialize(nbt);
    nbt["effect"] = ll::event::serializeRefObj(effect());
}

void ActorEffectRemoveBeforeEvent::serialize(CompoundTag& nbt) const {
    Cancellable::serialize(nbt);
    nbt["effect"] = ll::event::serializeRefObj(effect());
}

void ActorEffectRemoveAfterEvent::serialize(CompoundTag& nbt) const {
    ll::event::entity::ActorEvent::serialize(nbt);
    nbt["effect"] = ll::event::serializeRefObj(effect());
}


LL_TYPE_INSTANCE_HOOK(
    ActorEffectAddEventHook,
    ll::memory::HookPriority::Normal,
    Actor,
    &Actor::addEffect,
    void,
    ::MobEffectInstance const& effect
) {
    auto& bus = ll::event::EventBus::getInstance();

    ActorEffectAddBeforeEvent beforeEvent(*this, effect);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(effect);

    ActorEffectAddAfterEvent afterEvent(*this, effect);
    bus.publish(afterEvent);
}

LL_TYPE_INSTANCE_HOOK(
    ActorEffectUpdateEventHook,
    ll::memory::HookPriority::Normal,
    Actor,
    &Actor::onEffectUpdated,
    void,
    MobEffectInstance& effect
) {
    auto& bus = ll::event::EventBus::getInstance();

    ActorEffectUpdateBeforeEvent beforeEvent(*this, effect);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(effect);

    ActorEffectUpdateAfterEvent afterEvent(*this, effect);
    bus.publish(afterEvent);
}

LL_TYPE_INSTANCE_HOOK(
    ActorEffectRemoveEventHook,
    ll::memory::HookPriority::Normal,
    Actor,
    &Actor::$onEffectRemoved,
    void,
    ::MobEffectInstance& effect
) {
    auto& bus = ll::event::EventBus::getInstance();

    ActorEffectRemoveBeforeEvent beforeEvent(*this, effect);
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }

    origin(effect);

    ActorEffectRemoveAfterEvent afterEvent(*this, effect);
    bus.publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> addBeforeEmitterFactory();
class ActorEffectAddBeforeEventEmitter : public ll::event::Emitter<addBeforeEmitterFactory, ActorEffectAddBeforeEvent> {
    ll::memory::HookRegistrar<ActorEffectAddEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> addBeforeEmitterFactory() {
    return std::make_unique<ActorEffectAddBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> addAfterEmitterFactory();
class ActorEffectAddAfterEventEmitter : public ll::event::Emitter<addAfterEmitterFactory, ActorEffectAddAfterEvent> {
    ll::memory::HookRegistrar<ActorEffectAddEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> addAfterEmitterFactory() {
    return std::make_unique<ActorEffectAddAfterEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> updateBeforeEmitterFactory();
class ActorEffectUpdateBeforeEventEmitter
: public ll::event::Emitter<updateBeforeEmitterFactory, ActorEffectUpdateBeforeEvent> {
    ll::memory::HookRegistrar<ActorEffectUpdateEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> updateBeforeEmitterFactory() {
    return std::make_unique<ActorEffectUpdateBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> updateAfterEmitterFactory();
class ActorEffectUpdateAfterEventEmitter : public ll::event::Emitter<updateAfterEmitterFactory, ActorEffectUpdateAfterEvent> {
    ll::memory::HookRegistrar<ActorEffectUpdateEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> updateAfterEmitterFactory() {
    return std::make_unique<ActorEffectUpdateAfterEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> removeBeforeEmitterFactory();
class ActorEffectRemoveBeforeEventEmitter
: public ll::event::Emitter<removeBeforeEmitterFactory, ActorEffectRemoveBeforeEvent> {
    ll::memory::HookRegistrar<ActorEffectRemoveEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> removeBeforeEmitterFactory() {
    return std::make_unique<ActorEffectRemoveBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> removeAfterEmitterFactory();
class ActorEffectRemoveAfterEventEmitter : public ll::event::Emitter<removeAfterEmitterFactory, ActorEffectRemoveAfterEvent> {
    ll::memory::HookRegistrar<ActorEffectRemoveEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> removeAfterEmitterFactory() {
    return std::make_unique<ActorEffectRemoveAfterEventEmitter>();
}

} // namespace Catalyst
