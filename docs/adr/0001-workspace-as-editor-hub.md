# ADR 0001：Workspace 作为编辑器枢纽

## 状态

已接受。

**日期**：2026-06-05
**最后修改**：2026-10-23

## 背景

项目需要协调文档状态、编辑命令、工具、UI 状态、保存/加载服务与渲染状态。
这些区域需要一个集成点，但不应该随意访问彼此的内部实现。

## 决策

`Workspace` 继续作为编辑器枢纽，拥有 editor core、editor session 与 editor
services 的集成；其他模块优先通过公共 workspace 与 controller API 访问。

## 后果

- 新的顶层编辑行为通常应通过 workspace、controller、service 或 command API
  进入。
- 直接访问 workspace 内部状态应留在 workspace 实现文件中。
- 测试仅在明确验证内部生命周期行为时包含内部头文件。
- UI、工具、渲染与平台代码不得成为替代的编辑器枢纽。
