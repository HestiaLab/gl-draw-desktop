function(gldrawdesktop_target_warnings target)
    if(MSVC)
        # 源文件与用户可见文案统一 UTF-8，避免窄字符串被按本地代码页解释。
        target_compile_options(${target} PRIVATE /W4 /permissive- /utf-8)
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wconversion
        )
    endif()
endfunction()

function(gldrawdesktop_target_sanitizers target)
    if(GLDRAWDESKTOP_ENABLE_ASAN)
        if(MSVC)
            target_compile_options(${target} PRIVATE /fsanitize=address)
            target_link_options(${target} PRIVATE /fsanitize=address)
        else()
            target_compile_options(${target} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
            target_link_options(${target} PRIVATE -fsanitize=address)
        endif()
    endif()

    if(GLDRAWDESKTOP_ENABLE_UBSAN)
        if(NOT MSVC)
            target_compile_options(${target} PRIVATE -fsanitize=undefined)
            target_link_options(${target} PRIVATE -fsanitize=undefined)
        else()
            message(WARNING
                "GLDRAWDESKTOP_ENABLE_UBSAN 为 ON，但本模板未为 MSVC 配置 UBSAN。")
        endif()
    endif()
endfunction()

function(gldrawdesktop_target_coverage target)
    if(NOT GLDRAWDESKTOP_ENABLE_COVERAGE)
        return()
    endif()

    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        target_compile_options(${target} PRIVATE --coverage -O0 -g)
        target_link_options(${target} PRIVATE --coverage)
    else()
        message(WARNING
            "GLDRAWDESKTOP_ENABLE_COVERAGE 为 ON，但覆盖率选项仅对 GCC 与 Clang 系编译器配置。")
    endif()
endfunction()

function(gldrawdesktop_target_defaults target_name)
    target_include_directories(${target_name}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_compile_features(${target_name} PRIVATE c_std_11)
    gldrawdesktop_target_warnings(${target_name})
    gldrawdesktop_target_sanitizers(${target_name})
    gldrawdesktop_target_coverage(${target_name})
endfunction()

function(gldrawdesktop_target_glfw_defaults target_name)
    gldrawdesktop_target_defaults(${target_name})
    target_include_directories(${target_name}
        PRIVATE
            "${glfw_SOURCE_DIR}/include"
    )
endfunction()

function(gldrawdesktop_apply_source_compile_options)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c
            PROPERTIES COMPILE_FLAGS "-Wno-pedantic"
        )
    endif()

    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
        set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/ui/ui_runtime.c
            PROPERTIES COMPILE_FLAGS "-Wno-c23-extensions"
        )
    endif()
endfunction()
