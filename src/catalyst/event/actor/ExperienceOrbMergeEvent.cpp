#include "ExperienceOrbMergeEvent.h"

#include <algorithm>
#include <cmath>

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/Actor.h"
#include "mc/world/actor/ActorType.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/phys/AABB.h"

#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void ExperienceOrbMergeBeforeEvent::serialize(CompoundTag& nbt) const {
    Cancellable::serialize(nbt);
    nbt["sourceOrb"]        = ll::event::serializeRefObj(sourceOrb());
    nbt["value"]            = value();
    nbt["mergedPickupCount"] = mergedPickupCount();
}

void ExperienceOrbMergeAfterEvent::serialize(CompoundTag& nbt) const {
    ll::event::entity::ActorEvent::serialize(nbt);
    nbt["sourceOrb"]      = ll::event::serializeRefObj(sourceOrb());
    nbt["value"]          = value();
    nbt["oldPickupCount"] = oldPickupCount();
    nbt["newPickupCount"] = newPickupCount();
}


LL_TYPE_INSTANCE_HOOK(
    ExperienceOrbMergeEventHook,
    ll::memory::HookPriority::Normal,
    ExperienceOrb,
    &ExperienceOrb::_tryMergeExistingOrbs,
    void
) {
    auto& bus    = ll::event::EventBus::getInstance();
    auto& region = this->getDimensionBlockSource();
    auto  range  = this->getAABB().cloneAndGrow(Vec3{0.5f, 0.5f, 0.5f});
    int   selfValue = this->getValue();
    auto  selfId    = this->getOrCreateUniqueID();

    auto xpOrbs = region.fetchEntities(
        ActorType::Experience,
        range,
        this,
        [](Actor*) {
            return true;
        }
    );

    for (Actor* actor : xpOrbs) {
        if (!actor || actor->mRemoved) {
            continue;
        }
        if (this->mRemoved) {
            return;
        }

        auto* otherOrb = static_cast<ExperienceOrb*>(actor);

        // Must have same value.
        if (otherOrb->getValue() != selfValue) {
            continue;
        }

        // Either: entity ID difference is a multiple of 40,
        // or: self was spawned this tick (mAge == 0) with 2.5% probability.
        auto  otherId      = otherOrb->getOrCreateUniqueID();
        int64 diff         = std::abs(selfId.rawID - otherId.rawID);
        bool  idCondition  = diff != 0 && diff % 40 == 0;
        bool  ageCondition = this->mAge == 0 && (std::rand() % 40 == 0);
        if (!idCondition && !ageCondition) {
            continue;
        }

        // Only the lower-ID orb processes the merge to avoid duplicates.
        if (selfId.rawID > otherId.rawID) {
            continue;
        }

        // this = target (lower ID, survives), otherOrb = source (higher ID, removed).
        int oldPickupCount    = this->mRandomPickupValue;
        int mergedPickupCount = oldPickupCount + otherOrb->mRandomPickupValue;

        ExperienceOrbMergeBeforeEvent beforeEvent(*this, *otherOrb, selfValue, mergedPickupCount);
        bus.publish(beforeEvent);
        if (beforeEvent.isCancelled() || otherOrb->mRemoved || this->mRemoved) {
            continue;
        }

        this->mRandomPickupValue = beforeEvent.mergedPickupCount();
        this->mAge               = std::min(this->mAge, otherOrb->mAge);
        otherOrb->remove();

        ExperienceOrbMergeAfterEvent afterEvent(
            *this,
            *otherOrb,
            selfValue,
            oldPickupCount,
            this->mRandomPickupValue
        );
        bus.publish(afterEvent);
    }
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class ExperienceOrbMergeBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, ExperienceOrbMergeBeforeEvent> {
    ll::memory::HookRegistrar<ExperienceOrbMergeEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<ExperienceOrbMergeBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class ExperienceOrbMergeAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, ExperienceOrbMergeAfterEvent> {
    ll::memory::HookRegistrar<ExperienceOrbMergeEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<ExperienceOrbMergeAfterEventEmitter>();
}

} // namespace Catalyst
