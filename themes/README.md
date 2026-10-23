# 主题文件（`themes/*.json`）

应用启动时从 `themes/` 目录加载自定义主题，每个 `.json` 文件解析为一个主题
条目。运行期间主题文件会自动热重载，也可以通过“主题 -> 重新加载主题”强制刷新。

## 必填字段

- `id`（字符串）：唯一标识，允许字符 `[A-Za-z0-9._-]`。

## 可选字段

- `label`（字符串）：主题菜单中的显示名。
- `base_theme`（字符串）：父主题 ID，例如 `gldrawdesktop-dark-plus`。
- `base` 或 `extends` 可作为 `base_theme` 的别名。
- `base_theme` 应指向内置主题，以保证加载顺序可预测。

## 颜色字段（`#RRGGBB` 或 `#RRGGBBAA`）

- `primary`、`primary_hover`、`primary_active`
- `background`、`panel`、`panel_hover`、`canvas_background`
- `text`、`text_secondary`、`text_disabled`
- `border`、`border_hover`
- `success`、`warning`、`error`

## 浮点字段

- `row_height`、`panel_width`、`menu_height`、`status_height`、`tool_rail_width`
- `padding`、`margin`、`gap`
- `border_radius`
- `transition_duration`

## 布尔字段

- `enable_transitions`

## 示例

```json
{
  "id": "my-team-theme",
  "label": "团队主题",
  "base_theme": "gldrawdesktop-dark-plus",
  "primary": "#57A7FF",
  "panel": "#18212B",
  "canvas_background": "#0E141B",
  "enable_transitions": true
}
```
