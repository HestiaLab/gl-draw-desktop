# 更新日志

> 新增段落用中文书写；已发布段落保留发布时的原文，不重写。

## 3.0.0 — 2026-10-23

### 破坏性变更

- 项目更名为 `gl-draw-desktop`：仓库、可执行文件、安装资源目录、产物与
  安装包名称统一更新；C 代码内部 `GLDRAW_` / `gldraw_` 前缀保持不变。
- 项目仅提供中文界面与中文文档，不再维护英文版本。
- 主题设置与快捷键配置文件更名为 `gldrawdesktop.settings.json` 与
  `gldrawdesktop.keymap.json`，不兼容旧文件。
- 最低 CMake 版本提升到 3.21，默认构建类型改为 Debug。

### 新增

- 对齐 c-project-standard v0.2.1：ninja 预设、引导与检查脚本、格式与静态
  分析门禁、文档与架构门禁、覆盖率棘轮、Unity 测试、编辑器模板与
  Dependabot。
- 新增强制中文界面的 Noto Sans SC 字体分发与字体路径覆盖环境变量
  `GLDRAWDESKTOP_FONT`。
- 新增 `--help` 与 `--version` 命令行入口，以及编译期版本头。
- 新增安装冒烟、CI 静态分析与三平台构建矩阵。
- 新增第三方源码目录 `third_party/`，vendored 代码不再参与项目警告、
  格式化与静态分析。

### 变更

- 发布链保留 Windows 安装包与 Linux AppImage，并改用 `ninja-release`
  预设构建。
- 测试框架迁移到 vendored Unity。

## 旧版记录

## 0.0.2

### Notes

* See the GitHub Release notes for packaged assets and release-specific
  highlights.

## 0.0.1

### Notes

* Initial public release series.
