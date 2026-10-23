# 依赖与第三方源码

项目保持运行时依赖小而明确。本页记录依赖来源、布局以及移动依赖时需要同步更新
的文件。

## 构建期要求

- CMake 3.21 或更高版本。
- C11 编译器（MSVC、GCC、Clang、AppleClang）。
- Ninja（默认 preset）。
- OpenGL 运行时与开发文件。
- 首次 configure 需要 Git 与网络访问（GLFW 未缓存时）。

## 获取方式

| 组件 | 版本/来源 | 位置 | 许可证 |
| --- | --- | --- | --- |
| GLFW | 3.3.9，CMake FetchContent 获取 | 构建树 `_deps` 或本地缓存 | Zlib |
| OpenGL | 系统平台库 | `find_package(OpenGL)` | 系统 |
| GLAD | GL 3.3 compatibility profile 生成物 | `third_party/glad/` | MIT |
| KHR | Khronos 头文件 | `third_party/KHR/` | Apache-2.0 / MIT |
| Nuklear | 4.12.8 单头文件发行版 | `third_party/nuklear/` | MIT / Public Domain |
| Noto Sans SC | SIL OFL 1.1 | `assets/fonts/` | OFL 1.1 |
| Lua | 可选，`GLDRAWDESKTOP_ENABLE_SCRIPTING=ON` | 系统 `find_package(Lua)` | MIT |

`third_party/nuklear/nuklear_impl.c` 是本项目的编译单元，只定义实现宏并包含
vendored 头文件；修改它属于构建配置变更，不属于第三方代码维护。

## 布局

```text
third_party/
  glad/include/glad/glad.h
  glad/src/glad.c
  KHR/khrplatform.h
  nuklear/include/nuklear/nuklear.h
  nuklear/include/nuklear/nuklear_glfw_gl3.h
  nuklear/nuklear_impl.c
assets/fonts/
  NotoSansSC-Regular.ttf
  OFL.txt
```

vendored 代码由 `gldrawdesktop_vendor` 独立编译，不参与项目警告、格式化与静态
分析。详见 [third_party/README.md](../../third_party/README.md)。

## 更新依赖时

当依赖版本、路径或所有权变化时，同步更新：

- `cmake/Dependencies.cmake`
- `cmake/Sources.cmake`
- `.clang-format`、`.clang-tidy` 与 `.clangd` 中的排除/包含路径
- `scripts/static_analysis.sh`
- `.github/workflows/static-analysis.yml`
- `docs/guides/cmake.md`
- 本文件与 `third_party/README.md`
- 涉及安装文件或运行时资源时的发布打包脚本

## 许可证

在发布评审中保持第三方许可证可见。新增、移动或替换依赖时，在同一 PR 中附上
许可证与再分发说明。
