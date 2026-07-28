cmake_minimum_required(VERSION 3.15)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(ext/glfw)

set(GLAD_SRC ext/glad/glad.c
        shader.h)

add_executable(LearnOpenGL
        main.cpp
        ${GLAD_SRC}
)

target_include_directories(LearnOpenGL PRIVATE
        ext/glad/include
        ext/glm
        ext/stb_image
)

target_link_libraries(LearnOpenGL PRIVATE glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(LearnOpenGL PRIVATE OpenGL::GL)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) # copies shaders folder to cmake build folder