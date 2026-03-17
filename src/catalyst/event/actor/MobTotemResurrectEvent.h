#pragma once

#include <utility>
#include <vector>

#include "ll/api/event/Cancellable.h"
#include "ll/api/event/entity/MobEvent.h"

#include "catalyst/Macros.h"

class ActorDamageSource;
class ItemStack;
class MobEffect;

namespace Catalyst {

struct MobTotemResurrectEffect {
    MobEffect* effect;
    int        durationTicks;
    int        amplifier;
    bool       visible;
};

class CATALYST_API MobTotemResurrectBeforeEvent final : public ll::event::Cancellable<ll::event::entity::MobEvent> {
    ActorDamageSource const& mKillingDamage;
    ItemStack const&         mTotem;
    bool                     mHasTotem;
    std::vector<MobTotemResurrectEffect> mEffects;

public:
    MobTotemResurrectBeforeEvent(
        Mob&                     mob,
        ActorDamageSource const& killingDamage,
        ItemStack const&         totem,
        bool                     hasTotem,
        std::vector<MobTotemResurrectEffect> effects
    )
    : Cancellable(mob),
      mKillingDamage(killingDamage),
      mTotem(totem),
      mHasTotem(hasTotem),
      mEffects(std::move(effects)) {}

    ActorDamageSource const& killingDamage() const { return mKillingDamage; }
    ItemStack const&         totem() const { return mTotem; }
    bool                     hasTotem() const { return mHasTotem; }
    std::vector<MobTotemResurrectEffect>&       effects() { return mEffects; }
    std::vector<MobTotemResurrectEffect> const& effects() const { return mEffects; }
    void setEffects(std::vector<MobTotemResurrectEffect> effects) { mEffects = std::move(effects); }
};

class CATALYST_API MobTotemResurrectAfterEvent final : public ll::event::entity::MobEvent {
    ActorDamageSource const& mKillingDamage;
    ItemStack const&         mTotem;
    bool                     mHasTotem;
    bool                     mResult;
    std::vector<MobTotemResurrectEffect> mEffects;

public:
    MobTotemResurrectAfterEvent(
        Mob&                     mob,
        ActorDamageSource const& killingDamage,
        ItemStack const&         totem,
        bool                     hasTotem,
        bool                     result,
        std::vector<MobTotemResurrectEffect> effects
    )
    : MobEvent(mob),
      mKillingDamage(killingDamage),
      mTotem(totem),
      mHasTotem(hasTotem),
      mResult(result),
      mEffects(std::move(effects)) {}

    ActorDamageSource const& killingDamage() const { return mKillingDamage; }
    ItemStack const&         totem() const { return mTotem; }
    bool                     hasTotem() const { return mHasTotem; }
    bool                     result() const { return mResult; }
    std::vector<MobTotemResurrectEffect> const& effects() const { return mEffects; }
};

} // namespace Catalyst
