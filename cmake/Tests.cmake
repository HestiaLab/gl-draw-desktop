set(GLDRAWDESKTOP_UNITY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/vendor/unity/unity.c
)

set(GLDRAWDESKTOP_TEST_TARGETS "")

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
        ${GLDRAWDESKTOP_UNITY_SOURCES}
    )

    target_include_directories(${target}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${GLDRAWDESKTOP_GENERATED_INCLUDE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/support
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/vendor/unity
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
    set_tests_properties(${target} PROPERTIES LABELS "unit")
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

# 架构门禁：include/ 中的每个公共函数声明都必须在 src/ 中有定义。
# 下限取实测基线的 80%（2026-10-23：435 个声明 / 908 个定义）。
find_package(Python3 COMPONENTS Interpreter)
if(Python3_Interpreter_FOUND)
    add_test(NAME gldrawdesktop_decl_def_check
        COMMAND ${Python3_EXECUTABLE}
            ${CMAKE_CURRENT_SOURCE_DIR}/scripts/check_decl_def.py
            ${CMAKE_CURRENT_SOURCE_DIR}
            --min-declarations 348
            --min-definitions 726)
    set_tests_properties(gldrawdesktop_decl_def_check PROPERTIES LABELS "architecture")
else()
    message(FATAL_ERROR
        "未找到 Python3 解释器，但 GLDRAWDESKTOP_BUILD_TESTING 为 ON，"
        "架构门禁需要它。请安装 Python 3，或使用 "
        "-DGLDRAWDESKTOP_BUILD_TESTING=OFF 关闭测试。")
endif()

# 门禁自检：确认质量门禁脚本确实能够失败。
add_test(NAME gldrawdesktop_gate_behaviors
    COMMAND ${Python3_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/scripts/tests/test_gate_behaviors.py)
set_tests_properties(gldrawdesktop_gate_behaviors PROPERTIES LABELS "architecture")

# 版本门禁：CMake project VERSION、生成头文件、CHANGELOG 与 --version 输出一致。
set(GLDRAWDESKTOP_VERSION_GATE_ARGS
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/check_version_consistency.py
    ${CMAKE_CURRENT_SOURCE_DIR}
    --build-dir ${CMAKE_BINARY_DIR}
)
if(TARGET gldrawdesktop)
    list(APPEND GLDRAWDESKTOP_VERSION_GATE_ARGS --exe $<TARGET_FILE:gldrawdesktop>)
endif()
add_test(NAME gldrawdesktop_version_consistency
    COMMAND ${Python3_EXECUTABLE} ${GLDRAWDESKTOP_VERSION_GATE_ARGS}
)
set_tests_properties(gldrawdesktop_version_consistency PROPERTIES LABELS "architecture")
if(TARGET gldrawdesktop)
    set_tests_properties(gldrawdesktop_version_consistency PROPERTIES DEPENDS gldrawdesktop)
endif()

# 文档门禁：相对链接、锚点、代码围栏与 README 索引覆盖。
add_test(NAME gldrawdesktop_docs_check
    COMMAND ${Python3_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/scripts/check_docs.py
        ${CMAKE_CURRENT_SOURCE_DIR}
        --mode all)
set_tests_properties(gldrawdesktop_docs_check PROPERTIES LABELS "documentation")

if(GLDRAWDESKTOP_INSTALL)
    add_test(NAME gldrawdesktop_cpack_config
        COMMAND ${Python3_EXECUTABLE}
            ${CMAKE_CURRENT_SOURCE_DIR}/scripts/check-cpack-config.py
            ${CMAKE_CURRENT_BINARY_DIR}/CPackConfig.cmake
    )
    set_tests_properties(gldrawdesktop_cpack_config PROPERTIES LABELS "architecture")
endif()

add_custom_target(gldrawdesktop_tests
    DEPENDS
        ${GLDRAWDESKTOP_TEST_TARGETS}
)
