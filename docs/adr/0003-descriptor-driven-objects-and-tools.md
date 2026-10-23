# ADR 0003：描述符驱动的对象与工具

## 状态

已接受。

**日期**：2026-06-05
**最后修改**：2026-10-23

## 背景

项目支持内置对象与工具，并保留扩展能力。编辑器需要稳定的注册点，而不是把
每种行为硬编码进 workspace 或 UI。

## 决策

图形对象通过 `GraphicObjectDescriptor` 注册，工具通过 `ToolDescriptor` 注册。
内置注册集中在应用 manifest 中。

## 后果

- 新对象行为通过描述符回调表达生命周期、几何、命中测试、属性与持久化。
- 新工具根据标准形状拖拽生命周期是否足够，选择 `register_tool()` 或
  `register_shape_tool()`。
- 内置对象注册位于 `src/app/extension_manifest.c`。
- 内置工具注册位于 `src/app/tool_manifest.c`。
- UI 发现并调用已注册行为，而不是复制对象或工具规则。
