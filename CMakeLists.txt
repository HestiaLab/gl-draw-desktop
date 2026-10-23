cmake_minimum_required(VERSION 3.21...3.31)

project(GlDrawDesktop
    VERSION 3.0.0
    DESCRIPTION "基于 OpenGL 3.3 与 Nuklear 的画布绘图编辑器"
    LANGUAGES C
)
set(GLDRAWDESKTOP_PROJECT_AUTHOR "HestiaLab")
set(GLDRAWDESKTOP_PROJECT_LICENSE "MIT")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(GLDRAWDESKTOP_IS_TOP_LEVEL_DEFAULT ON)
else()
    set(GLDRAWDESKTOP_IS_TOP_LEVEL_DEFAULT OFF)
endif()

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "构建类型" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(GLDRAWDESKTOP_ENABLE_SCRIPTING "启用 Lua 脚本支持" OFF)
option(GLDRAWDESKTOP_ENABLE_ASAN "启用 AddressSanitizer（支持的平台上）" OFF)
option(GLDRAWDESKTOP_ENABLE_UBSAN "启用 UndefinedBehaviorSanitizer（支持的平台上）" OFF)
option(GLDRAWDESKTOP_ENABLE_COVERAGE "启用覆盖率编译选项（支持的平台上）" OFF)
option(GLDRAWDESKTOP_BUILD_APP "构建 GlDrawDesktop 应用" ${GLDRAWDESKTOP_IS_TOP_LEVEL_DEFAULT})
option(GLDRAWDESKTOP_BUILD_TESTING "构建 CTest 测试" ${GLDRAWDESKTOP_IS_TOP_LEVEL_DEFAULT})
option(GLDRAWDESKTOP_INSTALL "生成安装规则" ${GLDRAWDESKTOP_IS_TOP_LEVEL_DEFAULT})

file(TO_CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" GLDRAWDESKTOP_RESOURCE_ROOT)

set(GLDRAWDESKTOP_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated/include")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/gldrawdesktop_version.h.in"
    "${GLDRAWDESKTOP_GENERATED_INCLUDE_DIR}/gldrawdesktop/version.h"
    @ONLY
)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/CompilerOptions.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Dependencies.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Sources.cmake")

gldrawdesktop_apply_source_compile_options()

add_library(editor_model STATIC ${EDITOR_MODEL_SOURCES})
gldrawdesktop_target_defaults(editor_model)
target_compile_definitions(editor_model PUBLIC GLDRAW_RESOURCE_ROOT="${GLDRAWDESKTOP_RESOURCE_ROOT}")
if(UNIX AND NOT APPLE)
    target_link_libraries(editor_model PUBLIC m)
endif()

add_library(render_core STATIC ${RENDER_CORE_SOURCES})
gldrawdesktop_target_defaults(render_core)
target_link_libraries(render_core PUBLIC editor_model)

add_library(platform_glfw STATIC ${PLATFORM_GLFW_SOURCES})
gldrawdesktop_target_glfw_defaults(platform_glfw)
target_link_libraries(platform_glfw
    PUBLIC
        editor_model
    PRIVATE
        glfw
        OpenGL::GL
)

add_library(render_glfw_gl STATIC ${RENDER_GLFW_GL_SOURCES})
gldrawdesktop_target_glfw_defaults(render_glfw_gl)
target_link_libraries(render_glfw_gl
    PUBLIC
        render_core
    PRIVATE
        glfw
        OpenGL::GL
)

add_library(editor_commands STATIC ${EDITOR_COMMANDS_SOURCES})
gldrawdesktop_target_defaults(editor_commands)
target_link_libraries(editor_commands PUBLIC editor_model)

add_library(editor_tools STATIC ${EDITOR_TOOLS_SOURCES})
gldrawdesktop_target_glfw_defaults(editor_tools)
target_link_libraries(editor_tools
    PUBLIC
        editor_commands
        render_core
    PRIVATE
        glfw
)
if(GLDRAWDESKTOP_ENABLE_SCRIPTING)
    target_include_directories(editor_tools PRIVATE ${LUA_INCLUDE_DIR})
    target_link_libraries(editor_tools PRIVATE ${LUA_LIBRARIES})
    target_compile_definitions(editor_tools PRIVATE GLDRAW_ENABLE_SCRIPTING=1)
endif()

add_library(editor_runtime STATIC ${EDITOR_RUNTIME_SOURCES})
gldrawdesktop_target_glfw_defaults(editor_runtime)
target_link_libraries(editor_runtime
    PUBLIC
        editor_commands
        editor_tools
    PRIVATE
        render_core
        glfw
)

add_library(ui_nuklear_glfw STATIC ${UI_NUKLEAR_GLFW_SOURCES})
gldrawdesktop_target_glfw_defaults(ui_nuklear_glfw)
target_link_libraries(ui_nuklear_glfw
    PUBLIC
        editor_runtime
        platform_glfw
)

add_library(editor_app STATIC ${EDITOR_APP_SOURCES})
gldrawdesktop_target_glfw_defaults(editor_app)
target_include_directories(editor_app
    PUBLIC
        "${GLDRAWDESKTOP_GENERATED_INCLUDE_DIR}"
)
target_link_libraries(editor_app
    PUBLIC
        editor_runtime
        ui_nuklear_glfw
        render_glfw_gl
        platform_glfw
)
if(WIN32)
    target_link_libraries(editor_app PUBLIC gdi32 comdlg32)
endif()
if(UNIX AND NOT APPLE)
    target_link_libraries(editor_app PUBLIC pthread dl)
endif()

if(GLDRAWDESKTOP_BUILD_APP)
    add_executable(gldrawdesktop
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    )

    target_include_directories(gldrawdesktop
        PRIVATE
            "${GLDRAWDESKTOP_GENERATED_INCLUDE_DIR}"
    )
    target_link_libraries(gldrawdesktop PRIVATE editor_app)
    gldrawdesktop_target_defaults(gldrawdesktop)
    set_target_properties(gldrawdesktop PROPERTIES
        OUTPUT_NAME "gl-draw-desktop"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    add_custom_command(TARGET gldrawdesktop POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
            "$<TARGET_FILE_DIR:gldrawdesktop>/shaders"
    )

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/themes")
        add_custom_command(TARGET gldrawdesktop POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/themes"
                "$<TARGET_FILE_DIR:gldrawdesktop>/themes"
        )
    endif()

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/scripts")
        add_custom_command(TARGET gldrawdesktop POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/scripts"
                "$<TARGET_FILE_DIR:gldrawdesktop>/scripts"
        )
    endif()

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets/fonts")
        add_custom_command(TARGET gldrawdesktop POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/assets/fonts"
                "$<TARGET_FILE_DIR:gldrawdesktop>/fonts"
            COMMENT "复制界面字体到输出目录"
        )
    endif()
endif()

if(GLDRAWDESKTOP_BUILD_TESTING)
    enable_testing()
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Tests.cmake")
endif()

if(GLDRAWDESKTOP_INSTALL)
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Packaging.cmake")
endif()

message(STATUS "----------------------------------")
message(STATUS "项目: ${PROJECT_NAME}")
message(STATUS "版本: ${PROJECT_VERSION}")
message(STATUS "系统: ${CMAKE_SYSTEM_NAME}")
message(STATUS "编译器: ${CMAKE_C_COMPILER}")
message(STATUS "----------------------------------")
