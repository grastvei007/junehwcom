cmake_minimum_required(VERSION 3.16)
project(hwcom)

set_target_properties(PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")

include_directories($ENV{JUNE_ROOT})
link_directories($ENV{DEV_LIBS})

add_definitions(-DNO_GUI)

set(CMAKE_INCLUDE_CURRENT_DIR "YES")
set(CMAKE_AUTOMOC "YES")
set(CMAKE_AUTOUIC "YES")

find_package(Qt6 COMPONENTS WebSockets REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS Gui REQUIRED)
find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS SerialPort)

qt_add_executable(hwcom
    main.cpp
    app.h
    app.cpp

    menu.h
    menu.cpp

    gui/console.h
    gui/console.cpp
    gui/systemtrayui.h
    gui/systemtrayui.cpp
)

qt_add_resources(hwcom resources
    PREFIX "/resources"
    FILES resources/icon.jpeg
)

target_link_libraries(hwcom PRIVATE Qt6::WebSockets
    Qt6::Widgets
    Qt6::Gui
    Qt6::Core
    Qt6::SerialPort)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    target_link_libraries(hwcom PRIVATE tagsystemd)
    target_link_libraries(hwcom PRIVATE influxdbd)
    target_link_libraries(hwcom PRIVATE pluginLoadd)
    target_link_libraries(hwcom PRIVATE deviced)
    target_link_libraries(hwcom PRIVATE cored)
else()
    message("Release mode")
    target_link_libraries(hwcom PRIVATE tagsystem)
    target_link_libraries(hwcom PRIVATE influxdb)
    target_link_libraries(hwcom PRIVATE pluginLoad)
    target_link_libraries(hwcom PRIVATE device)
    target_link_libraries(hwcom PRIVATE core)
endif()

target_include_directories(hwcom INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})


add_custom_command(TARGET hwcom
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hwcom> $ENV{DEV_BIN} )


