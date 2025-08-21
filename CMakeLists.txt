cmake_minimum_required(VERSION 3.16)
project(Ue4VisiualScript LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(IMGUI_DIR    ${EXTERNAL_DIR}/imgui)
set(IMNODES_DIR  ${EXTERNAL_DIR}/imnodes)
set(GLAD_DIR     ${EXTERNAL_DIR}/glad)

# --- GLFW ---
include(FetchContent)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)
# GLFW ayarları (FetchContent'ten ÖNCE)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "Disable Wayland backend" FORCE)
set(GLFW_BUILD_X11     ON  CACHE BOOL "Enable X11 backend"   FORCE)

# (zaten vardı ama tam olsun)
set(BUILD_SHARED_LIBS  OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)


# --- GLAD ---
add_library(glad STATIC ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_DIR}/include)
if (UNIX AND NOT APPLE)
  target_link_libraries(glad PUBLIC dl)
endif()

# --- ImGui ---
set(IMGUI_SOURCES
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC
  ${IMGUI_DIR}
  ${IMGUI_DIR}/backends
)
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD IMGUI_DISABLE_OBSOLETE_FUNCTIONS)
target_link_libraries(imgui PUBLIC glfw glad)

# --- imnodes (manuel) ---
add_library(imnodes STATIC ${IMNODES_DIR}/imnodes.cpp)
target_include_directories(imnodes PUBLIC
  ${IMNODES_DIR}
  ${IMGUI_DIR}   # imnodes -> imgui header’larına ihtiyaç duyar
)

# --- App ---
file(GLOB SRC_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
add_executable(editor ${SRC_FILES})
target_include_directories(editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (WIN32)
  target_link_libraries(editor PRIVATE imgui imnodes opengl32)
  target_compile_definitions(editor PRIVATE NOMINMAX)
elseif(APPLE)
  target_link_libraries(editor PRIVATE imgui imnodes
    "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
  target_compile_definitions(editor PRIVATE GL_SILENCE_DEPRECATION)
else()
  find_package(OpenGL REQUIRED)
  target_link_libraries(editor PRIVATE imgui imnodes OpenGL::GL)
endif()

if (MSVC)
  target_compile_options(editor PRIVATE /W4)
else()
  target_compile_options(editor PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()
