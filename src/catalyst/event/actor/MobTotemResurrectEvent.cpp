#include "MobTotemResurrectEvent.h"

#include <algorithm>

#include "mc/deps/shared_types/legacy/LevelEvent.h"
#include "mc/deps/shared_types/legacy/actor/ActorDamageCause.h"
#include "mc/entity/components/FallDistanceComponent.h"
#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/ActorDamageSource.h"
#include "mc/world/actor/ActorEvent.h"
#include "mc/world/actor/Mob.h"
#include "mc/world/attribute/AttributeBuffType.h"
#include "mc/world/attribute/Attribute.h"
#include "mc/world/attribute/AttributeInstance.h"
#include "mc/world/attribute/AttributeInstanceRef.h"
#include "mc/world/attribute/AttributeModificationContext.h"
#include "mc/world/attribute/BaseAttributeMap.h"
#include "mc/world/attribute/InstantaneousAttributeBuff.h"
#include "mc/world/attribute/SharedAttributes.h"
#include "mc/world/effect/EffectDuration.h"
#include "mc/world/effect/MobEffect.h"
#include "mc/world/effect/MobEffectInstance.h"
#include "mc/world/item/ItemStack.h"
#include "mc/world/level/Level.h"

#include "mc/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void MobTotemResurrectBeforeEvent::serialize(CompoundTag& nbt) const {
    Cancellable::serialize(nbt);
    nbt["killingDamage"] = ll::event::serializeRefObj(killingDamage());
    nbt["totem"]         = ll::event::serializeRefObj(totem());
    nbt["hasTotem"]      = hasTotem();
    ListTag effectsList;
    for (auto const& eff : effects()) {
        CompoundTag effTag;
        effTag["effect"]        = ll::event::serializePtrObj(eff.effect);
        effTag["durationTicks"] = eff.durationTicks;
        effTag["amplifier"]     = eff.amplifier;
        effTag["visible"]       = eff.visible;
        effectsList.emplace_back(std::move(effTag));
    }
    nbt["effects"] = std::move(effectsList);
}

void MobTotemResurrectAfterEvent::serialize(CompoundTag& nbt) const {
    ll::event::entity::MobEvent::serialize(nbt);
    nbt["killingDamage"] = ll::event::serializeRefObj(killingDamage());
    nbt["totem"]         = ll::event::serializeRefObj(totem());
    nbt["hasTotem"]      = hasTotem();
    nbt["result"]        = result();
    ListTag effectsList;
    for (auto const& eff : effects()) {
        CompoundTag effTag;
        effTag["effect"]        = ll::event::serializePtrObj(eff.effect);
        effTag["durationTicks"] = eff.durationTicks;
        effTag["amplifier"]     = eff.amplifier;
        effTag["visible"]       = eff.visible;
        effectsList.emplace_back(std::move(effTag));
    }
    nbt["effects"] = std::move(effectsList);
}


LL_TYPE_INSTANCE_HOOK(
    MobTotemResurrectHook,
    HookPriority::Normal,
    Mob,
    &Mob::checkTotemDeathProtection,
    bool,
    ::ActorDamageSource const& killingDamage
) {
    auto const damageCause = killingDamage.mCause;
    if (
        damageCause == SharedTypes::Legacy::ActorDamageCause::Void
        || damageCause == SharedTypes::Legacy::ActorDamageCause::SelfDestruct
    ) {
        return false;
    }

    auto const& beforeTotem = this->getEquippedTotem();
    bool        hadTotem    = !beforeTotem.isNull();

    auto& bus = ll::event::EventBus::getInstance();

    std::vector<MobTotemResurrectEffect> totemEffects;
    if (hadTotem) {
        if (auto* regen = MobEffect::REGENERATION()) {
            totemEffects.push_back({regen, 900, 1, regen->mEffectVisible});
        }
        if (auto* absorption = MobEffect::ABSORPTION()) {
            totemEffects.push_back({absorption, 100, 1, absorption->mEffectVisible});
        }
        if (auto* fireResistance = MobEffect::FIRE_RESISTANCE()) {
            totemEffects.push_back({fireResistance, 800, 0, fireResistance->mEffectVisible});
        }
    }

    MobTotemResurrectBeforeEvent beforeEvent(*this, killingDamage, beforeTotem, hadTotem, std::move(totemEffects));
    bus.publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return false;
    }

    auto const effectsToApply = beforeEvent.effects();
    auto const& totem         = this->getEquippedTotem();
    bool        hasTotem      = !totem.isNull();

    bool result = false;
    if (hasTotem) {
        // Restore health to exactly 1.0f, same behavior as vanilla totem protection path.
        auto*                attributeMap = const_cast<BaseAttributeMap*>(this->getAttributes().get());
        AttributeInstanceRef healthRef    = attributeMap->getMutableInstance(SharedAttributes::HEALTH().mIDValue);
        if (healthRef.mPtr) {
            AttributeModificationContext context{};
            context.mAttributeMap = attributeMap;

            float const healthBuffValue = 1.0f - healthRef.mPtr->mCurrentValue;
            if (healthBuffValue > 0.0f) {
                InstantaneousAttributeBuff healthBuff(healthBuffValue, AttributeBuffType::None);
                healthRef.mPtr->addBuff(healthBuff, context);
            }
        }

        this->removeAllEffects();

        auto applyTotemEffect = [this](MobTotemResurrectEffect const& config) {
            MobEffect* effect = config.effect;
            if (!effect) {
                return;
            }
            MobEffectInstance effectInstance(effect->mId);
            EffectDuration duration{};
            duration.mValue              = std::max(0, config.durationTicks);
            effectInstance.mDuration     = duration;
            effectInstance.mAmplifier    = std::max(0, config.amplifier);
            effectInstance.mEffectVisible = config.visible;
            this->addEffect(effectInstance);
        };

        for (auto const& effect : effectsToApply) {
            applyTotemEffect(effect);
        }

        auto& fallDistance = this->getEntityContext().getOrAddComponent<FallDistanceComponent>();
        fallDistance.mValue = 0.0f;

        auto& level = this->getLevel();
        level.broadcastActorEvent(*this, ActorEvent::TalismanActivate, 0);
        level.broadcastLocalEvent(
            this->getDimensionBlockSource(),
            SharedTypes::Legacy::LevelEvent::SoundTotemUsed,
            this->getPosition(),
            0
        );

        this->consumeTotem();
        result = true;
    }

    MobTotemResurrectAfterEvent afterEvent(*this, killingDamage, totem, hasTotem, result, effectsToApply);
    bus.publish(afterEvent);

    return result;
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class MobTotemResurrectBeforeEventEmitter
: public ll::event::Emitter<beforeEmitterFactory, MobTotemResurrectBeforeEvent> {
    ll::memory::HookRegistrar<MobTotemResurrectHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<MobTotemResurrectBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class MobTotemResurrectAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, MobTotemResurrectAfterEvent> {
    ll::memory::HookRegistrar<MobTotemResurrectHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<MobTotemResurrectAfterEventEmitter>();
}

} // namespace Catalyst
