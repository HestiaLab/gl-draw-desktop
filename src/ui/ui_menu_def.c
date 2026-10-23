#include "ui_menu_def.h"

#include <tools/tool.h>

#include <stdlib.h>
#include <string.h>

/**
 * @file ui_menu_def.c
 * @brief Menu item static configuration data.
 *
 * This file contains the menu structure as static data.
 * To add/modify menu items, edit this array rather than
 * the rendering code.
 */

/**
 * @brief Menu item definitions
 *
 * Each entry defines one menu item with its type, label,
 * keyboard shortcut, ID, and parent menu ID.
 */
static const MenuItemDef g_core_menu_items[] = {
    { MENU_ITEM_SUBMENU, "文件", "", MENU_ID_FILE, -1 },

    { MENU_ITEM_ACTION, "新建", "Ctrl+N", MENU_ID_FILE_NEW, MENU_ID_FILE },
    { MENU_ITEM_ACTION, "打开", "Ctrl+O", MENU_ID_FILE_OPEN, MENU_ID_FILE },
    { MENU_ITEM_ACTION, "保存", "Ctrl+S", MENU_ID_FILE_SAVE, MENU_ID_FILE },
    { MENU_ITEM_ACTION, "另存为...", "", MENU_ID_FILE_SAVE_AS, MENU_ID_FILE },

    /* Separator */
    { MENU_ITEM_SEPARATOR, "", "", -1, MENU_ID_FILE },

    { MENU_ITEM_ACTION, "导出为 PNG", "", MENU_ID_FILE_EXPORT_PNG, MENU_ID_FILE },

    /* Separator */
    { MENU_ITEM_SEPARATOR, "", "", -1, MENU_ID_FILE },

    { MENU_ITEM_ACTION, "退出", "Alt+F4", MENU_ID_FILE_EXIT, MENU_ID_FILE },

    { MENU_ITEM_SUBMENU, "编辑", "", MENU_ID_EDIT, -1 },

    { MENU_ITEM_ACTION, "撤销", "Ctrl+Z", MENU_ID_EDIT_UNDO, MENU_ID_EDIT },
    { MENU_ITEM_ACTION, "重做", "Ctrl+Y", MENU_ID_EDIT_REDO, MENU_ID_EDIT },

    /* Separator */
    { MENU_ITEM_SEPARATOR, "", "", -1, MENU_ID_EDIT },

    { MENU_ITEM_ACTION, "剪切", "Ctrl+X", MENU_ID_EDIT_CUT, MENU_ID_EDIT },
    { MENU_ITEM_ACTION, "复制", "Ctrl+C", MENU_ID_EDIT_COPY, MENU_ID_EDIT },
    { MENU_ITEM_ACTION, "粘贴", "Ctrl+V", MENU_ID_EDIT_PASTE, MENU_ID_EDIT },
    { MENU_ITEM_ACTION, "删除", "Del", MENU_ID_EDIT_DELETE, MENU_ID_EDIT },

    /* Separator */
    { MENU_ITEM_SEPARATOR, "", "", -1, MENU_ID_EDIT },

    { MENU_ITEM_ACTION, "全选", "Ctrl+A", MENU_ID_EDIT_SELECT_ALL, MENU_ID_EDIT },

    { MENU_ITEM_SUBMENU, "视图", "", MENU_ID_VIEW, -1 },

    { MENU_ITEM_ACTION, "放大", "Ctrl++", MENU_ID_VIEW_ZOOM_IN, MENU_ID_VIEW },
    { MENU_ITEM_ACTION, "缩小", "Ctrl+-", MENU_ID_VIEW_ZOOM_OUT, MENU_ID_VIEW },
    { MENU_ITEM_ACTION, "适应窗口", "Ctrl+0", MENU_ID_VIEW_ZOOM_FIT, MENU_ID_VIEW },

    /* Separator */
    { MENU_ITEM_SEPARATOR, "", "", -1, MENU_ID_VIEW },

    { MENU_ITEM_ACTION, "显示/隐藏网格", "", MENU_ID_VIEW_TOGGLE_GRID, MENU_ID_VIEW },
    { MENU_ITEM_ACTION, "显示/隐藏检查器", "", MENU_ID_VIEW_TOGGLE_INSPECTOR, MENU_ID_VIEW },

    { MENU_ITEM_SUBMENU, "帮助", "", MENU_ID_HELP, -1 },

    { MENU_ITEM_ACTION, "键盘快捷键", "?", MENU_ID_HELP_SHORTCUTS, MENU_ID_HELP },
    { MENU_ITEM_ACTION, "关于", "", MENU_ID_HELP_ABOUT, MENU_ID_HELP },
};

static MenuItemDef* g_menu_items = NULL;
static int g_menu_item_count = 0;
static int g_menu_item_capacity = 0;

static void ui_menu_def_rebuild(void)
{
    int i = 0;
    int tool_count = 0;
    int needed = 0;
    int core_count = (int)(sizeof(g_core_menu_items) / sizeof(g_core_menu_items[0]));

    tool_count = tool_registry_count();
    needed = core_count + tool_count;
    if (needed > g_menu_item_capacity) {
        MenuItemDef* new_items =
            (MenuItemDef*)realloc(g_menu_items, (size_t)needed * sizeof(g_menu_items[0]));
        if (!new_items) {
            return;
        }
        g_menu_items = new_items;
        g_menu_item_capacity = needed;
    }

    memcpy(g_menu_items, g_core_menu_items, sizeof(g_core_menu_items));
    g_menu_item_count = core_count;

    for (i = 0; i < tool_count; ++i) {
        const ToolDescriptor* descriptor = tool_registry_at(i);
        if (!descriptor || !descriptor->command_id) {
            continue;
        }

        g_menu_items[g_menu_item_count].type = MENU_ITEM_ACTION;
        g_menu_items[g_menu_item_count].label = descriptor->name;
        g_menu_items[g_menu_item_count].shortcut = "";
        g_menu_items[g_menu_item_count].id = MENU_ID_TOOL_DYNAMIC_BASE + i;
        g_menu_items[g_menu_item_count].parent_id = MENU_ID_EDIT;
        g_menu_item_count++;
    }
}

/**
 * @brief Get the number of menu item definitions.
 * @return Total count of entries in the menu definition table.
 */

/**
 * @brief Gets the number of menu item definitions.
 * @param void No parameters.
 * @return Menu definition count.
 */
int ui_menu_def_count(void)
{
    ui_menu_def_rebuild();
    return g_menu_item_count;
}

/**
 * @brief Get the menu item definition table.
 * @return Pointer to the static menu item array.
 */

/**
 * @brief Gets the menu item definition table.
 * @param void No parameters.
 * @return Pointer to menu definition array.
 */
const MenuItemDef* ui_menu_def_items(void)
{
    ui_menu_def_rebuild();
    return g_menu_items;
}
