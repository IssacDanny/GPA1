cmake_minimum_required(VERSION 3.10)

# Project Name
project(WhackAZombie VERSION 1.0.0 LANGUAGES CXX)

# Standard C++17 (Required for modern features)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(SFML_DIR "${CMAKE_SOURCE_DIR}/vendor/SFML-2.6.1/lib/cmake/SFML")

# Locate the SFML Library (Graphics, Audio, Window, System)
# Note: Ensure SFML is installed on your machine.
find_package(SFML 2.5 COMPONENTS graphics audio window system REQUIRED)

# Define the Include Directory
include_directories(include)

# Gather all Source Files
file(GLOB SOURCES "src/*.cpp")

# Create the Executable
add_executable(WhackAZombie ${SOURCES})

# Link SFML to the Executable
target_link_libraries(WhackAZombie sfml-graphics sfml-audio sfml-window sfml-system)

# This command copies the necessary SFML .dll files to the build folder
# so the executable can find them when you run it.
add_custom_command(TARGET WhackAZombie POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/vendor/SFML-2.6.1/bin"
        $<TARGET_FILE_DIR:WhackAZombie>
)

# Post-Build Command: Copy assets to the build folder automatically
# This prevents the "File not found" error when running the game.
add_custom_command(TARGET WhackAZombie POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:WhackAZombie>/assets
)