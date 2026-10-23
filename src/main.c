#include <gldrawdesktop/app.h>

#include <stdio.h>
#include <string.h>

static void print_usage(FILE* stream, const char* program)
{
    fprintf(stream,
            "用法：%s [选项]\n"
            "\n"
            "选项：\n"
            "  -h, --help     显示本帮助\n"
            "  -v, --version  显示版本号\n",
            program);
}

int main(int argc, char** argv)
{
    const char* program = (argc > 0 && argv[0] != NULL) ? argv[0] : "gl-draw-desktop";

    for (int i = 1; i < argc; ++i) {
        const char* arg = argv[i];
        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            print_usage(stdout, program);
            return 0;
        }
        if (strcmp(arg, "-v") == 0 || strcmp(arg, "--version") == 0) {
            printf("gl-draw-desktop %s\n", gldrawdesktop_version());
            return 0;
        }
        fprintf(stderr, "未知选项：%s\n", arg);
        print_usage(stderr, program);
        return 2;
    }

    return gldrawdesktop_run();
}
