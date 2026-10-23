#include <app/workspace_service.h>
#include <app/workspace_internal.h>

#include <base/math2d.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test_assertions.h"

static const char* k_temp_document_path = "gldraw_workspace_service_test.json";

static GraphicObject* make_rect(float x, float y, float w, float h)
{
    return object_create_rect((RectF){ x, y, w, h }, object_default_style());
}

static int seed_clipboard_with_object(Workspace* workspace, const GraphicObject* source)
{
    if (!workspace || !source) {
        return 0;
    }

    workspace_clear_clipboard(workspace);
    workspace->session.clipboard_objects =
        (GraphicObject**)calloc(1u, sizeof(workspace->session.clipboard_objects[0]));
    if (!workspace->session.clipboard_objects) {
        return 0;
    }

    workspace->session.clipboard_objects[0] = object_clone(source);
    if (!workspace->session.clipboard_objects[0]) {
        workspace_clear_clipboard(workspace);
        return 0;
    }

    workspace->session.clipboard_count = 1;
    workspace->session.clipboard_capacity = 1;
    workspace->session.clipboard_paste_serial = 3u;
    return 1;
}

static int make_workspace_dirty(Workspace* workspace)
{
    GraphicObject* object = NULL;

    if (!workspace) {
        return 0;
    }

    if (!document_add_object(&workspace->core.document, make_rect(10.0f, 20.0f, 30.0f, 40.0f))) {
        return 0;
    }
    object = document_find_object(&workspace->core.document, 1u);
    if (!object) {
        return 0;
    }
    if (!selection_set_add(&workspace->session.selection, object->id)) {
        return 0;
    }
    if (!seed_clipboard_with_object(workspace, object)) {
        return 0;
    }
    if (!command_executor_execute(&workspace->core.commands,
                                  command_create_set_property_from_document(
                                      &workspace->core.document, object->id, "width", 55.0f),
                                  &workspace->core.document)) {
        return 0;
    }
    if (!tool_controller_set_active(&workspace->core.tools, NULL, "rect")) {
        return 0;
    }

    canvas_view_set_center_zoom(&workspace->core.canvas, vec2_make(22.0f, -14.0f), 2.5f);
    workspace_service_set_document_path(workspace, "dirty.json");
    workspace_set_status_message(workspace, "Dirty workspace");
    workspace->session.saved_revision = 1u;
    workspace->session.document_dirty = 1;
    return 1;
}

static int expect_runtime_state_cleared(const Workspace* workspace)
{
    const char* active_id = NULL;

    if (!workspace) {
        return 0;
    }

    if (workspace->session.selection.count != 0) {
        return 0;
    }
    if (workspace->session.clipboard_count != 0) {
        return 0;
    }
    if (workspace->session.clipboard_capacity != 0) {
        return 0;
    }
    if (workspace->session.clipboard_paste_serial != 0u) {
        return 0;
    }
    if (command_executor_can_undo(&workspace->core.commands)) {
        return 0;
    }
    if (command_executor_can_redo(&workspace->core.commands)) {
        return 0;
    }

    active_id = tool_controller_active_id(&workspace->core.tools);
    if (!active_id || strcmp(active_id, "select") != 0) {
        return 0;
    }
    if (workspace->session.saved_revision != workspace->core.document.revision) {
        return 0;
    }
    if (workspace->session.document_dirty) {
        return 0;
    }

    return 1;
}

static int test_new_document_resets_workspace_state(Workspace* workspace)
{
    EXPECT_TRUE(make_workspace_dirty(workspace));

    EXPECT_TRUE(workspace_service_new_document(workspace));

    EXPECT_INT_EQ(workspace->core.document.count, 0);
    EXPECT_UINT_EQ(workspace->core.document.next_id, 1u);
    EXPECT_UINT_EQ(workspace->core.document.active_layer_id, 1u);
    EXPECT_TRUE(expect_runtime_state_cleared(workspace));
    EXPECT_TRUE(workspace->session.current_document_path[0] == '\0');
    EXPECT_STR_EQ(workspace->session.status_message, "新建空白文档");
    EXPECT_FLOAT_EQ(canvas_view_center(&workspace->core.canvas).x, 0.0f);
    EXPECT_FLOAT_EQ(canvas_view_center(&workspace->core.canvas).y, 0.0f);
    EXPECT_FLOAT_EQ(canvas_view_zoom(&workspace->core.canvas), 1.0f);
    return 0;
}

static int test_load_document_resets_runtime_state(Workspace* workspace)
{
    EXPECT_TRUE(
        document_add_object(&workspace->core.document, make_rect(0.0f, 0.0f, 10.0f, 10.0f)));
    EXPECT_TRUE(workspace_service_save_to_path(workspace, k_temp_document_path));
    EXPECT_TRUE(make_workspace_dirty(workspace));

    EXPECT_TRUE(workspace_service_load_from_path(workspace, k_temp_document_path));

    EXPECT_INT_EQ(workspace->core.document.count, 1);
    EXPECT_TRUE(document_find_object(&workspace->core.document, 1u) != NULL);
    EXPECT_TRUE(document_find_object(&workspace->core.document, 2u) == NULL);
    EXPECT_UINT_EQ(workspace->core.document.next_id, 2u);
    EXPECT_TRUE(expect_runtime_state_cleared(workspace));
    EXPECT_STR_EQ(workspace_service_document_path(workspace), k_temp_document_path);
    EXPECT_STR_EQ(workspace->session.status_message,
                  "已加载文档：gldraw_workspace_service_test.json");
    return 0;
}

static void test_workspace_create_destroy_allocates_opaque_workspace(void)
{
    Workspace* workspace =
        workspace_create((RectF){ 0.0f, 0.0f, 640.0f, 480.0f }, "gldraw.test.keymap.json");

    EXPECT_TRUE(workspace != NULL);
    EXPECT_INT_EQ(workspace->core.document.count, 0);
    EXPECT_FLOAT_EQ(canvas_view_zoom(&workspace->core.canvas), 1.0f);
    workspace_destroy(workspace);
}

static int test_workspace_service_invalid_arguments_are_safe(Workspace* workspace)
{
    EXPECT_FALSE(workspace_service_new_document(NULL));
    EXPECT_FALSE(workspace_service_save_to_path(NULL, k_temp_document_path));
    EXPECT_FALSE(workspace_service_save_to_path(workspace, NULL));
    EXPECT_FALSE(workspace_service_save_to_path(workspace, ""));
    EXPECT_FALSE(workspace_service_save(NULL));
    EXPECT_FALSE(workspace_service_load_from_path(NULL, k_temp_document_path));
    EXPECT_FALSE(workspace_service_load_from_path(workspace, NULL));
    EXPECT_FALSE(workspace_service_load_from_path(workspace, ""));
    EXPECT_FALSE(workspace_service_load(NULL));
    EXPECT_FALSE(workspace_service_file_exists(NULL));
    EXPECT_FALSE(workspace_service_file_exists(""));
    EXPECT_STR_EQ(workspace_service_document_path(NULL), "document.json");
    workspace_service_set_document_path(NULL, "ignored.json");
    workspace_service_set_document_path(workspace, NULL);
    return 0;
}

static Workspace g_workspace;

static void run_new_document_resets_workspace_state(void)
{
    (void)test_new_document_resets_workspace_state(&g_workspace);
}

static void run_load_document_resets_runtime_state(void)
{
    (void)test_load_document_resets_runtime_state(&g_workspace);
}

static void run_workspace_service_invalid_arguments_are_safe(void)
{
    (void)test_workspace_service_invalid_arguments_are_safe(&g_workspace);
}

void setUp(void) {}

void tearDown(void) {}

int main(void)
{
    int result = 0;

    remove(k_temp_document_path);
    memset(&g_workspace, 0, sizeof(g_workspace));

    if (!workspace_init(
            &g_workspace, (RectF){ 0.0f, 0.0f, 800.0f, 600.0f }, "gldraw.test.keymap.json")) {
        fprintf(stderr, "workspace_init failed\n");
        return 1;
    }

    UNITY_BEGIN();
    RUN_TEST(run_new_document_resets_workspace_state);
    RUN_TEST(run_load_document_resets_runtime_state);
    RUN_TEST(test_workspace_create_destroy_allocates_opaque_workspace);
    RUN_TEST(run_workspace_service_invalid_arguments_are_safe);
    result = UNITY_END();

    workspace_shutdown(&g_workspace);
    remove(k_temp_document_path);
    return result;
}
