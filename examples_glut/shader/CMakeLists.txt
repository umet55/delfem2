cmake_minimum_required(VERSION 2.8)

option(USE_HEADERONLY "USE_DFM2_HEADERONLY" OFF)

#######################

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
if(MSVC)
  set(CMAKE_CXX_FLAGS "-O2 /W4 \
     /wd4100 /wd4458 /wd4577 /wd4267 /wd4244 /wd4800 /wd4996 \
     /wd4530 /wd4245 /wd4505 /wd4838 /wd4305 /wd4505 /wd4456")
else()
  set(CMAKE_CXX_FLAGS "-Wall -Wno-deprecated-declarations -g" )
endif()

#######################

project(01_shader)

add_definitions(-DPATH_INPUT_DIR="${PROJECT_SOURCE_DIR}/../../test_inputs")

set(DELFEM2_INCLUDE_DIR "../../include")
set(DELFEM2_INC         "../../include/delfem2")
set(3RD_PARTY           "../../3rd_party")
set(GLAD_INCLUDE_DIR    "${3RD_PARTY}/glad3/include")
get_filename_component(GLAD_INCLUDE_DIR ${GLAD_INCLUDE_DIR} ABSOLUTE)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(
  ${OPENGL_INCLUDE_DIR}
  ${GLUT_INCLUDE_DIR}
  ${GLAD_INCLUDE_DIR}
  ${DELFEM2_INCLUDE_DIR}
)

IF(NOT USE_HEADERONLY)
  message("compile as STATIC libary: ${PROJECT_NAME} ${USE_HEADERONLY}")
  add_definitions(-DDFM2_STATIC_LIBRARY=ON)
  set(SRC_DFM2
      ${DELFEM2_INC}/quat.h               ${DELFEM2_INC}/quat.cpp
      ${DELFEM2_INC}/camera.h             ${DELFEM2_INC}/camera.cpp
      ${DELFEM2_INC}/imgio.h              ${DELFEM2_INC}/imgio.cpp
      
      ${DELFEM2_INC}/opengl/tex_gl.h      ${DELFEM2_INC}/opengl/tex_gl.cpp
      ${DELFEM2_INC}/opengl/funcs_glold.h    ${DELFEM2_INC}/opengl/funcs_glold.cpp
      )
ELSE()
  message("compile as HEADER-ONLY library: ${PROJECT_NAME} ${USE_HEADERONLY}")
ENDIF()


add_executable(${PROJECT_NAME}

    ${DELFEM2_INC}/opengl/gl_funcs.h       ${DELFEM2_INC}/opengl/gl_funcs.cpp

    ${3RD_PARTY}/glad3/include/glad/glad.h ${3RD_PARTY}/glad3/src/glad.c
    ${SRC_DFM2}
    main.cpp
)

IF(UNIX)
  target_link_libraries(${PROJECT_NAME}
    ${GLUT_LIBRARY}
    ${OPENGL_LIBRARY}
    -ldl
  )
ELSE()
  target_link_libraries(${PROJECT_NAME}
    ${GLUT_LIBRARY}
    ${OPENGL_LIBRARY}
  )
ENDIF()
