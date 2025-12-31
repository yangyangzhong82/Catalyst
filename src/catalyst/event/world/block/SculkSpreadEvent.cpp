#include "SculkSpreadEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/world/level/WorldBlockTarget.h"
#include "mc/world/level/block/MultifaceSpreader.h"

namespace Catalyst {

using SpreadResult = ::std::optional<::std::pair<::BlockPos const, uchar const>>;

LL_AUTO_TYPE_INSTANCE_HOOK(
    SculkSpreadHook,
    ll::memory::HookPriority::Normal,
    MultifaceSpreader,
    &MultifaceSpreader::getSpreadFromFaceTowardDirection,
    SpreadResult,
    ::IBlockWorldGenAPI& target,
    ::Block const&       self,
    ::Block const&       block,
    ::BlockPos const&    pos,
    uchar                startingFace,
    uchar                spreadDirection
) {
    auto& region = static_cast<WorldBlockTarget&>(target).mBlockSource;
    auto& bus    = ll::event::EventBus::getInstance();

    auto result = origin(target, self, block, pos, startingFace, spreadDirection);

    if (result.has_value()) {
        BlockPos targetPos = result->first;

        SculkSpreadBeforeEvent beforeEvent(region, pos, targetPos, startingFace, spreadDirection);
        bus.publish(beforeEvent);

        if (beforeEvent.isCancelled()) {
            return std::nullopt;
        }

        SculkSpreadAfterEvent afterEvent(region, pos, targetPos, startingFace, spreadDirection);
        bus.publish(afterEvent);
    }

    return result;
}

static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory();
class SculkSpreadBeforeEventEmitter : public ll::event::Emitter<beforeEmitterFactory, SculkSpreadBeforeEvent> {
    ll::memory::HookRegistrar<SculkSpreadHook> hook;
};
static std::unique_ptr<ll::event::EmitterBase> beforeEmitterFactory() {
    return std::make_unique<SculkSpreadBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory();
class SculkSpreadAfterEventEmitter : public ll::event::Emitter<afterEmitterFactory, SculkSpreadAfterEvent> {};
static std::unique_ptr<ll::event::EmitterBase> afterEmitterFactory() {
    return std::make_unique<SculkSpreadAfterEventEmitter>();
}

} // namespace Catalyst