# GL Draw Desktop

A C11 desktop canvas drawing editor built on OpenGL 3.3 and Nuklear, with Chinese-only UI.

用 C11 + OpenGL 3.3 + Nuklear 写成的桌面画布绘图编辑器：直线、矩形、椭圆、图层、检查器、三套内置主题与外部主题、JSON 保存、PNG 导出、命令式撤销重做，界面与文档全中文。仓库名与远程地址为 `gl-draw-desktop`，构建产物与安装目录使用 `gldrawdesktop` 命名空间。

**状态**：v3.0.0（2026-10-23）· MIT · 中文界面/文档 · Windows / Linux 预编译包（macOS 源码构建）

[![CI](https://github.com/HestiaLab/gl-draw-desktop/actions/workflows/ci.yml/badge.svg)](https://github.com/HestiaLab/gl-draw-desktop/actions/workflows/ci.yml)
[![Static Analysis](https://github.com/HestiaLab/gl-draw-desktop/actions/workflows/static-analysis.yml/badge.svg)](https://github.com/HestiaLab/gl-draw-desktop/actions/workflows/static-analysis.yml)
[![Release Smoke](https://github.com/HestiaLab/gl-draw-desktop/actions/workflows/release-smoke.yml/badge.svg)](https://github.com/HestiaLab/gl-draw-desktop/actions/workflows/release-smoke.yml)
[![Release](https://img.shields.io/badge/release-v3.0.0-blue)](https://github.com/HestiaLab/gl-draw-desktop/releases/tag/v3.0.0)
[![License](https://img.shields.io/github/license/HestiaLab/gl-draw-desktop)](LICENSE)

[下载 v3.0.0](https://github.com/HestiaLab/gl-draw-desktop/releases/tag/v3.0.0) · [快速开始](#快速开始) · [文档](#文档) · [贡献](CONTRIBUTING.md) · [许可](LICENSE)

![GL Draw Desktop 中文界面：菜单、工具栏、网格画布、检查器与图层、中文状态栏](assets/preview-zh.png)

本项目只提供中文文档与中文界面，不维护英文版本。

## 快速开始

前置：CMake 3.21+、Ninja、C11 编译器（MSVC、GCC、Clang 或 AppleClang）、支持 OpenGL 3.3 的显卡驱动；首次 configure 需要网络拉取 GLFW 3.3.9（离线可用 `FETCHCONTENT_SOURCE_DIR_GLFW`）。Linux 还需要 OpenGL 与 X11 开发包，macOS 需要 Xcode Command Line Tools。详见[环境配置](docs/guides/environment.md)。

首次检出先运行 bootstrap，它会检查工具链并完成一次 configure：

**Windows PowerShell**

```powershell
./scripts/bootstrap.ps1
```

**Linux/macOS**

```sh
./scripts/bootstrap.sh
```

日常检查与构建：

```powershell
./scripts/check.ps1
```

```sh
./scripts/check.sh
```

手动执行 CMake 命令：

```sh
cmake --preset ninja-debug
cmake --build --preset ninja-debug
ctest --preset ninja-debug --output-on-failure
```

本机实测（Windows 11 + MSYS2 UCRT64 GCC 16.2.0 + CMake 4.3.3 + Ninja 1.13.2）：

```console
$ ./build/ninja-debug/bin/gl-draw-desktop.exe --version
gl-draw-desktop 3.0.0

$ ctest --preset ninja-debug --output-on-failure
100% tests passed, 0 tests failed out of 18

Label Time Summary:
architecture     =   1.99 sec*proc (4 tests)
documentation    =   0.24 sec*proc (1 test)
unit             =   0.18 sec*proc (13 tests)
```

（`ctest` 的 `Test project` 行包含本机路径，已省略。）

运行应用：

```sh
# Windows
./build/ninja-debug/bin/gl-draw-desktop.exe

# Linux/macOS
./build/ninja-debug/bin/gl-draw-desktop
```

运行后应看到标题为 `GL Draw Desktop 画布编辑器` 的 1440×900 窗口，包含菜单、工具栏、网格画布、检查器与图层、中文状态栏。

命令行参数：

```text
-h, --help     显示帮助
-v, --version  显示版本号
```

## 环境要求

- CMake 3.21 或更高版本。
- Ninja（默认 preset）与 C11 编译器（MSVC、GCC、Clang、AppleClang）。
- 支持 OpenGL 3.3 的显卡驱动。
- 首次配置需要网络：GLFW 3.3.9 通过 FetchContent 获取；离线环境可用
  `FETCHCONTENT_SOURCE_DIR_GLFW` 指向本地 GLFW 源码。

Linux 还需要 OpenGL 与 X11 开发包，macOS 需要 Xcode Command Line Tools。
详见 [环境配置](docs/guides/environment.md)。

## 项目结构

```text
include/                    公共头文件（app、document、commands、tools、ui 等）
src/                        实现；模块与 include/ 保持同名目录
third_party/                vendored GLAD、KHR、Nuklear（不参与项目警告）
tests/                      CTest 测试（Unity）
cmake/                      CMake 模块
scripts/                    本地引导、检查与质量门禁脚本
docs/                       中文项目文档
assets/                     截图与界面字体
shaders/                    OpenGL 着色器
themes/                     内置与外部主题
packaging/                  Windows 安装包与 Linux desktop 文件
tools/                      发布打包脚本
```

## 界面字体

界面使用随包分发的 Noto Sans SC Regular（SIL OFL 1.1），按 GB2312 常用字
范围烘焙图集。构建时字体复制到可执行文件同级的 `fonts/` 目录，安装后位于
`share/gldrawdesktop/fonts/`；可用 `GLDRAWDESKTOP_FONT` 环境变量指定其他
字体文件。字体来源与再生成方式见
[assets/fonts/README.md](assets/fonts/README.md)。

## 安装

预编译包见 [Releases v3.0.0](https://github.com/HestiaLab/gl-draw-desktop/releases/tag/v3.0.0)：Windows 安装包/zip、Linux AppImage/tar.gz；macOS 需从源码构建。从源码安装：

```sh
cmake --preset ninja-release
cmake --build --preset ninja-release
cmake --install build/ninja-release --prefix install
```

安装内容包括可执行文件、着色器、主题、脚本、界面字体与文档。本项目是纯应用，
不导出库或 CMake 包。

## 文档

- [CMake 与构建](docs/guides/cmake.md)
- [操作与快捷键](docs/guides/controls.md)
- [测试与质量门禁](docs/guides/testing.md)
- [编码规范](docs/guides/coding-style.md)
- [环境配置](docs/guides/environment.md)
- [依赖与 third_party](docs/guides/dependencies.md)
- [发布流程](docs/guides/release.md)
- [AI 代理政策](docs/guides/ai-agents.md)
- [AI 代理工作流](docs/guides/agent-playbooks.md)
- [架构决策记录索引](docs/adr/README.md)
- [ADR 0001 Workspace 作为编辑器枢纽](docs/adr/0001-workspace-as-editor-hub.md)
- [ADR 0002 持久变更通过 CommandExecutor](docs/adr/0002-command-executor-for-durable-mutations.md)
- [ADR 0003 描述符驱动的对象与工具](docs/adr/0003-descriptor-driven-objects-and-tools.md)
- [ADR 0004 UI 不拥有编辑规则](docs/adr/0004-ui-does-not-own-editing-rules.md)
- [ADR 0005 AI 代理贡献边界](docs/adr/0005-ai-agent-contribution-boundaries.md)
- [ADR 0006 第三方源码布局](docs/adr/0006-third-party-source-layout.md)
- [ADR 0007 应用型项目与中文单语适配](docs/adr/0007-application-and-chinese-only-shape.md)
- [归档文档索引](docs/archive/README.md)
- [历史重构记录](docs/archive/REFACTOR_LOG.md)
- [工程标准](C_PROJECT_STANDARD.md)
- [贡献指南](CONTRIBUTING.md)
- [安全策略](SECURITY.md)

## 许可证

MIT，见 [LICENSE](LICENSE)。界面字体另受 SIL Open Font License 1.1 约束。
