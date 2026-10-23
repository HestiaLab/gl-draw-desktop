# 应用型项目的安装与 CPack 打包规则。
#
# 本项目只分发可执行文件、运行时资源与文档，不导出库或 CMake 包。
include(GNUInstallDirs)

if(TARGET gldrawdesktop)
    install(TARGETS gldrawdesktop
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        BUNDLE DESTINATION .
    )
endif()

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/shaders/"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/gldrawdesktop/shaders"
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/themes")
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/themes/"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/gldrawdesktop/themes"
    )
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/scripts")
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/scripts/"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/gldrawdesktop/scripts"
    )
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets/fonts")
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets/fonts/"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/gldrawdesktop/fonts"
    )
endif()

install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
    "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
)

set(CPACK_PACKAGE_INSTALL_DIRECTORY "GlDrawDesktop")
set(CPACK_PACKAGE_NAME "gl-draw-desktop")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_VENDOR "${GLDRAWDESKTOP_PROJECT_AUTHOR}")

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CPACK_GENERATOR "TGZ")
else()
    find_program(CPACK_DEB_EXECUTABLE dpkg)
    find_program(CPACK_RPM_EXECUTABLE rpmbuild)
    set(CPACK_GENERATOR "TGZ")
    if(CPACK_DEB_EXECUTABLE)
        list(APPEND CPACK_GENERATOR "DEB")
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${GLDRAWDESKTOP_PROJECT_AUTHOR}")
        set(CPACK_DEBIAN_PACKAGE_SECTION "graphics")
        set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    endif()
    if(CPACK_RPM_EXECUTABLE)
        list(APPEND CPACK_GENERATOR "RPM")
        set(CPACK_RPM_PACKAGE_LICENSE "${GLDRAWDESKTOP_PROJECT_LICENSE}")
        set(CPACK_RPM_PACKAGE_GROUP "Applications/Graphics")
    endif()
endif()

set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_IGNORE_FILES
    "/build/"
    "/_CPack_Packages/"
    "/\\\\.git/"
    "/\\\\.vs/"
    "/\\\\.cache/"
    "/install/"
    "/install-"
    "/dist/"
    "/out/"
    "\\\\.swp$"
    "\\\\.swo$"
    "\\\\.user$"
    "\\\\.idx$"
)

include(CPack)
