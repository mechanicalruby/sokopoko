cmake_minimum_required(VERSION 3.5)

project(Sokopoko LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLFW
add_subdirectory("include/glfw/")
set(GLFW_BUILD_WAYLAND OFF)

# OpenGL
find_package(OpenGL REQUIRED)

# GLM
add_subdirectory("include/glm/")

file(GLOB COMMON_SRC
  # "include/yyjson/yyjson.c" # yyjson
  "src/*.c"
  "src/*.cpp"
)

file(GLOB SOKOPOKO_SRC
  "src/sokopoko/*.cpp"
)

file(GLOB IMGUI_SRC
  "include/imgui/*.cpp"
  "include/imgui/backends/imgui_impl_glfw.cpp"
  "include/imgui/backends/imgui_impl_opengl3.cpp"

  # Custom
  "src/imgui/*.cpp"
)

include_directories(
  ${OPENGL_INCLUDE_DIR}
  ${GLFW3_INCLUDE_DIR}
  "include/imgui/"
  "include/imgui/backends"
  "include/"
)

add_executable(${PROJECT_NAME}
  ${COMMON_SRC}
  ${IMGUI_SRC}
  ${SOKOPOKO_SRC}
)

target_link_libraries(${PROJECT_NAME}
  ${OPENGL_gl_LIBRARY}
  glm::glm
  glfw
)

# copy assets to build folder
file(COPY res DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
