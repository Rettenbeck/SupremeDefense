cmake_minimum_required(VERSION 3.20)
project(MyApp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSFML_STATIC -O2")

# ImGui sources
set(IMGUI_SRC
    include/imgui-master/imgui.cpp
    include/imgui-master/imgui_draw.cpp
    include/imgui-master/imgui_widgets.cpp
    include/imgui-master/imgui_tables.cpp
)

# ImGui-SFML source
set(IMGUI_SFML_SRC include/imgui-sfml-master/imgui-SFML.cpp)

# Your own sources
file(GLOB_RECURSE MY_SRC CONFIGURE_DEPENDS src/*.cpp)

# set(CMAKE_VERBOSE_MAKEFILE ON)
message(STATUS "Linker search path: ${CMAKE_SOURCE_DIR}/lib")
add_executable(MyApp ${MY_SRC} ${IMGUI_SRC} ${IMGUI_SFML_SRC})

# Includes
target_include_directories(MyApp PRIVATE
    include
    include/imgui-master
    include/imgui-sfml-master
)

target_include_directories(MyApp PRIVATE src)

target_compile_definitions(MyApp PRIVATE SFML_STATIC)

# Tell the linker where to find the .a/.lib/.dll.a files
get_filename_component(LIB_ABS_PATH "${CMAKE_SOURCE_DIR}/lib" ABSOLUTE)
link_directories(${LIB_ABS_PATH})
target_link_directories(MyApp PRIVATE ${LIB_ABS_PATH})

# Link SFML and other system libraries
target_link_libraries(MyApp
    sfml-graphics-s
    sfml-window-s
    sfml-system-s
    sfml-audio-s
    sfml-network-s
    ws2_32
    opengl32
    freetype
    gdi32
    winmm
    shell32
)
