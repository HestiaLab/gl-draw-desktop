<!-- 对应英文版：docs/testing.md | 最后同步：2026-07-09 -->

# GLDraw 测试

GLDraw 使用 CTest 进行自动化测试。CI 是平台覆盖率的权威来源，本地检查应聚焦于改动涉及的模块。

## 目录

- [快速命令](#快速命令)
- [测试分层](#测试分层)
- [回归测试要求](#回归测试要求)
- [覆盖率](#覆盖率)
- [手动验证](#手动验证)


## 快速命令

推荐的本地检查：

```sh
./scripts/check.sh
```

Windows PowerShell：

```powershell
./scripts/check.ps1
```

指定预设：

```sh
cmake --preset debug
cmake --build --preset debug
ctest --preset debug --output-on-failure
```

AddressSanitizer 和覆盖率预设（需要支持的编译器）：

```sh
cmake --preset asan
cmake --build --preset asan
ctest --preset asan --output-on-failure
```

```sh
cmake --preset coverage
cmake --build --preset coverage
ctest --preset coverage --output-on-failure
```


## 测试分层

使用最窄的测试层来验证行为：

* **Document 测试**：对象存储、图层、ID、边界、序列化、命中测试、属性、空间行为。
* **Command 测试**：可执行状态检查、成功路径、撤销/重做、脏状态影响。
* **Tool 测试**：生命周期决策、命令创建、取消、预览状态。
* **Workspace/Service 测试**：保存、加载、导出、剪贴板、选择、对话框、跨模块行为。
* **UI 逻辑测试**：视图模型和状态推导。避免直接测试 Nuklear 绘制细节。
* **Render 测试**：渲染器合同验证，不将文档行为移入渲染代码。

测试文件按所属区域分组：

```text
tests/
  app/
  base/
  commands/
  document/
  image/
  render/
  script/
  support/
  ui/
```

共享测试工具放在 `tests/support/`。新测试靠近所验证的模块边界。


## 回归测试要求

Bug 修复应包含回归测试，除非行为只能通过图形输出或手动 UI 交互观察。无法添加测试时，需在 PR 中记录原因并给出手动验证说明。

新对象类型应包含以下测试：

* 创建与销毁
* 克隆与平移行为
* 边界与命中测试
* 属性读写
* 序列化往返

新命令应包含以下测试：

* 可用性检查
* 成功执行
* 撤销和重做
* 无操作或无效输入行为

新工具应包含以下测试：

* 激活与取消
* 输入序列处理
* 命令生成
* 预览状态


## 覆盖率

覆盖率报告由 Linux CI 生成。覆盖率作为风险评估的参考，而非替代有针对性断言的指标。避免仅覆盖代码行而不验证行为的测试。


## 手动验证

以下类型的改动需要手动检查：

* 画布交互手感
* 视觉渲染输出
* 菜单、对话框、面板或主题外观
* 平台打包和可执行文件布局

在 PR 中附上截图或录屏。
