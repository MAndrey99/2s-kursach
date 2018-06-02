cmake_minimum_required(VERSION 3.10)
project(Game)

set(CMAKE_CXX_STANDARD 17)

# Задаём параметры для стстической линковки
set(SFML_STATIC_LIBRARIES TRUE)
set(CMAKE_EXE_LINKER_FLAGS "-static")

set(CMAKE_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/src)
add_executable(Game src/main.cpp src/Player.cpp src/Muvement.cpp src/Field.cpp src/Bullet.cpp src/Collision.cpp)

# Подключаем sfml
set(SFML_DIR "D:/programming/libs/SFML-2.4.2")
set(CMAKE_MODULE_PATH "${SFML_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED audio graphics window system)
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(Game ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
