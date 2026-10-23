# AGENTS.md

面向在 GL Draw Desktop 项目中工作的 AI 代理。代理改动与其他贡献一样受范围、
评审与验证约束。

## 必读

修改文件之前，先阅读：

- `CONTRIBUTING.md`
- `docs/guides/testing.md`
- `docs/guides/cmake.md`

涉及架构或结构变化的改动，还需阅读：

- `docs/adr/README.md`
- 与改动区域相关的 ADR

涉及依赖、打包或安全的工作，还需阅读：

- `third_party/README.md`
- `SECURITY.md`
- `docs/guides/release.md`
- `docs/guides/dependencies.md`

## 项目形状

C11 + CMake + OpenGL 3.3 + Nuklear 的纯桌面应用：

```text
Workspace
  -> EditorCore: Document, CommandExecutor, CanvasView, ToolController
  -> EditorSession: keymap, layout, selection, clipboard, dialogs, dirty state
  -> EditorServices: save, load, export, and top-level action callbacks
```

`Workspace` 是编辑器枢纽，但优先使用公共 workspace 与 controller API，而不是
直接访问内部状态。

```text
include/                    公共头文件
src/                        实现；模块目录与 include/ 同名
third_party/                vendored GLAD/KHR/Nuklear（不参与项目警告）
tests/                      CTest 测试（Unity）
cmake/                      构建模块
scripts/                    本地引导、检查与质量门禁
docs/                       中文文档
assets/                     截图与界面字体
```

## 入口点

- `src/main.c`：进程入口，解析 `--help` / `--version` 后调用 `gldrawdesktop_run()`。
- `include/gldrawdesktop/app.h`：最小公共 API。
- `src/app/application.c`：应用生命周期与帧序列。
- `src/app/application_callbacks.c`：平台回调注册。
- `src/app/application_workspace_services.c`：应用层保存/加载/导出回调。
- `include/app/workspace.h`：公共 workspace API。
- `include/app/editor_controller.h`：输入与渲染场景状态门面。
- `src/app/registration_manifest.c`：内置注册顺序。
- `src/app/extension_manifest.c`：内置对象注册。
- `src/app/tool_manifest.c`：内置工具注册。
- `src/app/command_registry.c`：公共命令执行入口。
- `src/commands/command_executor.c`：撤销/重做历史与命令执行。
- `src/render/render_system.c`：渲染系统生命周期与场景提交。
- `src/tools/tool_runtime.c`：活动工具生命周期与分发。
- `src/ui/`：聚焦的 UI 模块。
- `CMakeLists.txt` 与 `cmake/`：构建入口与模块。

## 构建与验证

```sh
cmake --preset ninja-debug
cmake --build --preset ninja-debug
ctest --preset ninja-debug --output-on-failure
```

Windows PowerShell 使用 `./scripts/check.ps1`，Linux/macOS 使用
`./scripts/check.sh`。提交前运行带 clang-tidy 的完整检查：

```sh
./scripts/check.sh --enable-tidy
```

```powershell
./scripts/check.ps1 -EnableTidy
```

涉及窗口、字体或渲染的改动，必须启动应用并截图核验中文显示；CTest 不覆盖 GUI。
截图工具必须 DPI-aware，否则坐标会错位。

## 所有权规则

- 持久文档变更通过 `CommandExecutor`。
- 对象存储、图层、ID、修订与空间查询属于 `document/`。
- 选择、剪贴板、对话框、脏状态与布局属于 workspace/session。
- 工具输入通过 `editor_controller` 与 `tool_runtime`，不要从控件直接驱动。
- UI 消费 view-model 或 workspace 查询状态并发出 action，不拥有编辑规则。
- 渲染消费 `RenderSceneDesc` 或编辑器渲染场景状态，不定义文档行为。
- 平台输入值在 app/input 边界转换。
- 着色器、主题、脚本与字体查找使用 `base/resource_path`。
- 除 workspace 实现文件外，不新增 `workspace_internal.h` 依赖；仅当测试明确
  验证内部生命周期行为时例外。

## 扩展模式

对象通过 `GraphicObjectDescriptor` 描述：

- 按需实现 create、clone、destroy、translate、bounds、hit-test、path、
  property 与 persistence 回调。
- 使用 `register_object_type()` 注册。
- 内置注册位于 `src/app/extension_manifest.c`。
- 新自动分配的对象类型 ID 优先使用 `GRAPHIC_OBJECT_INVALID`。

工具通过 `ToolDescriptor` 描述：

- 使用 `register_tool()` 注册自定义工具。
- 标准形状拖拽生命周期足够时使用 `register_shape_tool()`。
- 内置注册位于 `src/app/tool_manifest.c`。
- 持久编辑保持命令化；预览使用 overlay。

## 命令与 UI

- `command_catalog` 拥有命令描述符与 ID。
- `command_availability` 拥有可执行状态检查。
- `command_registry_execute()` 是公共执行入口。
- 具体 workspace 行为拆分在 `workspace_*_commands.c` 模块中。
- UI 帧构建拆分在聚焦的 `src/ui/` 模块中。

## 规则

- 公共 API 属于 `include/`，私有实现与私有头属于 `src/`。
- `third_party/` 与 `tests/vendor/` 不参与格式化与静态分析，不得因无关改动
  重写它们。
- 文档、界面、命令行输出与提交摘要只使用中文；提交类型与作用域保持英文。
- 字体变更需同步更新 `assets/fonts/README.md` 的来源、SHA-256 与字形范围。
- 测试应断言行为，而不是偶然的实现细节。
- 不为让改动通过而放宽警告、测试、静态分析或 CI。
- 修改构建、测试、安装或安全策略时同步更新文档。

许可证：MIT；界面字体为 SIL OFL 1.1。
