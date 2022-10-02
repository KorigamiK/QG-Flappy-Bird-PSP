cmake_minimum_required(VERSION 3.17)
project(flappy)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(flappy main.c)
add_subdirectory(./QuickGame/ QuickGame)

target_link_libraries(flappy PUBLIC QuickGame)
target_include_directories(flappy PUBLIC QuickGame/include)

if (PLATFORM_PSP)
    create_pbp_file(
        TARGET flappy
        TITLE Flappy Bird PSP
	ICON_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/bird/idle.png"
        BUILD_PRX ON
    )
endif()

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})