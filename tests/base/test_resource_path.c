#include <base/resource_path.h>

#include <stdio.h>
#include <string.h>

#include <unity.h>

static int write_marker_file(const char* path)
{
    FILE* file = fopen(path, "wb");
    if (!file) {
        return 0;
    }
    fputs("resource-path-test", file);
    fclose(file);
    return 1;
}

static void test_resource_path_lookup(void)
{
    char resolved[260];
    const char* marker_path = "gldraw_resource_path_test.tmp";

    remove(marker_path);
    TEST_ASSERT_TRUE_MESSAGE(write_marker_file(marker_path), "无法创建标记文件");

    TEST_ASSERT_TRUE_MESSAGE(resource_path_resolve(marker_path, resolved, sizeof(resolved)),
                             "当前目录资源应能解析");
    TEST_ASSERT_EQUAL_STRING(marker_path, resolved);

    remove(marker_path);

    TEST_ASSERT_FALSE_MESSAGE(
        resource_path_resolve("missing-resource-path-test.file", resolved, sizeof(resolved)),
        "缺失资源应报告未解析");
    TEST_ASSERT_EQUAL_STRING("missing-resource-path-test.file", resolved);
}

void setUp(void) {}

void tearDown(void) {}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_resource_path_lookup);
    return UNITY_END();
}
