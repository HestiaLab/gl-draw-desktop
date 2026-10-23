# ADR 0006：第三方源码布局

## 状态

已接受，取代 2026-06-05 的初版决定。

**日期**：2026-06-05
**最后修改**：2026-10-23

## 背景

初版决定把 GLAD、KHR 与 Nuklear 保留在常规源码与 include 根目录下，以避免
一次性迁移带来的噪声。随着项目对齐 `c-project-standard`，质量门禁需要明确的
第三方扫描面，include 根目录也不再适合混放 vendored 头文件。

## 决策

第三方源码统一迁移到 `third_party/`：

```text
third_party/glad/include/glad/glad.h
third_party/glad/src/glad.c
third_party/KHR/khrplatform.h
third_party/nuklear/include/nuklear/nuklear.h
third_party/nuklear/include/nuklear/nuklear_glfw_gl3.h
third_party/nuklear/nuklear_impl.c
```

vendored 代码由独立的 `gldrawdesktop_vendor` 静态目标编译，不施加项目警告；
质量门禁扫描面保持 `include/ src/ tests/`，因此 `third_party/` 天然不在
clang-format、clang-tidy 与 cppcheck 范围内。

## 后果

- 所有目标通过 SYSTEM include 目录引用第三方头文件，警告不来自 vendored 代码。
- 依赖升级单独提交，并在 `third_party/README.md` 记录版本与许可证。
- 常规功能改动不得顺带重写 vendored 代码。
- 新增第三方依赖时同步更新 CMake、门禁脚本、文档与许可证说明。
