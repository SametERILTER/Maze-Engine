cmake_minimum_required(VERSION 3.10)
project(MazeEngine)

set(CMAKE_CXX_STANDARD 17)

# SFML'i sistemden bul (MSYS2 sayesinde otomatik bulacak)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

file(GLOB SOURCES "src/*.cpp")

add_executable(MazeGame ${SOURCES})

target_link_libraries(MazeGame sfml-graphics sfml-window sfml-system)