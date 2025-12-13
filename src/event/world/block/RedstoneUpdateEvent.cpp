#include "RedstoneUpdateEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/block/Block.h"
#include "mc/world/redstone/circuit/ChunkCircuitComponentList.h"
#include "mc/world/redstone/circuit/CircuitSceneGraph.h"
#include "mc/world/redstone/circuit/CircuitSystem.h"

namespace Catalyst {

LL_AUTO_TYPE_INSTANCE_HOOK(
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

            for (const auto& listItem : *chunkComponentList.mComponents) {
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
                                beforeEvent(region, component->mPos, newStrength, component->mIsFirstTime, component);
                            bus.publish(beforeEvent);

                            if (!beforeEvent.isCancelled()) {
                                if (!component->mIsFirstTime || !component->mIgnoreFirstUpdate) {
                                    const Block& blockToUpdate = region.getBlock(component->mPos);
                                    blockToUpdate.getBlockType().onRedstoneUpdate(
                                        region,
                                        component->mPos,
                                        newStrength,
                                        component->mIsFirstTime
                                    );
                                }
                                RedstoneUpdateAfterEvent afterEvent(
                                    region,
                                    component->mPos,
                                    newStrength,
                                    component->mIsFirstTime,
                                    component
                                );
                                bus.publish(afterEvent);
                            }
                        }
                        component->mIsFirstTime = false;
                    }
                }
            }

            for (const auto& listItem : secondaryUpdateQueue) {
                BaseCircuitComponent* component = listItem.mComponent;
                if (!component) continue;

                int newStrength = component->getStrength();
                if (newStrength != -1) {
                    RedstoneUpdateBeforeEvent
                        beforeEvent(region, component->mPos, newStrength, component->mIsFirstTime, component);
                    bus.publish(beforeEvent);

                    if (!beforeEvent.isCancelled()) {
                        if (!component->mIsFirstTime || !component->mIgnoreFirstUpdate) {
                            const Block& blockToUpdate = region.getBlock(component->mPos);
                            blockToUpdate.getBlockType()
                                .onRedstoneUpdate(region, component->mPos, newStrength, component->mIsFirstTime);
                        }
                        RedstoneUpdateAfterEvent
                            afterEvent(region, component->mPos, newStrength, component->mIsFirstTime, component);
                        bus.publish(afterEvent);
                    }
                }
                component->mIsFirstTime = false;
            }
        }
    }
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class RedstoneUpdateBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, RedstoneUpdateBeforeEvent> {
    ll::memory::HookRegistrar<RedstoneUpdateEventHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<RedstoneUpdateBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class RedstoneUpdateAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, RedstoneUpdateAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<RedstoneUpdateAfterEventEmitter>();
}

} // namespace Catalyst