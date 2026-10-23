# 贡献指南

GL Draw Desktop 是 C11 + CMake + OpenGL 3.3 + Nuklear 的桌面绘图应用。本指南
约束日常贡献、评审与 CI 预期。

## 本地检查

提交前必须执行完整的本地检查（格式检查 + 配置 + 构建 + 测试）。检查命令不会
自动改写源码，格式不符合时会失败。

**Linux/macOS**

```sh
./scripts/bootstrap.sh
./scripts/check.sh
```

**Windows PowerShell**

```powershell
./scripts/bootstrap.ps1
./scripts/check.ps1
```

提交前运行包含 clang-tidy 的完整质量门禁：

```sh
./scripts/check.sh --enable-tidy
```

```powershell
./scripts/check.ps1 -EnableTidy
```

需要显式修复 C 源码和头文件格式时执行：

```sh
./scripts/check.sh --fix-format
```

```powershell
./scripts/check.ps1 -FixFormat
```

## 分支与提交

使用短小、聚焦的分支名：

- `feature/<topic>`：用户可见行为。
- `fix/<topic>`：缺陷修复。
- `refactor/<topic>`：不改变行为的内部调整。
- `infra/<topic>`：构建、CI、文档、打包或工具链。
- `release/<version>`：发布准备。

提交标题使用 Conventional Commits，类型与作用域保持英文，摘要使用中文：

```text
fix(commands): 撤销后保留选择状态
feat(tools): 增加多边形拖拽生命周期
chore(ci): 对齐 CMake preset
```

## Pull Request

创建 Pull Request 前：

- 使用 `type(scope): 摘要` 格式的标题。
- 使用仓库 PR 模板，保留摘要、验证、影响与 AI 辅助章节；不适用时标记为
  “不适用”，不要删除评审上下文。
- 如果改动依赖最新的 CI 或构建变更，先 rebase 或合并目标分支。
- 运行覆盖改动范围的最小验证。
- 可见的 UI、画布、渲染或主题改动附截图或短录屏。
- 说明兼容性、文件格式、打包或发布影响。
- 使用 `Closes #...` 或 `Refs #...` 关联 Issue。

评审首先关注行为、所有权边界与测试；风格问题应指回共享的格式化配置。

## AI 辅助贡献

AI 辅助贡献必须像其他改动一样受范围、评审与验证约束。编辑前，AI 代理应阅读
`AGENTS.md`、[AI 代理政策](docs/guides/ai-agents.md)、
[AI 代理工作流](docs/guides/agent-playbooks.md) 以及任务涉及的文档。

使用实质性 AI 辅助时：

- 在 PR 模板中披露。
- 写明代理或工具名称。
- 概括代理完成的工作。
- 列出已阅读的必读文档。
- 记录人工验证与仍需执行的手动检查。

AI 生成的改动不得绕过架构边界、削弱检查或替代维护者评审。

## 架构边界

保持改动与当前所有权模型一致：

- 持久文档变更通过 `CommandExecutor`。
- 对象存储、图层、ID、修订与空间查询属于 `document/`。
- 选择、剪贴板、对话框、脏状态与布局属于 workspace/session。
- 工具输入通过 `editor_controller` 与 `tool_runtime`。
- UI 消费 view-model 或 workspace 查询状态并发出 action，不拥有编辑规则。
- 渲染消费 `RenderSceneDesc` 或编辑器渲染场景状态，不定义文档行为。
- 平台输入值在 app/input 边界转换。
- 着色器、主题、脚本与字体查找使用 `base/resource_path`。

除 workspace 实现文件外，不要新增 `workspace_internal.h` 依赖；仅当测试明确
验证内部生命周期行为时例外。

## 添加对象

对象通过 `GraphicObjectDescriptor` 描述。

新增或修改对象类型时：

- 按需实现 create、clone、destroy、translate、bounds、hit-test、path、
  property 与 persistence 回调。
- 使用 `register_object_type()` 注册描述符。
- 通过 `src/app/extension_manifest.c` 注册内置对象。
- 新自动分配的对象类型 ID 优先使用 `GRAPHIC_OBJECT_INVALID`。
- 为持久化、边界、命中测试与属性行为补充聚焦测试。

## 添加工具

工具通过 `ToolDescriptor` 描述。

新增或修改工具时：

- 使用 `register_tool()` 注册自定义工具。
- 标准形状拖拽生命周期足够时使用 `register_shape_tool()`。
- 通过 `src/app/tool_manifest.c` 注册内置工具。
- 持久编辑保持命令化；预览使用 overlay。
- 为命令创建与取消行为补充测试。

## 添加命令

命令元数据、可用性与执行是分开的：

- `command_catalog` 拥有命令描述符与 ID。
- `command_availability` 拥有可执行状态检查。
- `command_registry_execute()` 是公共执行入口。
- 具体 workspace 行为拆分在聚焦的 workspace 命令模块中。

命令改动应包含成功路径、不可用状态与撤销/重做测试。

## UI 改动

UI 帧构建拆分在聚焦的 `src/ui/` 模块中。保持 UI 薄：读取 workspace 查询状态、
呈现控件、发出 action，不要把编辑规则搬进控件。

可见 UI 改动应在 PR 中附截图或短录屏；中文字体或布局改动必须启动应用核验。

## 本地验证

```sh
./scripts/check.sh
```

```powershell
./scripts/check.ps1
```

针对特定范围可直接运行 preset：

```sh
cmake --preset ninja-debug
cmake --build --preset ninja-debug
ctest --preset ninja-debug --output-on-failure
```

CI 是三平台覆盖的事实来源。
