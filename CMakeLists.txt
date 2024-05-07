cmake_minimum_required(VERSION 3.1.0)

project(GFTS3)

add_library(numeric_method STATIC
    numeric_method/Matrix_solver.cpp
    numeric_method/MWR.cpp
)

set(project_sources
    src/main.cpp
)

set(project_headers
    include/Matrix_solver.h
    include/MWR.h
)

set(project_compile_flags "-march=native -O2 ")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${project_compile_flags}")

add_executable(GFTS3
    ${project_sources}
    ${project_headers}
)

target_link_libraries(GFTS3 PRIVATE
    numeric_method
)

set_target_properties(GFTS3 PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
