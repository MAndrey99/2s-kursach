cmake_minimum_required(VERSION 3.10)
project(Game)

set(CMAKE_CXX_STANDARD 17)

# set(SFML_STATIC_LIBRARIES TRUE)

add_executable(Game src/main.cpp src/Player.cpp src/Player.h)

target_link_libraries(${PROJECT_NAME} PUBLIC sfml-graphics sfml-window sfml-system)
