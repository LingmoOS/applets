set(PUBLIC_HEADERS
    ${CMAKE_SOURCE_DIR}/include/LingmoApplets/Applet.h
    ${CMAKE_SOURCE_DIR}/include/LingmoApplets/AppletContext.h
    ${CMAKE_SOURCE_DIR}/include/LingmoApplets/AppletFactory.h
    ${CMAKE_SOURCE_DIR}/include/LingmoApplets/AppletHost.h
)

set(PRIVATE_HEADERS
    LingmoApplets/private/Applet_p.h
    LingmoApplets/private/AppletContext_p.h
    LingmoApplets/private/AppletHost_p.h
)

set(SOURCES
    LingmoApplets/Applet.cpp
    LingmoApplets/AppletContext.cpp
    LingmoApplets/AppletFactory.cpp
    LingmoApplets/AppletHost.cpp
)

set(CMAKE_AUTOMOC ON)

add_library(LingmoApplets SHARED ${SOURCES} ${PUBLIC_HEADERS} ${PRIVATE_HEADERS})
add_library(Lingmo::Applets ALIAS LingmoApplets)

target_include_directories(LingmoApplets
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(LingmoApplets
    PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::DBus
    PRIVATE
        Lingmo::Plugin
)

target_compile_definitions(LingmoApplets
    PRIVATE LINGMOAPPLETS_LIBRARY
)

set_target_properties(LingmoApplets PROPERTIES
    SOVERSION 1
    VERSION   ${PROJECT_VERSION}
    DEFINE_SYMBOL LINGMOAPPLETS_LIBRARY
    OUTPUT_NAME LingmoApplets
)

configure_file(
    "${CMAKE_SOURCE_DIR}/include/LingmoApplets/LingmoAppletsExport.h.in"
    "${CMAKE_BINARY_DIR}/include/LingmoApplets/LingmoAppletsExport.h"
)

configure_file(
    "${CMAKE_SOURCE_DIR}/include/LingmoApplets/LingmoAppletsVersion.h.in"
    "${CMAKE_BINARY_DIR}/include/LingmoApplets/LingmoAppletsVersion.h"
)

install(TARGETS LingmoApplets
    EXPORT LingmoAppletsTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/LingmoApplets/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/LingmoApplets
    FILES_MATCHING PATTERN "*.h"
    PATTERN "*.h.in" EXCLUDE
)

install(FILES
    "${CMAKE_BINARY_DIR}/include/LingmoApplets/LingmoAppletsExport.h"
    "${CMAKE_BINARY_DIR}/include/LingmoApplets/LingmoAppletsVersion.h"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/LingmoApplets
)

install(EXPORT LingmoAppletsTargets
    FILE      LingmoAppletsTargets.cmake
    NAMESPACE Lingmo::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LingmoApplets
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/LingmoAppletsConfig.cmake.in"
    "${CMAKE_BINARY_DIR}/LingmoAppletsConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LingmoApplets
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/LingmoAppletsConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_BINARY_DIR}/LingmoAppletsConfig.cmake"
    "${CMAKE_BINARY_DIR}/LingmoAppletsConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LingmoApplets
)
