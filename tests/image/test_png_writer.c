#include <image/png_writer.h>

#include "../support/test_temp_files.h"

#include <stdio.h>
#include <string.h>

#include <unity.h>

static unsigned int read_u32_be(const unsigned char bytes[4])
{
    return ((unsigned int)bytes[0] << 24) | ((unsigned int)bytes[1] << 16) |
           ((unsigned int)bytes[2] << 8) | (unsigned int)bytes[3];
}

static void test_png_writer_creates_rgba_header(void)
{
    static const unsigned char expected_signature[8] = { 137u, 80u, 78u, 71u, 13u, 10u, 26u, 10u };
    const unsigned char pixels[16] = { 255u, 0u, 0u,   255u, 0u,   255u, 0u,   255u,
                                       0u,   0u, 255u, 255u, 255u, 255u, 255u, 255u };
    unsigned char header[33];
    char path[TEST_TEMP_PATH_MAX];
    FILE* file = NULL;

    TEST_ASSERT_TRUE_MESSAGE(test_temp_make_path(path, sizeof(path), "png-writer", ".png"),
                             "无法创建临时路径");
    TEST_ASSERT_TRUE_MESSAGE(png_writer_write_rgba_file(path, pixels, 2, 2), "写入 PNG 失败");

    file = fopen(path, "rb");
    TEST_ASSERT_NOT_NULL_MESSAGE(file, "无法重新打开 PNG");
    TEST_ASSERT_EQUAL_UINT(sizeof(header), fread(header, 1u, sizeof(header), file));
    fclose(file);
    remove(path);

    TEST_ASSERT_EQUAL_MEMORY(expected_signature, header, sizeof(expected_signature));
    TEST_ASSERT_EQUAL_UINT(13u, read_u32_be(&header[8]));
    TEST_ASSERT_EQUAL_MEMORY("IHDR", &header[12], 4u);
    TEST_ASSERT_EQUAL_UINT(2u, read_u32_be(&header[16]));
    TEST_ASSERT_EQUAL_UINT(2u, read_u32_be(&header[20]));
    TEST_ASSERT_EQUAL_UINT(8u, header[24]);
    TEST_ASSERT_EQUAL_UINT(6u, header[25]);
}

void setUp(void) {}

void tearDown(void) {}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_png_writer_creates_rgba_header);
    return UNITY_END();
}
