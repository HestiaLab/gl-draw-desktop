# CMake 与构建

本指南介绍常用 CMake 工作流：配置、构建、测试、安装，以及新增源文件、测试和
vendored 依赖时的落点。

## 前置要求

- CMake 3.21 或更高版本。
- C11 编译器：Windows 上的 MSVC、Linux 上的 GCC/Clang、macOS 上的 AppleClang。
- 默认 preset 使用 Ninja；也可通过本地 preset 或命令行选择其他 generator。
- 支持 OpenGL 3.3 的图形驱动。
- 首次配置需要网络以获取 GLFW。

Windows 工具链安装步骤见 [环境配置](environment.md)。

项目通过 `c_std_11` 在目标级别要求 C11。项目代码不得依赖编译器扩展。

## 心智模型

1. Configure：读取 `CMakeLists.txt`、选择选项并生成构建文件。
2. Build：编译源文件并链接目标。
3. Test：从构建树运行 CTest。
4. Install：把可执行文件、运行时资源、字体与文档复制到安装前缀。

configure 失败时构建文件不可信；build 失败时测试与安装没有意义。

## 快速命令

```sh
cmake --preset ninja-debug
cmake --build --preset ninja-debug
ctest --preset ninja-debug --output-on-failure
```

日常开发使用封装脚本：

**Windows PowerShell**

```powershell
./scripts/bootstrap.ps1
./scripts/check.ps1
```

**Linux/macOS**

```sh
./scripts/bootstrap.sh
./scripts/check.sh
```

构建后运行应用：

```sh
./build/ninja-debug/bin/gl-draw-desktop
```

启动时会输出实际加载的中文字体路径，便于排查字体与打包问题。

## Presets

| Preset | 用途 |
| --- | --- |
| `ninja-debug` | 默认跨平台开发构建，输出编译数据库。 |
| `ninja-release` | Ninja 优化构建，用于发布验证。 |
| `ninja-asan` | AddressSanitizer + UBSan 的 Ninja debug 构建。 |
| `ninja-coverage` | 带覆盖率标志的 Ninja debug 构建。 |
| `debug` / `release` / `asan` / `coverage` | 与 generator 无关的兼容条目。 |

configure、build、test preset 共用名称：

```sh
cmake --preset ninja-release
cmake --build --preset ninja-release
ctest --preset ninja-release --output-on-failure
```

`ninja-*` preset 是编辑器与 CI 的默认路径，会强制 Ninja 并生成
`build/<preset>/compile_commands.json`。

需要 Visual Studio、Xcode 等 generator 时，可复制
`CMakeUserPresets.example.json` 为 `CMakeUserPresets.json`（已被 Git 忽略）。
不要在提交的 preset 中写入本机编译器或 SDK 路径。

## 重要选项

| 选项 | 含义 |
| --- | --- |
| `GLDRAWDESKTOP_BUILD_APP` | 构建应用可执行文件。 |
| `GLDRAWDESKTOP_BUILD_TESTING` | 构建 CTest 测试与质量门禁。 |
| `GLDRAWDESKTOP_INSTALL` | 生成安装与 CPack 规则。 |
| `GLDRAWDESKTOP_ENABLE_SCRIPTING` | 启用 Lua 脚本支持。 |
| `GLDRAWDESKTOP_ENABLE_ASAN` | 启用 AddressSanitizer。 |
| `GLDRAWDESKTOP_ENABLE_UBSAN` | 启用 UndefinedBehaviorSanitizer。 |
| `GLDRAWDESKTOP_ENABLE_COVERAGE` | 启用覆盖率标志。 |

对于顶层构建，应用、测试与安装规则默认为开启；通过 `add_subdirectory()` 引入时
默认为关闭。

## 目标结构

`CMakeLists.txt` 只负责顶层编排，源文件清单与模块逻辑在 `cmake/` 中：

| 目标 | 职责 |
| --- | --- |
| `editor_model` | document、model、base 与对象注册 |
| `render_core` | 画布视图、渲染场景、图像输出 |
| `platform_glfw` | GLFW 窗口与平台适配 |
| `render_glfw_gl` | OpenGL 设备、着色器与缓冲池 |
| `editor_commands` | 命令与撤销/重做历史 |
| `editor_tools` | 工具注册与运行时 |
| `editor_runtime` | workspace、session、命令分发与输入 |
| `ui_nuklear_glfw` | Nuklear UI 模块 |
| `editor_app` | 应用生命周期、文件对话框与平台服务 |
| `gldrawdesktop` | 可执行文件，输出名为 `gl-draw-desktop` |
| `gldrawdesktop_vendor` | vendored GLAD 与 Nuklear 实现，不参与项目警告 |

## 依赖与 third_party

- GLFW 3.3.9 由 `cmake/Dependencies.cmake` 通过 FetchContent 获取；设置
  `FETCHCONTENT_SOURCE_DIR_GLFW` 或 `GLDRAWDESKTOP_LOCAL_GLFW_DIR` 可切换到
  本地源码，适用于离线构建。
- OpenGL 通过 `find_package(OpenGL REQUIRED)` 获取。
- Lua 仅在 `GLDRAWDESKTOP_ENABLE_SCRIPTING=ON` 时通过 `find_package(Lua)` 获取。
- GLAD、KHR 头与 Nuklear 位于 `third_party/`，由独立静态目标
  `gldrawdesktop_vendor` 编译，不施加项目警告与静态分析。
- 第三方版本与许可证记录在 [third_party/README.md](../../third_party/README.md)。

修改 `third_party/` 只应出于依赖维护目的；常规功能改动不得顺带重写 vendored 代码。

## 添加源文件

1. 实现文件放在 `src/<module>/`，公共 API 放在 `include/<module>/`。
2. 把新的 `.c` 文件加入 `cmake/Sources.cmake` 对应目标列表。
3. 公共函数必须在 `src/` 中有定义，声明/定义门禁会检查这一点。
4. 在 `tests/` 下补充测试。
5. 运行 `./scripts/check.ps1` 或 `./scripts/check.sh`。

私有头文件放在 `src/<module>/` 内并与源文件相邻，不要泄漏到 `include/`。

## 添加测试

使用 `cmake/Tests.cmake` 提供的辅助函数：

```cmake
gldrawdesktop_add_test(gldraw_feature_tests
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/feature/test_feature.c
    LIBS
        editor_runtime
)
```

测试目标会自动获得 `include/`、生成头目录、`tests/support` 与 Unity 头文件路径，
并链接 vendored 依赖。测试文件使用 Unity 断言，提供 `setUp()`/`tearDown()`。

测试不得启动交互式 OpenGL 窗口；命令行行为可用 `--version` 之类的无窗口路径
验证。

## 生成的版本头文件

版本只在 `project(...)` 中声明一次：

```cmake
project(GlDrawDesktop VERSION 3.0.0 LANGUAGES C)
```

CMake 生成 `gldrawdesktop/version.h`，源文件通过
`GLDRAWDESKTOP_VERSION_STRING` 等宏使用它。版本一致性门禁会比较 CMake、
生成头、`CHANGELOG.md` 与 `gl-draw-desktop --version` 的输出。

## 安装

```sh
cmake --preset ninja-release
cmake --build --preset ninja-release
cmake --install build/ninja-release --prefix install
```

安装内容：

```text
install/bin/gl-draw-desktop[.exe]
install/share/gldrawdesktop/shaders/
install/share/gldrawdesktop/themes/
install/share/gldrawdesktop/scripts/
install/share/gldrawdesktop/fonts/NotoSansSC-Regular.ttf
install/share/gldrawdesktop/fonts/OFL.txt
install/share/doc/GlDrawDesktop/{README.md,LICENSE}
```

本项目不安装公共头、不导出 CMake 包，因此下游不能 `find_package` 消费。

## 资源查找

`base/resource_path` 按以下顺序解析 `shaders/`、`themes/`、`scripts/` 与
`fonts/`：

1. 当前工作目录下的相对路径；
2. 可执行文件目录下的相对路径；
3. `../share/gldrawdesktop/` 下的安装路径；
4. 源码树根目录（`GLDRAW_RESOURCE_ROOT`）。

构建时着色器、主题、脚本与字体也会复制到可执行文件同级，便于直接运行。

## 编辑器默认配置

仓库提交了可移植的编辑器默认配置：

- `.vscode/extensions.json` 推荐 CMake Tools 与 clangd。
- `.vscode/settings.json` 使用 `ninja-debug` preset。
- `.clangd` 指向 `build/ninja-debug` 并补充 `third_party/` 头路径。
- `.vscode/tasks.json` 提供 configure、build、test 任务。

不要提交本机绝对路径；把本地差异放进 `CMakeUserPresets.json` 或用户级设置。

## 本地质量检查

```powershell
./scripts/check.ps1 -EnableTidy
```

```sh
./scripts/check.sh --enable-tidy
```

如果本机缺少 `clang-format`、`clang-tidy` 或 `yamllint`，以 CI 结果为准。

## 故障排除

**GLFW 拉取失败**

- 症状：configure 阶段下载失败或超时。
- 处理：检查网络；或把 `FETCHCONTENT_SOURCE_DIR_GLFW` 指向本地 GLFW 源码。

**旧缓存指向其他路径**

- 症状：configure 提示源目录不一致。
- 处理：删除对应 `build/<preset>` 后重新 configure。

**应用提示找不到字体**

- 症状：启动输出“界面字体加载失败”。
- 处理：确认 `fonts/NotoSansSC-Regular.ttf` 与可执行文件同级，或安装目录包含
  `share/gldrawdesktop/fonts/`，或设置 `GLDRAWDESKTOP_FONT`；详见
  [assets/fonts/README.md](../../assets/fonts/README.md)。

**Windows 窗口尺寸或 DPI 异常**

- 当前后端按窗口坐标绘制；截图工具必须 DPI-aware，否则截图区域会错位。
