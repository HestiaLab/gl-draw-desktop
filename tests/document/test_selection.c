#include <model/selection.h>

#include <inttypes.h>
#include <stdio.h>

#include "test_assertions.h"

static void test_selection(void)
{
    SelectionSet selection = { 0 };
    SelectionSet copy = { 0 };

    EXPECT_UINT64_EQ(selection.revision, 0u);
    EXPECT_TRUE(selection_set_add(&selection, 7u));
    EXPECT_UINT64_EQ(selection.revision, 1u);
    EXPECT_TRUE(selection_set_contains(&selection, 7u));
    EXPECT_INT_EQ(selection.count, 1);

    EXPECT_TRUE(selection_set_add(&selection, 7u));
    EXPECT_UINT64_EQ(selection.revision, 1u);

    selection_set_toggle(&selection, 7u);
    EXPECT_UINT64_EQ(selection.revision, 2u);
    EXPECT_FALSE(selection_set_contains(&selection, 7u));
    EXPECT_INT_EQ(selection.count, 0);

    EXPECT_TRUE(selection_set_add(&selection, 9u));
    EXPECT_UINT64_EQ(selection.revision, 3u);
    EXPECT_TRUE(selection_set_copy(&copy, &selection));
    EXPECT_UINT64_EQ(copy.revision, 1u);
    EXPECT_TRUE(selection_set_copy(&copy, &selection));
    EXPECT_UINT64_EQ(copy.revision, 1u);

    selection_set_clear(&selection);
    EXPECT_UINT64_EQ(selection.revision, 4u);

    selection_set_shutdown(&copy);
    selection_set_shutdown(&selection);
}

void setUp(void) {}

void tearDown(void) {}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_selection);
    return UNITY_END();
}
