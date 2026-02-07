
#include "ll/api/memory/Hook.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/containers/models/LevelContainerModel.h"

LL_AUTO_TYPE_INSTANCE_HOOK(
    BlockContainerChangeHook,
    HookPriority::Normal,
    LevelContainerModel,
    &LevelContainerModel::$_onItemChanged,
    void,
    int              slotNumber,
    ItemStack const& oldItem,
    ItemStack const& newItem
) {
    if (*reinterpret_cast<void***>(this) != LevelContainerModel::$vftable())
        return origin(slotNumber, oldItem, newItem);
    if (mPlayer.mContainerManager) {
        auto id      = mPlayer.getDimensionId();
        auto slotNum = slotNumber + this->_getContainerOffset();
    }
}