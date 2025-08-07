cmake_minimum_required(VERSION 3.15)
project(Ue4-Visiual-Script)

set(CMAKE_CXX_STANDARD 17)

# Kütüphane dizinleri
add_subdirectory(external/glfw)
add_subdirectory(external/imgui)
add_subdirectory(external/imnodes)

include_directories(
  external/glfw/include
  external/imgui
  external/imnodes
  src
)

file(GLOB SRC_FILES src/*.cpp)

add_executable(editor ${SRC_FILES})

target_link_libraries(editor
    glfw
    imgui
    imnodes
    opengl32  # ✅ Windows'ta OpenGL kütüphanesi
)

