cmake_minimum_required(VERSION 3.5)

project(Sokopoko LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenGL ES 2.0 support (*PS Vita [vitaGL], Android, Emscripten)
option(OPTION_IS_GLES2  "Target is compiled for OpenGL ES 2.0" 0)
# Legacy graphics support (*PSP, N3DS)
option(OPTION_IS_LEGACY "Target only supports immediate mode graphics" 0)

if(OPTION_IS_GLES2 AND OPTION_IS_LEGACY)
  message(FATAL_ERROR "Mutually exclusive graphics options were chosen simultaneously. Go back.")
endif()

if(WIN32 OR UNIX)
  # GLFW
  add_subdirectory("ext/glfw/")
  set(GLFW_BUILD_WAYLAND OFF)

  # OpenGL
  find_package(OpenGL REQUIRED)

  # GLM
  add_subdirectory("ext/glm/")

  # NFD
  add_subdirectory("ext/nativefiledialog-extended")

  # Tracy profiler
  option(TRACY_ENABLE " " ON)
  option(TRACY_ON_DEMAND " " ON)
  add_subdirectory("ext/tracy")
endif()

file(GLOB COMMON_SRC
  "ext/yyjson/yyjson.c" # yyjson
  "src/*.cpp"
)

file(GLOB SOKOPOKO_SRC
  "src/sokopoko/*.cpp"
)

file(GLOB IMGUI_SRC
  "ext/imgui/*.cpp"
  "ext/imgui/backends/imgui_impl_glfw.cpp"
  "ext/imgui/backends/imgui_impl_opengl3.cpp"

  # Custom
  "src/imgui/*.cpp"
)

file(GLOB TRACY_SRC
  "ext/tracy/public/TracyClient.cpp"
)

if(WIN32 OR UNIX)
  include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLFW3_INCLUDE_DIR}
    "ext/imgui/"
    "ext/imgui/backends"
    "ext/"
  )

  # Add glad_gles2.h to GLES2 builds, glad.h if not
  if(OPTION_IS_GLES2)
    list(APPEND COMMON_SRC "src/glad_gles2.c")
  else()
    list(APPEND COMMON_SRC "src/glad.c")
  endif()

  add_executable(${PROJECT_NAME}
    ${COMMON_SRC}
    ${IMGUI_SRC}
    ${SOKOPOKO_SRC}
    ${TRACY_SRC}
  )

  target_compile_definitions(${PROJECT_NAME} PRIVATE
    TB_GRAPHICS_GLES2=${OPTION_IS_GLES2}
    TB_GRAPHICS_LEGACY=${OPTION_IS_LEGACY}
    $<$<CONFIG:Release>:TRACY_ENABLE>
  )

  target_link_libraries(${PROJECT_NAME}
    ${OPENGL_gl_LIBRARY}
    glm::glm
    glfw
    nfd
    Tracy::TracyClient
  )
endif()

# copy assets to build folder
file(COPY res DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
