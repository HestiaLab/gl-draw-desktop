#include <app/application_dialog_actions.h>

#include <app/workspace_dialogs.h>
#include <app/workspace_service.h>
#include <base/path_utils.h>

#include "application_internal.h"

#include <stdio.h>

void application_dialog_actions_update_save_as_message(Application* app, const char* error_text)
{
    char directory[GLDRAW_PATH_MAX];

    if (!app) {
        return;
    }

    if (!path_utils_dirname(
            workspace_service_document_path(app->workspace), directory, sizeof(directory))) {
        snprintf(directory, sizeof(directory), ".");
    }

    if (error_text && error_text[0] != '\0') {
        char message[1024];

        snprintf(message,
                 sizeof(message),
                 "%s\n\n请输入新文件名。\n文件将保存在同一目录：\n%s",
                 error_text,
                 directory);
        workspace_dialog_set_message(app->workspace, message);
        return;
    }

    {
        char message[1024];

        snprintf(
            message, sizeof(message), "请输入新文件名。\n文件将保存在同一目录：\n%s", directory);
        workspace_dialog_set_message(app->workspace, message);
    }
}

void application_dialog_actions_report_startup_load_failure(Application* app, const char* path)
{
    if (!app || !path) {
        return;
    }

    workspace_set_statusf(app->workspace, "启动加载失败：%s", path);
}

void application_dialog_actions_report_open_cancelled(Application* app)
{
    if (!app) {
        return;
    }

    workspace_set_status_message(app->workspace, "已取消打开。");
}

void application_dialog_actions_report_open_picker_error(Application* app)
{
    if (!app) {
        return;
    }

    workspace_set_status_message(app->workspace, "打开失败：文件选择器不可用或操作失败。");
}

void application_dialog_actions_report_export_cancelled(Application* app)
{
    if (!app) {
        return;
    }

    workspace_set_status_message(app->workspace, "已取消导出 PNG。");
}

void application_dialog_actions_report_export_picker_error(Application* app)
{
    if (!app) {
        return;
    }

    workspace_set_status_message(app->workspace, "导出 PNG 失败：保存对话框不可用或操作失败。");
}
