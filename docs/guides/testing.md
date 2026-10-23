# 测试与质量门禁

使用 CTest 进行自动化测试。

```sh
ctest --preset ninja-debug --output-on-failure
```

命令详情见 [CMake 指南](cmake.md)。

## 测试框架

测试使用 vendored 的 [Unity](https://github.com/ThrowTheSwitch/Unity) 框架
（MIT，位于 `tests/vendor/unity/`）。每个测试在 `UNITY_BEGIN()` 与
`UNITY_END()` 之间用 `RUN_TEST()` 注册用例，并提供 `setUp()` 与 `tearDown()`。
`tests/support/test_assertions.h` 提供项目沿用的 `EXPECT_*` 断言词汇，全部映射
到 Unity。vendored Unity 源码不参与 `clang-format`、`clang-tidy` 与 `cppcheck`。

## 期望

- 缺陷修复应包含回归测试。
- 新增公共 API 应包含成功与失败用例。
- 测试应断言行为，而非行覆盖率。
- 不启动交互式 OpenGL 窗口；窗口、字体与布局由人工冒烟验证。
- 新增质量门禁必须证明自己能失败：破坏被守护项后应得到指名 `文件:行` 的
  非 0 退出，恢复后必须复绿。

## 自动化测试范围

| 测试 | 覆盖 |
| --- | --- |
| `gldraw_document_core_tests` | 文档、命令、持久化、路径与压力场景 |
| `gldraw_document_tests` | 文档初始化与对象所有权 |
| `gldraw_selection_tests` | 选择集合与修订号 |
| `gldraw_command_tests` | 命令执行、撤销/重做、内存预算与锁定图层 |
| `gldraw_registry_tests` | 工具/命令注册、菜单与快捷键 |
| `gldraw_ui_logic_tests` | view-model、命令分发、工具与对话框路由 |
| `gldraw_ui_theme_tests` | 内置/外部主题、设置读写与错误回退 |
| `gldraw_workspace_service_tests` | 新建、保存、加载与状态清理 |
| `gldraw_resource_path_tests` | 资源路径解析与回退 |
| `gldraw_renderer_tests` | 渲染场景、选择高亮与缓冲复用 |
| `gldraw_png_writer_tests` | RGBA PNG 头部输出 |
| `gldraw_fake_star_tests` | 扩展对象注册、命中与持久化 |
| `gldraw_no_builtin_knowledge_tests` | 对象类型只通过注册表发现 |
| `gldrawdesktop_decl_def_check` | `include/` 的公共声明在 `src/` 均有定义 |
| `gldrawdesktop_docs_check` | 相对链接、锚点、代码围栏与 README 索引覆盖 |
| `gldrawdesktop_version_consistency` | CMake、生成头、CHANGELOG 与 `--version` 一致 |
| `gldrawdesktop_gate_behaviors` | 门禁脚本自身能失败（unittest） |
| `gldrawdesktop_cpack_config` | CPack 发布敏感配置 |

CI 不运行 GUI：OpenGL 上下文、字体烘焙与窗口布局依赖图形会话，无法在无头
runner 上稳定复现。

## 手动检查

以下行为无法通过 CTest 验证，PR 中需记录命令、平台与观察结果：

1. 启动应用，确认窗口标题为“GL Draw Desktop 画布编辑器”。
2. 确认菜单、工具栏、检查器与图层面板中的中文正常显示（无方块或缺字）。
3. 打开快捷键对话框与关于对话框，确认中文排版正常。
4. 新建/保存/加载文档，确认状态栏提示为中文。
5. 记录启动日志中的字体路径；字体或打包改动时执行一次安装并运行安装目录中的
   可执行文件。

截图核验时请使用 DPI-aware 的截图方式；普通截图工具在系统缩放下可能产生
坐标错位。

## 静态分析

CI 运行 clang-format、clang-tidy、cppcheck 与 YAML lint。本地 clang-tidy
可选，因为工具可用性因平台而异：

**Linux/macOS**

```sh
./scripts/check.sh --enable-tidy
```

**Windows PowerShell**

```powershell
./scripts/check.ps1 -EnableTidy
```

`third_party/` 与 `tests/vendor/` 不在扫描面内；不要为了让检查通过而格式化
它们的代码。

## 需要运行的检查

| 变更类型 | 最低限度的有效检查 |
| --- | --- |
| C 源码或公共头文件 | `./scripts/check.ps1` 或 `./scripts/check.sh` |
| 字体、窗口或渲染改动 | 上述检查 + 手动启动与截图核验 |
| 安装或 CPack 改动 | `ninja-release` + `cmake --install` 安装冒烟 |
| CI 或 YAML 改动 | `git diff --check` 与 CI 静态分析 |
| 发布敏感改动 | `ninja-release`、`ninja-asan` 与安装冒烟 |

## 覆盖率

`scripts/coverage_ratchet.py` 是可选入口，默认不接入 CI 与 check 脚本。
覆盖率基线只能有唯一平台口径，更新时请在 `scripts/coverage_baseline.txt`
中注明取数平台。
