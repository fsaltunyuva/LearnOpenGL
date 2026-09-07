cmake_minimum_required(VERSION 3.15)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(ext/glfw)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "" FORCE)

add_subdirectory(ext/assimp)

set(GLAD_SRC ext/glad/glad.c
        shader.h
        stb_image.h
        camera.h)

add_executable(LearnOpenGL
        main.cpp
        ${GLAD_SRC}
)

target_include_directories(LearnOpenGL PRIVATE
        ext/glad/include
        ext/glm
        ext/stb_image
        ext/assimp/include
        ${CMAKE_CURRENT_BINARY_DIR}/ext/assimp/include
)

target_link_libraries(LearnOpenGL PRIVATE glfw assimp)

find_package(OpenGL REQUIRED)
target_link_libraries(LearnOpenGL PRIVATE OpenGL::GL)