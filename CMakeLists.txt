# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   particles
#   2014 (c) GMRV / Universidad Rey Juan Carlos
#   gmrv@gmrv.es
#   www.gmrv.es
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
cmake_minimum_required(VERSION 2.8)
project(particles)

# Version numbers
set(${PROJECT_NAME}_VERSION_MAJOR 0)
set(${PROJECT_NAME}_VERSION_MINOR 1)
set(${PROJECT_NAME}_VERSION_PATCH 0)
set(${PROJECT_NAME}_VERSION
    ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH})

# Add CMake dir to the module path
list(APPEND CMAKE_MODULE_PATH 
  ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

# Standard configs like version, debug as default
include(StandardConfig)

# Custom flags for Release and Debug
set(CMAKE_CXX_FLAGS 
  "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS_DEBUG 
  "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

#########################################################
# FIND GLEW
#########################################################

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
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

FILE(GLOB particles_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/default/*.cpp)

# Build library
add_library (${PROJECT_NAME} SHARED
${particles_SOURCES}
)
set_target_properties(${CMAKE_PROJECT_NAME}
  PROPERTIES VERSION ${${CMAKE_PROJECT_NAME}_VERSION} SOVERSION ${${CMAKE_PROJECT_NAME}_VERSION}
)

# Install rules
# Find files to install 
file(GLOB DEPLOY_FILES_AND_DIRS 
"${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/*"
)
include(InstallLibrary)

# Doxygen documentation generation
include(Doxygen)

# Examples subdirectory 
add_subdirectory(examples)

# Export library
include(ExportLibrary)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} ${CUDA_LIBRARIES} ${FREEIMAGE_LIBRARIES})

