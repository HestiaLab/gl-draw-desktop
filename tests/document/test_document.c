#include <document/document_internal.h>
#include <document/object.h>

#include <app/extension_loader.h>

#include <stdio.h>

#include "test_assertions.h"

static void test_document(void)
{
    Document document;
    GraphicObject* object = NULL;

    extension_loader_register_all();

    document_init(&document);
    object = object_create_rect((RectF){ 0.0f, 0.0f, 10.0f, 12.0f }, object_default_style());

    EXPECT_TRUE(object != NULL);
    EXPECT_TRUE(document_add_object(&document, object));
    EXPECT_INT_EQ(document.count, 1);
    EXPECT_TRUE(document_find_object(&document, 1u) == object);

    document_shutdown(&document);
}

void setUp(void) {}

void tearDown(void) {}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_document);
    return UNITY_END();
}
