/**
 * @file test_no_builtin_knowledge.c
 * @brief Verify that object.c has no hardcoded type knowledge and that
 * all types (line, rect, ellipse, fake_star) are registered via the
 * extension loader, not via builtin bootstrapping inside object.c.
 */
#include <app/extension_loader.h>
#include <document/object.h>

#include <stdio.h>
#include <string.h>

#include "test_assertions.h"

static void test_no_builtin_knowledge(void)
{
    /* ------------------------------------------------------------------ */
    /* 1. Before any registration, the registry should be empty and        */
    /*    lookups for builtin types should return NULL.                    */
    /* ------------------------------------------------------------------ */
    EXPECT_TRUE(object_registry_count() == 0);
    EXPECT_TRUE(object_registry_lookup("line") == NULL);
    EXPECT_TRUE(object_registry_lookup("rect") == NULL);
    EXPECT_TRUE(object_registry_lookup("ellipse") == NULL);
    EXPECT_TRUE(object_registry_lookup("fake_star") == NULL);
    EXPECT_TRUE(object_create("line", NULL, object_default_style()) == NULL);
    EXPECT_TRUE(object_create("rect", NULL, object_default_style()) == NULL);

    /* ------------------------------------------------------------------ */
    /* 2. After registering builtins, all 3 core types must be present.   */
    /* ------------------------------------------------------------------ */
    EXPECT_TRUE(extension_loader_register_builtins());
    EXPECT_TRUE(object_registry_count() == 3);

    EXPECT_TRUE(object_registry_lookup("line") != NULL);
    EXPECT_STR_EQ(object_registry_lookup("line")->type_id, "line");

    EXPECT_TRUE(object_registry_lookup("rect") != NULL);
    EXPECT_STR_EQ(object_registry_lookup("rect")->type_id, "rect");

    EXPECT_TRUE(object_registry_lookup("ellipse") != NULL);
    EXPECT_STR_EQ(object_registry_lookup("ellipse")->type_id, "ellipse");

    /* ------------------------------------------------------------------ */
    /* 3. After register_all, fake_star is also present.                  */
    /* ------------------------------------------------------------------ */
    EXPECT_TRUE(extension_loader_register_all());
    EXPECT_TRUE(object_registry_count() >= 4);
    EXPECT_TRUE(object_registry_lookup("fake_star") != NULL);

    /* ------------------------------------------------------------------ */
    /* 4. Re-registration is idempotent and does not change the count.    */
    /* ------------------------------------------------------------------ */
    {
        int count = object_registry_count();
        EXPECT_TRUE(extension_loader_register_all());
        EXPECT_TRUE(object_registry_count() == count);
    }

    /* ------------------------------------------------------------------ */
    /* 5. Convenience constructors work after registration.               */
    /* ------------------------------------------------------------------ */
    {
        GraphicObject* line = object_create_line(
            (Vec2){ 0.0f, 0.0f }, (Vec2){ 10.0f, 10.0f }, object_default_style());
        EXPECT_TRUE(line != NULL);
        EXPECT_STR_EQ(object_type_id(line), "line");
        object_destroy(line);

        GraphicObject* rect =
            object_create_rect((RectF){ 0.0f, 0.0f, 10.0f, 10.0f }, object_default_style());
        EXPECT_TRUE(rect != NULL);
        EXPECT_STR_EQ(object_type_id(rect), "rect");
        object_destroy(rect);

        GraphicObject* ellipse =
            object_create_ellipse((RectF){ 0.0f, 0.0f, 10.0f, 10.0f }, object_default_style());
        EXPECT_TRUE(ellipse != NULL);
        EXPECT_STR_EQ(object_type_id(ellipse), "ellipse");
        object_destroy(ellipse);
    }
}

void setUp(void) {}

void tearDown(void) {}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_no_builtin_knowledge);
    return UNITY_END();
}
