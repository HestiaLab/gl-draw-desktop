function(gldrawdesktop_add_test target)
    cmake_parse_arguments(GLDRAWDESKTOP_TEST
        ""
        ""
        "SOURCES;LIBS"
        ${ARGN}
    )

    if(NOT GLDRAWDESKTOP_TEST_SOURCES)
        message(FATAL_ERROR "gldrawdesktop_add_test(${target}) requires SOURCES")
    endif()

    add_executable(${target}
        ${GLDRAWDESKTOP_TEST_SOURCES}
    )

    target_include_directories(${target}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${GLDRAWDESKTOP_GENERATED_INCLUDE_DIR}
    )

    target_link_libraries(${target} PRIVATE
        gldrawdesktop_vendor
        glfw
        OpenGL::GL
    )

    if(GLDRAWDESKTOP_TEST_LIBS)
        target_link_libraries(${target} PRIVATE ${GLDRAWDESKTOP_TEST_LIBS})
    endif()

    gldrawdesktop_target_defaults(${target})

    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    add_test(NAME ${target} COMMAND $<TARGET_FILE:${target}>)
    list(APPEND GLDRAWDESKTOP_TEST_TARGETS ${target})
    set(GLDRAWDESKTOP_TEST_TARGETS "${GLDRAWDESKTOP_TEST_TARGETS}" PARENT_SCOPE)
endfunction()

gldrawdesktop_add_test(gldraw_document_core_tests
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/document/test_document_core.c
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/support/test_temp_files.c
    LIBS
        editor_runtime
)

gldrawdesktop_add_test(gldraw_png_writer_tests
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/image/test_png_writer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/support/test_temp_files.c
    LIBS
        render_core
)

gldrawdesktop_add_test(gldraw_selection_tests
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/document/test_selection.c
    LIBS
        editor_model
)

gldrawdesktop_add_test(gldraw_resource_path_tests
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/base/test_resource_path.c
    LIBS
        editor_model
)

gldrawdesktop_add_test(gldraw_document_tests
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/document/test_document.c
    LIBS
        editor_model
)

gldrawdesktop_add_test(gldraw_command_tests
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/commands/test_commands.c
    LIBS
        editor_runtime
)

gldrawdesktop_add_test(gldraw_registry_tests
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/app/test_registry.c
    LIBS
        ui_nuklear_glfw
)

if(GLDRAWDESKTOP_ENABLE_SCRIPTING)
    gldrawdesktop_add_test(gldraw_script_runtime_tests
        SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/script/test_script_runtime.c
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/support/test_temp_files.c
        LIBS
            editor_tools
    )
endif()

gldrawdesktop_add_test(gldraw_fake_star_tests
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/document/test_fake_star.c
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/support/test_temp_files.c
    LIBS
        render_core
)

gldrawdesktop_add_test(gldraw_no_builtin_knowledge_tests
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/app/test_no_builtin_knowledge.c
    LIBS
        editor_model
)

gldrawdesktop_add_test(gldraw_ui_logic_tests
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/ui/test_ui_logic.c
    LIBS
        editor_runtime
)

gldrawdesktop_add_test(gldraw_ui_theme_tests
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/ui/test_ui_theme.c
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/support/test_temp_files.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/ui_theme.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/ui_theme_apply.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/ui_theme_builtin.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/ui_theme_external.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/ui_theme_parse.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/ui_theme_settings.c
    LIBS
        editor_model
)

target_compile_options(gldraw_ui_theme_tests
    PRIVATE
        $<$<COMPILE_LANG_AND_ID:C,GNU,Clang,AppleClang>:-Wno-unused-function>
        $<$<COMPILE_LANG_AND_ID:C,MSVC>:/wd4505>
)

gldrawdesktop_add_test(gldraw_renderer_tests
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/render/test_renderer.c
    LIBS
        render_core
)

gldrawdesktop_add_test(gldraw_workspace_service_tests
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/app/test_workspace_service.c
    LIBS
        editor_runtime
)

add_custom_target(gldrawdesktop_tests
    DEPENDS
        ${GLDRAWDESKTOP_TEST_TARGETS}
)
