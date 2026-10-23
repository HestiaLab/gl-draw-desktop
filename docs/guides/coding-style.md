# 编码规范

本指南描述项目使用的 C 编码约定。它刻意保持简短：编译器标志与
`.clang-format`/`.clang-tidy` 文件是可强制执行的基线，本文档记录工具无法检查
的选型背后的意图。

## 语言

- C11，以 CMake 目标特性 `c_std_11` 表达。
- 保持公共代码在 MSVC、GCC、Clang 和 AppleClang 之间可移植。
- 平台差异集中在 `src/platform/` 与 `base/resource_path`，不要在主逻辑中散布
  `#ifdef`。

## 命名

| 对象 | 约定 | 示例 |
| --- | --- | --- |
| 文件与目录 | snake_case | `workspace_file_commands.c` |
| 函数 | snake_case，带模块或项目前缀 | `workspace_file_new_document` |
| 结构体与枚举类型 | PascalCase（沿用现有类型） | `Workspace`、`CommandDescriptor` |
| 枚举值 | UPPER_SNAKE_CASE | `COMMAND_AVAILABILITY_ALWAYS` |
| 宏与常量 | UPPER_SNAKE_CASE | `GLDRAW_PATH_MAX`、`UI_THEME_ID_CAPACITY` |
| Include guards | `<MODULE>_<FILE>_H` | `GLDRAW_APP_APPLICATION_H` |

函数式宏使用 UPPER_SNAKE_CASE 书写，函数使用 snake_case。这一区别十分关键：
`scripts/check_decl_def.py` 依靠它来区分宏与已声明的公共函数。

C 代码内部 `GLDRAW_` / `gldraw_` 前缀保持不变；对外产物与 CMake 变量使用
`GLDRAWDESKTOP_` 前缀。

## 头文件组织

- 公共的跨模块接口位于 `include/<module>/` 下。
- 为每个公共头文件提供 `<MODULE>_<FILE>_H` 形式的 include guard。
- 公共头文件只包含它需要的内容；私有头文件位于 `src/<module>/` 并与其源文件
  相邻，绝不泄漏到 `include/`。
- `third_party/` 与 `tests/vendor/` 不得被项目头文件直接修改。

## 格式与静态分析

- `clang-format` 是格式化的权威，配置位于 `.clang-format`：
  LLVM 基础风格、4 空格缩进、100 列、函数花括号换行、控制语句同行、指针左靠。
- 检查脚本运行 `clang-format --dry-run --Werror`。
- `clang-tidy` 在 CI 中以警告即错误运行。本地运行用
  `./scripts/check.ps1 -EnableTidy`（Windows）或 `./scripts/check.sh --enable-tidy`
  （Linux/macOS）。
- `third_party/` 与 `tests/vendor/` 不参与格式化、clang-tidy 与 cppcheck。
  不要手工格式化或修复 vendored 代码。

## 注释与 API 文档

- 公共头文件用 Doxygen `/** ... */` 注释记录模块、类型与函数契约。
- 每个公共函数都有一行 `@brief`。非 void 函数有 `@return`。`@param` 条目记录
  NULL 约束、有效范围、借用指针与拥有指针，以及输出参数的失败状态。
- 实现文件不重复公共头文件的契约注释。局部注释解释为什么，而不是做了什么。
- 用户可见文案使用中文；代码注释与提交摘要可以保留中文，但内部标识符、
  JSON 键与命令 ID 保持英文。

## 所有权与错误处理

- 明确所有权：负责分配的函数负责释放，除非所有权被转移并在头文件中记录。
- 持久文档变更通过 `CommandExecutor`；不要直接修改文档内部状态。
- 优先返回状态码与输出参数，而不是隐藏的全局状态。
- 失败路径必须留下可安全清理的状态。

## 文件编码

源文件为 UTF-8 编码，使用 LF 行尾（由 `.gitattributes` 强制执行）。MSVC 构建
通过 `/utf-8` 保证中文字符串按 UTF-8 解释。
