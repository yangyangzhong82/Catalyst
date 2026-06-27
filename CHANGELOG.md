# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html)

## [v0.4.0] - 2026-06-27

### ⚠️ 破坏性变更 (Breaking Changes)

重构事件库，为每对 `XxxBeforeEvent` / `XxxAfterEvent` 抽出共享基类 `XxxEvent`，消除字段、getter、构造函数与 `serialize()` 的重复。

对下游使用者的影响：

- 新增公开基类符号 `XxxEvent`（如 `PlayerDropItemEvent`、`BlockPistonEvent` 等）。`XxxBeforeEvent` 现派生自 `ll::event::Cancellable<XxxEvent>`，`XxxAfterEvent` 现派生自 `XxxEvent`。
- **继承链改变**：直接依赖 `XxxBeforeEvent` / `XxxAfterEvent` 继承层级或对其做静态类型断言的代码需要重新编译，可能需要调整。监听 `XxxBeforeEvent` / `XxxAfterEvent` 的常规代码无需改动（类名与字段访问接口保持不变）。
- **容器事件基类收敛**：`PlayerOpenContainerAfterEvent`、`PlayerCloseContainerAfterEvent` 的基类由 `PlayerEvent` 上移至 `ServerPlayerEvent`（向后兼容的扩宽，After 事件新增 `self()` 返回 `ServerPlayer&`）。
- 因 ABI 改变，使用本事件库的插件需随本版本一并重新编译。

未改动：`ServerPong`、`ClientLogin`、`NetherPortalCreate`、`DragonEggBlockTeleport` 等 Before/After 字段本质不同的事件保持原样；NBT 序列化输出字段保持不变。

## [v0.3.0] - 2026-04-07

- 适配LL 26.10.0

## [v0.2.1] - 2026-01-31

- 修复红石更新事件和生物放置方块事件，并恢复客户端连接事件

## [v0.2.2] - 2026-02-02

- 增加玩家盾牌格挡事件
- 为耕地退化事件增加Actor参数

## [v0.2.3] - 2026-02-02

- 增加方块被爆炸摧毁事件