# Helper macro for built-in applets
macro(add_lingmo_applet name applet_id)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    file(GLOB APPLET_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/${name}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/${name}/*.h"
    )

    add_library(lingmo-applet-${name} MODULE ${APPLET_SOURCES})

    target_include_directories(lingmo-applet-${name}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/${name}
    )

    target_link_libraries(lingmo-applet-${name}
        PRIVATE
            Lingmo::Applets
            Qt6::Core
            Qt6::Gui
            Qt6::Qml
            Qt6::Quick
            Qt6::DBus
    )

    set_target_properties(lingmo-applet-${name} PROPERTIES
        OUTPUT_NAME lingmo-applet-${name}
        PREFIX ""
    )

    install(TARGETS lingmo-applet-${name}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/lingmo/applets
    )

    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${name}/${applet_id}.lingmo-plugin"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/lingmo/plugins
    )

    # Install QML files if present
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${name}/qml")
        install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${name}/qml/"
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/lingmo/applets/${name}/qml
        )
    endif()
endmacro()

add_lingmo_applet(applicationmenu  org.lingmo.applet.applicationmenu)
add_lingmo_applet(taskmanager      org.lingmo.applet.taskmanager)
add_lingmo_applet(systemtray       org.lingmo.applet.systemtray)
add_lingmo_applet(digitalclock     org.lingmo.applet.digitalclock)
add_lingmo_applet(battery          org.lingmo.applet.battery)
add_lingmo_applet(volume           org.lingmo.applet.volume)
add_lingmo_applet(networkindicator org.lingmo.applet.networkindicator)
add_lingmo_applet(quicksettings    org.lingmo.applet.quicksettings)
add_lingmo_applet(spacer           org.lingmo.applet.spacer)
add_lingmo_applet(separator        org.lingmo.applet.separator)
