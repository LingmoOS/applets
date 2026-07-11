set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Test)

function(add_lingmo_applets_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name}
        PRIVATE
            Lingmo::Applets
            Qt6::Test
    )
    add_test(NAME ${name} COMMAND ${name})
endfunction()

add_lingmo_applets_test(tst_Applet)
add_lingmo_applets_test(tst_AppletHost)
add_lingmo_applets_test(tst_AppletFactory)
add_lingmo_applets_test(tst_AppletContext)

# Structural and ABI-readiness checks
add_lingmo_applets_test(tst_Architecture)

# Built-in applet logic tests
# These tests link against the built-in applet plugins so their
# LINGMO_EXPORT_APPLET registrars fire at load time.
add_executable(tst_BuiltinApplets tst_BuiltinApplets.cpp)
target_link_libraries(tst_BuiltinApplets
    PRIVATE
        Lingmo::Applets
        Qt6::Test
        # Link each built-in plugin directly (MODULE libs are loaded as OBJECT
        # via an intermediate SHARED wrapper when unit-testing without a host).
        lingmo-applet-digitalclock
        lingmo-applet-volume
        lingmo-applet-quicksettings
        lingmo-applet-spacer
        lingmo-applet-separator
        lingmo-applet-battery
        lingmo-applet-networkindicator
        lingmo-applet-systemtray
        lingmo-applet-taskmanager
        lingmo-applet-applicationmenu
)
add_test(NAME tst_BuiltinApplets COMMAND tst_BuiltinApplets)
