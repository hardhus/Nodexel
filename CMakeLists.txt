cmake_minimum_required(VERSION 3.14)

if(POLICY CMP0169)
  cmake_policy(SET CMP0169 OLD)
endif()

project(
  Nodexel
  VERSION 0.1.0
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# ---- 1. GLFW DOWNLOAD ----
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw)
set(GLFW_BUILD_DOCS
    OFF
    CACHE INTERNAL "")
set(GLFW_BUILD_TESTS
    OFF
    CACHE INTERNAL "")
set(GLFW_BUILD_EXAMPLES
    OFF
    CACHE INTERNAL "")
FetchContent_MakeAvailable(glfw)

# ---- 2. IMGUI DOWNLOAD ----
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG docking
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui)
FetchContent_MakeAvailable(imgui)

# ---- 3. IMNODES DOWNLOAD ----
FetchContent_Declare(
  imnodes
  GIT_REPOSITORY https://github.com/Nelarius/imnodes.git
  GIT_TAG master
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imnodes)
FetchContent_Populate(imnodes)

# ---- 4. STB IMAGE DOWNLOAD ----
FetchContent_Declare(
  stb
  URL https://raw.githubusercontent.com/nothings/stb/master/stb_image.h
  DOWNLOAD_NO_EXTRACT TRUE
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stb)
FetchContent_Populate(stb)

if(MINGW)
  set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
endif()

# ---- 5. PROJECT COMPILE ----
add_executable(
  ${PROJECT_NAME}
  src/main.cpp
  src/application.cpp
  src/editor_layout.cpp
  src/pipeline/texture.cpp
  src/graph/node_graph.cpp
  src/graph/nodes/input_node.cpp
  src/graph/nodes/output_node.cpp
  src/graph/nodes/pixelate_node.cpp
  src/pipeline/pipeline_manager.cpp
  src/graph/nodes/pixelate_node.cpp
  src/graph/nodes/color_quantize_node.cpp)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE src
          ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw/include
          ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui
          ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/backends
          ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imnodes
          ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/stb)

target_sources(
  ${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/backends/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imnodes/imnodes.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw ${CMAKE_DL_LIBS})

if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
endif()
