cmake_minimum_required(VERSION 2.8)

project(particles)

list(APPEND CMAKE_MODULE_PATH 
  ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

SET(SRCDIR src)
SET(INCDIR src)

FILE(GLOB particles_SOURCES ${SRCDIR}/*.cpp)
FILE(GLOB particles_HEADERS ${INCDIR}/*.h)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x") # -Wall

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${INCDIR})

#########################################################
# FIND GLEW
#########################################################

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
message(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${GLEW_DEFINITIONS})
if(NOT GLEW_FOUND)
    message(ERROR " GLEW not found!")
endif(NOT GLEW_FOUND)

#########################################################
# FIND X11
#########################################################

find_package(X11)
include_directories(${X11_INCLUDE_DIRS})
link_directories(${X11_LIBRARY_DIRS})
add_definitions(${X11_DEFINITIONS})
if(NOT X11_FOUND)
    message(ERROR " X11 not found!")
endif(NOT X11_FOUND)



#########################################################
# FIND GLUT
#########################################################

find_package(GLUT)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)


#find_package(X11 REQUIRED)

#########################################################
# FIND GLM
#########################################################

#include_directories(/usr/include/glm/)

#########################################################
# FIND CUDA
#########################################################
find_package(CUDA)
include_directories(${CUDA_INCLUDE_DIRS})
link_directories(${CUDA_LIBRARY_DIRS})
add_definitions(${CUDA_DEFINITIONS})
if(NOT CUDA_FOUND)
    message(ERROR " CUDA not found!")
endif(NOT CUDA_FOUND)

find_package(FreeImage REQUIRED)


add_executable(particles ${particles_SOURCES})
# ${X11_LIBRARIES}
TARGET_LINK_LIBRARIES(particles ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} ${CUDA_LIBRARIES} ${FREEIMAGE_LIBRARIES})
