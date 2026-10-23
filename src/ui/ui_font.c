/**
 * @file ui_font.c
 * @brief 中文界面字体查找与加载。
 */
#include <ui/ui_system_internal.h>

#include "ui_font_glyph_ranges.h"

#include <base/resource_path.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UI_FONT_PATH_MAX 1024
#define UI_FONT_HEIGHT 16.0f
#define UI_FONT_BUNDLED_FILE "NotoSansSC-Regular.ttf"

static int ui_font_file_exists(const char* path)
{
    FILE* file = NULL;

    if (!path || path[0] == '\0') {
        return 0;
    }

    file = fopen(path, "rb");
    if (!file) {
        return 0;
    }
    fclose(file);
    return 1;
}

static int ui_font_copy_path(const char* path, char* output, size_t output_size)
{
    int written = snprintf(output, output_size, "%s", path);

    return written > 0 && (size_t)written < output_size;
}

static int ui_font_resolve_bundled(char* output, size_t output_size)
{
    static const char* const kCandidates[] = { "fonts/" UI_FONT_BUNDLED_FILE,
                                               "assets/fonts/" UI_FONT_BUNDLED_FILE };
    const char* override_path = getenv("GLDRAWDESKTOP_FONT");
    size_t index = 0u;

    if (override_path && override_path[0] != '\0' && ui_font_file_exists(override_path)) {
        return ui_font_copy_path(override_path, output, output_size);
    }

    for (index = 0u; index < sizeof(kCandidates) / sizeof(kCandidates[0]); ++index) {
        char resolved[UI_FONT_PATH_MAX];
        if (resource_path_resolve(kCandidates[index], resolved, sizeof(resolved)) &&
            ui_font_file_exists(resolved)) {
            return ui_font_copy_path(resolved, output, output_size);
        }
    }

    return 0;
}

static int ui_font_resolve_system(char* output, size_t output_size)
{
    static const char* const kCandidates[] = {
#if defined(_WIN32)
        "C:/Windows/Fonts/simhei.ttf",
        "C:/Windows/Fonts/msyh.ttc",
        "C:/Windows/Fonts/simsun.ttc",
#elif defined(__APPLE__)
        "/System/Library/Fonts/PingFang.ttc",
        "/System/Library/Fonts/Hiragino Sans GB.ttc",
#else
        "/usr/share/fonts/truetype/noto/NotoSansCJK-Regular.ttc",
        "/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc",
        "/usr/share/fonts/truetype/wqy/wqy-zenhei.ttc",
#endif
    };
    size_t index = 0u;

    for (index = 0u; index < sizeof(kCandidates) / sizeof(kCandidates[0]); ++index) {
        if (ui_font_file_exists(kCandidates[index])) {
            return ui_font_copy_path(kCandidates[index], output, output_size);
        }
    }

    return 0;
}

struct nk_font* ui_font_load_default(struct nk_font_atlas* atlas)
{
    char path[UI_FONT_PATH_MAX] = { 0 };
    struct nk_font_config config = { 0 };

    if (!atlas) {
        return NULL;
    }

    if (!ui_font_resolve_bundled(path, sizeof(path)) &&
        !ui_font_resolve_system(path, sizeof(path))) {
        fputs("界面字体加载失败：未找到中文字体。\n"
              "请确认安装目录包含 share/gldrawdesktop/fonts/NotoSansSC-Regular.ttf，\n"
              "或通过 GLDRAWDESKTOP_FONT 环境变量指定字体文件。\n",
              stderr);
        return NULL;
    }

    config = nk_font_config(UI_FONT_HEIGHT);
    config.range = UI_GB2312_GLYPH_RANGES;
    config.oversample_h = 1;
    config.oversample_v = 1;
    config.fallback_glyph = (nk_rune)'?';

    {
        struct nk_font* font = nk_font_atlas_add_from_file(atlas, path, UI_FONT_HEIGHT, &config);
        if (font) {
            fprintf(stderr, "界面字体已加载：%s\n", path);
        }
        return font;
    }
}
