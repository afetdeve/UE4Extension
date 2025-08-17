cmake_minimum_required(VERSION 3.15)
project(Ue4-Visiual-Script CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Paths ----
set(GLFW_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw)
set(IMGUI_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)
set(IMNODES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imnodes)

# ---- GLFW (CMake'li) ----
add_subdirectory(${GLFW_DIR})

# ---- ImGui (CMake'siz: kaynaklardan statik lib oluştur) ----
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    # Backends:
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${GLFW_DIR}/include
)
# İsteğe bağlı: eski API'leri kapat
target_compile_definitions(imgui PUBLIC IMGUI_DISABLE_OBSOLETE_FUNCTIONS)

# ---- imnodes (genelde tek .cpp) ----
# Kullandığın imnodes sürümünde dosya adları farklıysa burayı uyarlayabilirsin.
add_library(imnodes STATIC ${IMNODES_DIR}/imnodes.cpp)
target_include_directories(imnodes PUBLIC
    ${IMNODES_DIR}
    ${IMGUI_DIR}          # imnodes, imgui'yi include eder
)

# ---- Uygulama ----
file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(editor ${SRC_FILES})

target_include_directories(editor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${IMGUI_DIR}
    ${IMNODES_DIR}
    ${GLFW_DIR}/include
)

# ---- OpenGL ve linkler ----
# ImGui backend'leri için: glfw + OpenGL
if (WIN32)
    target_link_libraries(editor PRIVATE imgui imnodes glfw opengl32)
elseif(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    find_library(OPENGL_LIBRARY OpenGL)
    target_link_libraries(editor PRIVATE imgui imnodes glfw
        ${OPENGL_LIBRARY} ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(editor PRIVATE imgui imnodes glfw OpenGL::GL)
endif()

# (İsteğe bağlı) Windows derlemelerinde min/max çakışmasını önle
if (MSVC)
    target_compile_definitions(editor PRIVATE NOMINMAX)
endif()
