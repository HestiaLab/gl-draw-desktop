# 架构决策记录

对影响未来贡献者的稳定决策使用 ADR。

当决策改变项目结构、公共 API 方向、依赖策略、打包策略，或长期测试与 CI 预期
时，创建一条 ADR。不要为常规实现细节或一次性 bug 修复创建 ADR。

每条 ADR 应包含背景（Context）、决策（Decision）与后果（Consequences），并
保持简短。

## 记录

- [0001 Workspace 作为编辑器枢纽](0001-workspace-as-editor-hub.md)
- [0002 持久变更通过 CommandExecutor](0002-command-executor-for-durable-mutations.md)
- [0003 描述符驱动的对象与工具](0003-descriptor-driven-objects-and-tools.md)
- [0004 UI 不拥有编辑规则](0004-ui-does-not-own-editing-rules.md)
- [0005 AI 代理贡献边界](0005-ai-agent-contribution-boundaries.md)
- [0006 第三方源码布局](0006-third-party-source-layout.md)
- [0007 应用型项目与中文单语适配](0007-application-and-chinese-only-shape.md)
