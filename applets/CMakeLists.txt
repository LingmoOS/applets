set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Each applet is a MODULE (plugin .so) with explicit source lists.
# set_target_properties AUTOMOC ON is redundant when CMAKE_AUTOMOC is already
# ON at directory level, but we keep it explicit for clarity.

# ── Application Menu ──────────────────────────────────────────────────────────
add_library(lingmo-applet-applicationmenu MODULE
    applicationmenu/ApplicationMenuApplet.cpp
    applicationmenu/ApplicationMenuApplet.h
)
# ── Task Manager ──────────────────────────────────────────────────────────────
add_library(lingmo-applet-taskmanager MODULE
    taskmanager/TaskManagerApplet.cpp
    taskmanager/TaskManagerApplet.h
)
# ── System Tray ───────────────────────────────────────────────────────────────
add_library(lingmo-applet-systemtray MODULE
    systemtray/SystemTrayApplet.cpp
    systemtray/SystemTrayApplet.h
)
# ── Digital Clock ─────────────────────────────────────────────────────────────
add_library(lingmo-applet-digitalclock MODULE
    digitalclock/DigitalClockApplet.cpp
    digitalclock/DigitalClockApplet.h
)
# ── Battery ───────────────────────────────────────────────────────────────────
add_library(lingmo-applet-battery MODULE
    battery/BatteryApplet.cpp
    battery/BatteryApplet.h
)
# ── Volume ────────────────────────────────────────────────────────────────────
add_library(lingmo-applet-volume MODULE
    volume/VolumeApplet.cpp
    volume/VolumeApplet.h
)
# ── Network Indicator ─────────────────────────────────────────────────────────
add_library(lingmo-applet-networkindicator MODULE
    networkindicator/NetworkIndicatorApplet.cpp
    networkindicator/NetworkIndicatorApplet.h
)
# ── Quick Settings ────────────────────────────────────────────────────────────
add_library(lingmo-applet-quicksettings MODULE
    quicksettings/QuickSettingsApplet.cpp
    quicksettings/QuickSettingsApplet.h
)
# ── Spacer ────────────────────────────────────────────────────────────────────
add_library(lingmo-applet-spacer MODULE
    spacer/SpacerApplet.cpp
    spacer/SpacerApplet.h
)
# ── Separator ─────────────────────────────────────────────────────────────────
add_library(lingmo-applet-separator MODULE
    separator/SeparatorApplet.cpp
    separator/SeparatorApplet.h
)

# ── Common properties for all applet targets ──────────────────────────────────
set(APPLET_TARGETS
    lingmo-applet-applicationmenu
    lingmo-applet-taskmanager
    lingmo-applet-systemtray
    lingmo-applet-digitalclock
    lingmo-applet-battery
    lingmo-applet-volume
    lingmo-applet-networkindicator
    lingmo-applet-quicksettings
    lingmo-applet-spacer
    lingmo-applet-separator
)

foreach(target IN LISTS APPLET_TARGETS)
    # Strip the "lingmo-applet-" prefix to get the applet subdirectory name
    string(REPLACE "lingmo-applet-" "" applet_name "${target}")

    target_include_directories(${target}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/${applet_name}
    )

    target_link_libraries(${target}
        PRIVATE
            Lingmo::Applets
            Qt6::Core
            Qt6::Gui
            Qt6::Qml
            Qt6::Quick
            Qt6::DBus
    )

    set_target_properties(${target} PROPERTIES
        OUTPUT_NAME ${target}
        PREFIX      ""
        AUTOMOC     ON
    )

    install(TARGETS ${target}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/lingmo/applets
    )

    # Install QML files if present
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${applet_name}/qml")
        install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${applet_name}/qml/"
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/lingmo/applets/${applet_name}/qml
        )
    endif()
endforeach()

# ── Plugin metadata (.lingmo-plugin JSON) ─────────────────────────────────────
set(PLUGIN_METADATA
    applicationmenu/org.lingmo.applet.applicationmenu.lingmo-plugin
    taskmanager/org.lingmo.applet.taskmanager.lingmo-plugin
    systemtray/org.lingmo.applet.systemtray.lingmo-plugin
    digitalclock/org.lingmo.applet.digitalclock.lingmo-plugin
    battery/org.lingmo.applet.battery.lingmo-plugin
    volume/org.lingmo.applet.volume.lingmo-plugin
    networkindicator/org.lingmo.applet.networkindicator.lingmo-plugin
    quicksettings/org.lingmo.applet.quicksettings.lingmo-plugin
    spacer/org.lingmo.applet.spacer.lingmo-plugin
    separator/org.lingmo.applet.separator.lingmo-plugin
)

install(FILES ${PLUGIN_METADATA}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/lingmo/plugins
)
