cmake_minimum_required(VERSION 2.8.12)

# compile as "cmake -DEMSCRIPTEN=1 ."  to build with Emscripten
option(EMSCRIPTEN "Use Emscripten:" OFF)

option(USE_HEADERONLY "USE_DFM2_HEADERONLY" ON)

#####################################

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
IF(NOT EMSCRIPTEN)
  IF(MSVC)
    set(CMAKE_CXX_FLAGS "-O2 -std=c++11 /W4 \
    /wd4100 /wd4458 /wd4577 /wd4267 /wd4244 \
    /wd4800 /wd4996 /wd4530 /wd4245 /wd4505 \
    /wd4838 /wd4305 /wd4505 /wd4456 /wd4055")
  ELSE()
    set(CMAKE_CXX_FLAGS "-Wall -Wno-deprecated-declarations -g")
  ENDIF()
ENDIF()

#####################################

project(11_imgui_basic)

add_definitions(-DPATH_INPUT_DIR="${PROJECT_SOURCE_DIR}/../../test_inputs")

# opengl
find_package(OpenGL REQUIRED)

# glfw
set(GLFW_DIR "../../3rd_party/glfw")
get_filename_component(GLFW_DIR ${GLFW_DIR} ABSOLUTE)

# imgui
add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)
set(IMGUI_DIR "../../3rd_party/imgui")

# glad
set(GLAD_INCLUDE_DIR "../../3rd_party/glad4/include")
get_filename_component(GLAD_INCLUDE_DIR ${GLAD_INCLUDE_DIR} ABSOLUTE)

# dfm2
if(USE_HEADERONLY)
  add_definitions(-DDFM2_HEADER_ONLY=ON)
else()
  set(DFM2_LIBRARY
      ${PROJECT_SOURCE_DIR}/../dfm2_static_glfwnew/libdfm2_static_glfwnew.a )
endif()
set(DELFEM2_INCLUDE_DIR "../../include")


##############################

include_directories(
  ${OPENGL_INCLUDE_DIR}
  ${GLFW_DIR}/include
  ${IMGUI_DIR}
  ${IMGUI_DIR}/examples
  ${GLAD_INCLUDE_DIR}
  ${DELFEM2_INCLUDE_DIR}
)

set(3RD_PARTY           "../../3rd_party")
add_executable(${PROJECT_NAME}
    ${IMGUI_DIR}/imgui.h                    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/examples/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/examples/imgui_impl_opengl3.h
    ${IMGUI_DIR}/examples/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/examples/imgui_impl_glfw.h
    ${3RD_PARTY}/glad4/include/glad/glad.h  ${3RD_PARTY}/glad4/src/glad.c
    main.cpp
)

###########################

if (NOT EMSCRIPTEN)
  IF(APPLE)
    find_library( COCOA_LIBRARY Cocoa )
    find_library( IOKIT_LIBRARY IOKit )
    find_library( COREVIDEO_LIBRARY CoreVideo )
    target_link_libraries(${PROJECT_NAME}
      ${DFM2_LIBRARY}
      ${OPENGL_LIBRARY}
      ${COCOA_LIBRARY}
      ${IOKIT_LIBRARY}
      ${COREVIDEO_LIBRARY}
      ${GLFW_DIR}/src/libglfw3.a
    )
  ELSEIF(UNIX)
    target_link_libraries(${PROJECT_NAME}
      ${DFM2_LIBRARY}
      ${OPENGL_LIBRARY}
      ${GLEW_LIBRARY}
      ${GLFW_DIR}/src/libglfw3.a
      -lrt -lm -ldl -lX11 -lpthread -lxcb -lXau -lXdmcp
    )
  ELSEIF(MSVC)
    target_link_libraries(${PROJECT_NAME}
      ${OPENGL_LIBRARY}
      ${GLFW_DIR}/src/Release/glfw3.lib
      ${EXTRA_LIBS}
    )    
  ENDIF()
else()
  set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
  set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
  set(CMAKE_C_COMPILER "emcc")
  set(CMAKE_CXX_COMPILER "em++")
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DEMSCRIPTEN=1)
  set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1")
#  set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-s USE_GLFW=3 -s FULL_ES3=1")
  include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLFW_DIR}/include
    ${GLAD_INCLUDE_DIR}
    ${IMGUI_DIR}
    ${IMGUI_DIR}/examples
    ${DELFEM2_INCLUDE_DIR}
  )
endif()
