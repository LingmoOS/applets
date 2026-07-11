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
