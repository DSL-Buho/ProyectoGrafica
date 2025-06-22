cmake_minimum_required(VERSION 3.15)
project(ProyectoFinalGrafica)

set(CMAKE_CXX_STANDARD 20)

# --------------------------------------
# Vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "D:/LenguajesProgramacion/C++/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")

# --------------------------------------
# Dependencias
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLUT REQUIRED)

# --------------------------------------
# Ejecutable principal
add_executable(ProyectoFinalGrafica
        src/main.cpp
        src/planet.cpp
        src/spaceship.cpp
        src/functions.cpp
        src/object.cpp
        include/globals.h
        src/globals.cpp
)

target_include_directories(ProyectoFinalGrafica PRIVATE include)

target_link_libraries(ProyectoFinalGrafica PRIVATE
        OpenGL::GL
        GLEW::GLEW
        glfw
        ${GLUT_LIBRARIES}
)

# --------------------------------------
# Ejecutable de prueba (opcional)
add_executable(prueba
        prueba.cpp
        prueba.h
)

target_include_directories(prueba PRIVATE include)

target_link_libraries(prueba PRIVATE
        OpenGL::GL
        GLEW::GLEW
        glfw
        ${GLUT_LIBRARIES}
)
