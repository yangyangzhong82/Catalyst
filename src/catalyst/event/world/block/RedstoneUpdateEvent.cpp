#include "RedstoneUpdateEvent.h"

#include "catalyst/event/EmitterRegistration.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/redstone/circuit/ChunkCircuitComponentList.h"
#include "mc/world/redstone/circuit/CircuitSceneGraph.h"
#include "mc/world/redstone/circuit/CircuitSystem.h"
#include "mc/world/redstone/circuit/components/BaseCircuitComponent.h"
#include "mc/deps/nbt/CompoundTag.h"
#include "ll/api/event/EventRefObjSerializer.h"

namespace Catalyst {

void RedstoneUpdateEvent::serialize(CompoundTag& nbt) const {
    ll::event::world::WorldEvent::serialize(nbt);
    nbt["pos"]       = ListTag{pos().x, pos().y, pos().z};
    nbt["strength"]  = strength();
    nbt["firstTime"] = isFirstTime();
    nbt["component"] = ll::event::serializePtrObj(component());
}


LL_TYPE_INSTANCE_HOOK(
    RedstoneUpdateEventHook,
    ll::memory::HookPriority::Normal,
    CircuitSystem,
    &CircuitSystem::updateBlocks,
    void,
    ::BlockSource&    region,
    ::BlockPos const& chunkPos
) {
    if (this->mHasBeenEvaluated) {
        auto& componentsByChunk  = this->mSceneGraph->mActiveComponentsPerChunk;
        auto  chunkEntryIterator = componentsByChunk.find(chunkPos);

        if (chunkEntryIterator != componentsByChunk.end()) {
            std::vector<ChunkCircuitComponentList::Item> secondaryUpdateQueue;
            ChunkCircuitComponentList&                   chunkComponentList = chunkEntryIterator->second;
            auto&                                        bus                = ll::event::EventBus::getInstance();

            for (auto& listItem : *chunkComponentList.mComponents) {
                BaseCircuitComponent* component = listItem.mComponent;
                if (!component) continue;

                if (component->mNeedsUpdate && !component->mRemoved) {
                    component->mNeedsUpdate = false;

                    if (component->isSecondaryPowered()) {
                        secondaryUpdateQueue.push_back(listItem);
                    } else {
                        int newStrength = component->getStrength();
                        if (newStrength != -1) {
                            RedstoneUpdateBeforeEvent
                                beforeEvent(region, listItem.mPos, newStrength, component->mIsFirstTime, component);
                            bus.publish(beforeEvent);

                            if (!beforeEvent.isCancelled()) {
                                this->updateIndividualBlock(component, chunkPos, listItem.mPos, region);
                                RedstoneUpdateAfterEvent
                                    afterEvent(region, listItem.mPos, newStrength, component->mIsFirstTime, component);
                                bus.publish(afterEvent);
                            }
                        }
                        component->mIsFirstTime = false;
                    }
                }
            }

            for (auto& listItem : secondaryUpdateQueue) {
                BaseCircuitComponent* component = listItem.mComponent;
                if (!component) continue;

                int newStrength = component->getStrength();
                if (newStrength != -1) {
                    RedstoneUpdateBeforeEvent
                        beforeEvent(region, listItem.mPos, newStrength, component->mIsFirstTime, component);
                    bus.publish(beforeEvent);

                    if (!beforeEvent.isCancelled()) {
                        this->updateIndividualBlock(component, chunkPos, listItem.mPos, region);
                        RedstoneUpdateAfterEvent
                            afterEvent(region, listItem.mPos, newStrength, component->mIsFirstTime, component);
                        bus.publish(afterEvent);
                    }
                }
                component->mIsFirstTime = false;
            }
        }
    }
}

CATALYST_HOOKED_EVENT_PAIR(
    RedstoneUpdateBeforeEvent,
    RedstoneUpdateAfterEvent,
    RedstoneUpdateEventHook
)

} // namespace Catalyst
