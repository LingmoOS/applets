set(CMAKE_AUTOMOC ON)

# This example builds as a MODULE (plugin .so), exactly like a real applet.
add_library(lingmo-applet-example-counter MODULE
    CounterApplet.cpp
    CounterApplet.h
)

target_link_libraries(lingmo-applet-example-counter
    PRIVATE
        Lingmo::Applets
        Qt6::Core
)

set_target_properties(lingmo-applet-example-counter PROPERTIES
    OUTPUT_NAME lingmo-applet-example-counter
    PREFIX ""
)
