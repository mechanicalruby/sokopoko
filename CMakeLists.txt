cmake_minimum_required(VERSION 3.5)

project(Sokopoko LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLFW
add_subdirectory("include/glfw/")
set(GLFW_BUILD_WAYLAND OFF)

find_package(OpenGL REQUIRED)

file(GLOB COMMON_SRC
  # "include/yyjson/yyjson.c" # yyjson
  "src/*.cpp"
)

file(GLOB SOKOPOKO_SRC
  "src/sokopoko/*.cpp"
)

include_directories(
  ${OPENGL_INCLUDE_DIR}
  ${GLFW3_INCLUDE_DIR}
  "include/"
)

add_executable(${PROJECT_NAME}
  ${COMMON_SRC}
  ${SOKOPOKO_SRC}
)

target_link_libraries(${PROJECT_NAME}
  ${OPENGL_gl_LIBRARY}
  glfw
)

# copy assets to build folder
file(COPY res DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
