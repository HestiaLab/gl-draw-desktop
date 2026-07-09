<!-- 对应英文版：docs/build.md | 最后同步：2026-07-09 -->

# 构建与运行 GLDraw

GLDraw 是一个使用 CMake 构建的 C11 OpenGL 绘图编辑器。

推荐使用仓库构建脚本。当需要特定生成器或输出布局时，可使用手动 CMake 命令和可选的 CMake 预设。

## 目录

- [环境要求](#环境要求)
- [快速开始](#快速开始)
- [构建脚本](#构建脚本)
- [CMake 预设](#cmake-预设)
- [手动 CMake](#手动-cmake)
- [测试](#测试)
- [发布打包](#发布打包)
- [CMake 项目布局](#cmake-项目布局)
- [故障排除](#故障排除)


## 环境要求

* CMake 3.15 或更高版本
* 使用 CMake 预设需要 CMake 3.21 或更高版本
* C11 编译器
* OpenGL 运行时和开发文件
* 首次 CMake 配置需要 Git 和网络
* GLFW 3.3.9 由 CMake 自动下载
* GLAD 已提交在仓库中
* Nuklear 为 header-only，包含在源码树中

依赖布局与更新注意事项详见 [dependencies.md](../dependencies.md)。


## 快速开始

Linux 和 macOS：

```sh
./build.sh
./build/Release/bin/GLDraw
```

Windows MinGW/MSYS2 CMD：

```bat
build.bat
build\Release\bin\GLDraw.exe
```


## 构建脚本

Linux、macOS 或 MSYS-like shell：

```sh
./build.sh
./build.sh debug
./build.sh clean
```

Windows CMD with MinGW/MSYS2：

```bat
build.bat
build.bat debug
build.bat clean
```

脚本输出路径：

* Release: `build/Release/bin/GLDraw` 或 `build\Release\bin\GLDraw.exe`
* Debug: `build/Debug/bin/GLDraw` 或 `build\Debug\bin\GLDraw.exe`


## CMake 预设

使用 CMake 3.21+ 可用预设：

```sh
cmake --preset debug
cmake --build --preset debug
ctest --preset debug --output-on-failure
```

常用预设：

* `debug`
* `release`
* `coverage`
* `asan`
* `mingw-debug`
* `mingw-release`
* `msvc-release`

CI 使用预设，但构建脚本仍是本地用户推荐的简单入口。


## 手动 CMake

Linux 和 macOS：

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/bin/GLDraw
```

Windows MinGW Makefiles：

```bat
cmake -S . -B build-mingw -G "MinGW Makefiles"
cmake --build build-mingw --parallel
build-mingw\bin\GLDraw.exe
```

Windows Visual Studio 2022：

```bat
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
build\bin\Release\GLDraw.exe
```


## 测试

项目使用 CTest 进行测试，目前主要在 Linux/macOS CI 环境中运行。

```sh
ctest --test-dir build/Release --output-on-failure
```

手动 CMake 构建时，请将 `build/Release` 替换为使用的构建目录。


## 发布打包

Windows MinGW/MSYS2 创建可分发的 zip 包：

```bat
release.bat
```

安装 Inno Setup 6 后可创建 Windows 安装程序：

```bat
powershell -NoProfile -ExecutionPolicy Bypass -File tools\package_windows_installer.ps1 -Version 0.0.3 -Platform windows-x64
```

Linux 创建可分发的 tarball：

```sh
bash ./release.sh
```

创建 AppImage：

```sh
bash ./tools/package_appimage.sh --version 0.0.3 --platform linux-x64
```

包输出到 `dist/` 目录。当前安装布局：

```text
bin/GLDraw.exe
share/gldraw/shaders/
share/gldraw/themes/
share/gldraw/scripts/
```

GitHub Actions 有发布工作流，推送 `vX.Y.Z` 标签后自动构建并上传：

```text
GLDraw-vX.Y.Z-windows-x64-setup.exe
GLDraw-vX.Y.Z-windows-x64.zip
GLDraw-vX.Y.Z-linux-x64.AppImage
GLDraw-vX.Y.Z-linux-x64.tar.gz
```


## CMake 项目布局

`CMakeLists.txt` 是项目入口。CMake 模块在 `cmake/` 中：

* `CompilerOptions.cmake` - 编译器选项
* `Dependencies.cmake` - GLFW、OpenGL 等
* `Sources.cmake` - 源码分组
* `Tests.cmake` - 测试目标
* `Packaging.cmake` - 安装规则


## 故障排除

* 首次配置时 GLFW 下载失败，检查 Git 和网络后重新运行 CMake。
* 应用不在预期路径，检查构建使用的是脚本布局还是手动 CMake 布局。
* Windows 找不到 `mingw32-make`，确认 MinGW/MSYS2 已安装并在 PATH 中。
