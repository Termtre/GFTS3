cmake_minimum_required(VERSION 3.8.0)

project(GFTS3)

file(GLOB library_sources numeric_method/*.cpp)

add_library(numeric_method STATIC
    ${library_sources}
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Graphs)

file(GLOB project_sources src/Task_manager.cpp)

file(GLOB project_ui ui/*.cpp)

file(GLOB project_headers include/*.h ui/mainwindow.h)

set(project_forms ui/mainwindow.ui)

set(project_compile_flags "-march=native -O2 -I ui/")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${project_compile_flags}")

add_executable(GFTS3
    ${project_sources}
    ${project_headers}
    ${project_ui}
    ${project_forms}
)

target_link_libraries(GFTS3 PRIVATE
    numeric_method
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Graphs
)

set_target_properties(GFTS3 PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
