cmake_minimum_required(VERSION 3.17)
project(flappy)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(flappy main.c)
add_subdirectory(./QuickGame/ QuickGame)

target_link_libraries(flappy PUBLIC QuickGame)
target_include_directories(flappy PUBLIC QuickGame/include)

create_pbp_file(
    TARGET flappy
    TITLE Flappy Bird PSP
    BUILD_PRX ON
)