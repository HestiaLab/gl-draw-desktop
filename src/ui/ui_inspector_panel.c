/**
 * @file ui_inspector_panel.c
 * @brief UiSystem inspector, property, and layer panel rendering.
 */
#include "ui/ui_system_internal.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

static const char* UI_INSPECTOR_EMPTY_HINTS[] = { "未选择对象",
                                                  "快捷键：V H L R E",
                                                  "文档：Ctrl+S 保存，Ctrl+O 打开",
                                                  "滚轮以光标为中心缩放",
                                                  "Delete 删除所选对象" };

static const char* ui_property_label(const char* key)
{
    static const struct {
        const char* key;
        const char* label;
    } kLabels[] = {
        { "x", "X 坐标" },
        { "y", "Y 坐标" },
        { "width", "宽度" },
        { "height", "高度" },
        { "x1", "起点 X" },
        { "y1", "起点 Y" },
        { "x2", "终点 X" },
        { "y2", "终点 Y" },
        { "stroke_r", "描边红" },
        { "stroke_g", "描边绿" },
        { "stroke_b", "描边蓝" },
        { "stroke_a", "描边透明度" },
        { "stroke_width", "描边宽度" },
    };
    size_t index = 0u;

    if (!key) {
        return "";
    }
    for (index = 0u; index < sizeof(kLabels) / sizeof(kLabels[0]); ++index) {
        if (strcmp(key, kLabels[index].key) == 0) {
            return kLabels[index].label;
        }
    }
    return key;
}

static void ui_inspector_empty_hint(struct nk_context* ctx)
{
    int i = 0;

    for (i = 0; i < (int)(sizeof(UI_INSPECTOR_EMPTY_HINTS) / sizeof(UI_INSPECTOR_EMPTY_HINTS[0]));
         ++i) {
        nk_label(ctx, UI_INSPECTOR_EMPTY_HINTS[i], NK_TEXT_LEFT);
    }
}

static void ui_inspector_overview(struct nk_context* ctx, const EditorViewModel* view_model)
{
    if (!ctx || !view_model || !view_model->has_selection) {
        return;
    }

    nk_layout_row_dynamic(ctx, 20.0f, 1);
    nk_labelf(ctx, NK_TEXT_LEFT, "类型：%s", view_model->summary.selection_type_name);
    nk_labelf(ctx, NK_TEXT_LEFT, "已选：%d", view_model->summary.selection_count);
}

static void ui_property_apply_float(UiSystem* ui,
                                    struct nk_context* ctx,
                                    const EditorPropertyView* property_view)
{
    float value = 0.0f;
    char value_text[96];

    if (!ui || !ctx || !property_view) {
        return;
    }

    if (!property_view->editable) {
        snprintf(value_text,
                 sizeof(value_text),
                 "%s: %.2f",
                 ui_property_label(property_view->name),
                 property_view->value);
        nk_label(ctx, value_text, NK_TEXT_LEFT);
        return;
    }

    value = property_view->value;
    nk_property_float(ctx,
                      ui_property_label(property_view->name),
                      property_view->min_value,
                      &value,
                      property_view->max_value,
                      property_view->step > 0.0f ? property_view->step : 1.0f,
                      property_view->inc_per_pixel > 0.0f ? property_view->inc_per_pixel : 0.5f);
    if (fabsf(value - property_view->value) > 1e-6f) {
        EditorAction action = editor_action_make_modify_property(
            property_view->object_id, property_view->name, value);
        ui_system_emit_action(ui, &action);
    }
}

static void
ui_inspector_properties(UiSystem* ui, struct nk_context* ctx, const EditorViewModel* view_model)
{
    int i = 0;

    if (!ctx || !view_model || view_model->property_count <= 0) {
        return;
    }

    nk_layout_row_dynamic(ctx, 20.0f, 1);
    nk_label(ctx, "属性", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, 24.0f, 1);

    for (i = 0; i < view_model->property_count; ++i) {
        ui_property_apply_float(ui, ctx, &view_model->properties[i]);
    }
}

void ui_selection_panel(UiSystem* ui, const EditorViewModel* view_model, RectF bounds)
{
    struct nk_context* ctx = ui->ctx;

    ui->panel_bounds = bounds;
    if (!ctx || !view_model || bounds.w <= 0.0f || bounds.h <= 0.0f) {
        return;
    }

    if (nk_begin(ctx,
                 "检查器",
                 nk_rect(bounds.x, bounds.y, bounds.w, bounds.h),
                 NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_SCROLL_AUTO_HIDE)) {
        if (ui->modal_active) {
            nk_widget_disable_begin(ctx);
        }

        nk_layout_row_dynamic(ctx, 20.0f, 1);
        if (!view_model->has_selection) {
            ui_inspector_empty_hint(ctx);
        } else {
            ui_inspector_overview(ctx, view_model);
            ui_inspector_properties(ui, ctx, view_model);
        }
        ui_layers_panel(ui, ctx, view_model);

        if (ui->modal_active) {
            nk_widget_disable_end(ctx);
        }
    }
    nk_end(ctx);
}
