# 发布流程

本项目使用 GitHub Actions、CPack、Inno Setup 与 AppImage 生成发布制品。构建
命令见 [CMake 指南](cmake.md)，本页只描述发布步骤与制品契约。

## 版本

使用 `vX.Y.Z` Git 标签发布。发布工作流接受推送的标签（如 `v3.0.0`）或手动
触发的版本号（如 `3.0.0`）。

打标签前：

- 更新 `CHANGELOG.md`，新增段落使用中文。
- 确认 `CMakeLists.txt` 的 `project(... VERSION ...)`、生成版本头、
  CHANGELOG 最新段落与 `gl-draw-desktop --version` 一致。
- 确认 README 与打包布局仍然匹配。
- 打包相关改动先运行 Release Smoke 工作流。

## 预检

```sh
./scripts/check.sh --enable-tidy
cmake --preset ninja-release
cmake --build --preset ninja-release
ctest --preset ninja-release --output-on-failure
```

Sanitizer 构建（支持的平台）：

```sh
cmake --preset ninja-asan
cmake --build --preset ninja-asan
ctest --preset ninja-asan --output-on-failure
```

至少在一个带图形会话的平台上完成手动 GUI 冒烟：

- 窗口标题为“GL Draw Desktop 画布编辑器”；
- 菜单、工具栏、检查器与图层面板中文正常；
- 快捷键对话框与关于对话框中文正常；
- 启动日志输出字体路径。

## 本地打包

Windows（需要 Inno Setup 6 生成安装包）：

```bat
release.bat -Version 3.0.0 -Platform windows-x64
powershell -NoProfile -ExecutionPolicy Bypass -File tools\package_windows_installer.ps1 -Version 3.0.0 -Platform windows-x64
```

Linux：

```sh
bash ./release.sh --version 3.0.0 --platform linux-x64
bash ./tools/package_appimage.sh --version 3.0.0 --platform linux-x64
```

制品写入 `dist/`：

```text
gl-draw-desktop-v3.0.0-windows-x64-setup.exe
gl-draw-desktop-v3.0.0-windows-x64.zip
gl-draw-desktop-v3.0.0-linux-x64.AppImage
gl-draw-desktop-v3.0.0-linux-x64.tar.gz
```

安装布局：

```text
bin/gl-draw-desktop[.exe]
share/gldrawdesktop/shaders/
share/gldrawdesktop/themes/
share/gldrawdesktop/scripts/
share/gldrawdesktop/fonts/NotoSansSC-Regular.ttf
share/gldrawdesktop/fonts/OFL.txt
```

## CPack

`cmake/Packaging.cmake` 配置 CPack：

- 所有平台默认生成 TGZ；
- Linux 上检测到 `dpkg`/`rpmbuild` 时额外生成 DEB/RPM；
- 源码包为 TGZ/ZIP。

```sh
cd build/ninja-release
cpack
```

## 打标签

```sh
git tag -a v3.0.0 -m "v3.0.0"
git push origin v3.0.0
```

标签必须打在干净、通过 CI 的 `main` 提交上。未经维护者同意，不要发布由 AI
辅助完成的改动。

## 发布制品

`.github/workflows/release.yml` 在推送 `v*` 标签或手动触发时构建 Windows 与
Linux 制品并上传到对应 GitHub Release。`release-smoke.yml` 在 PR 中验证制品
结构与安装内容，不发布真实 Release。

发布说明应注明各平台的运行依赖（Linux 需要 OpenGL/X11 运行库）与中文字体
随包分发。
