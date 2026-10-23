#ifndef GLDRAW_TESTS_TEST_ASSERTIONS_H
#define GLDRAW_TESTS_TEST_ASSERTIONS_H

#include <string.h>

#include <unity.h>

/*
 * 项目测试沿用的断言词汇，全部映射到 vendored Unity。
 * 这样可以保留既有测试意图，同时由 Unity 统一报告失败与统计。
 */
#define EXPECT_TRUE(expr) TEST_ASSERT_TRUE(expr)
#define EXPECT_FALSE(expr) TEST_ASSERT_FALSE(expr)
#define EXPECT_INT_EQ(actual, expected) TEST_ASSERT_EQUAL_INT((expected), (actual))
#define EXPECT_UINT_EQ(actual, expected) TEST_ASSERT_EQUAL_UINT((expected), (actual))
#define EXPECT_UINT64_EQ(actual, expected) TEST_ASSERT_EQUAL_UINT64((expected), (actual))
#define EXPECT_FLOAT_EQ(actual, expected) TEST_ASSERT_FLOAT_WITHIN(1e-5f, (expected), (actual))
#define EXPECT_STR_EQ(actual, expected) TEST_ASSERT_EQUAL_STRING((expected), (actual))
#define EXPECT_SUBSTR(actual, expected)                                                            \
    TEST_ASSERT_NOT_NULL_MESSAGE(strstr((actual), (expected)), "未找到期望的子串")

#endif /* GLDRAW_TESTS_TEST_ASSERTIONS_H */
