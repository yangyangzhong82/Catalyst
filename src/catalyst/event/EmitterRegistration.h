#pragma once

#include <memory>

#include "ll/api/event/Emitter.h"
#include "ll/api/memory/Hook.h"

// 为一个 final 事件类型生成 Emitter 与工厂样板，把它注册到 LeviLamina 的 EventBus。
// 尾随参数是要与该 emitter 生命周期绑定的 BDS hook（存在 >=1 个监听器时安装）。
// HookRegistrar 本身是可变参数模板，故单 hook、多 hook、零 hook 均由同一展开覆盖。
#define CATALYST_HOOKED_EMITTER(EventType, ...)                                            \
    static ::std::unique_ptr<::ll::event::EmitterBase> EventType##EmitterFactory();        \
    class EventType##Emitter final                                                         \
    : public ::ll::event::Emitter<EventType##EmitterFactory, EventType> {                  \
        [[maybe_unused]] ::ll::memory::HookRegistrar<__VA_ARGS__> hook;                     \
    };                                                                                     \
    static ::std::unique_ptr<::ll::event::EmitterBase> EventType##EmitterFactory() {        \
        return ::std::make_unique<EventType##Emitter>();                                    \
    }

// 共享同一组 hook 的 Before/After 事件对的便捷封装。
#define CATALYST_HOOKED_EVENT_PAIR(BeforeEvent, AfterEvent, ...)                            \
    CATALYST_HOOKED_EMITTER(BeforeEvent, __VA_ARGS__)                                       \
    CATALYST_HOOKED_EMITTER(AfterEvent, __VA_ARGS__)
