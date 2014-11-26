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

# Disable in source building 
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)


# Option to build with shared pointers or raw pointers
option(WITH_CUDA "WITH_SHARED_PTR" OFF)
if (WITH_CUDA)
  set(${PROJECT_NAME}_WITH_CUDA 1)
else()
  set(${PROJECT_NAME}_WITH_CUDA 0)
endif()

option(PROFILE "ACTIVATE PROFILING" OFF)
IF (PROFILE)
  SET(${PROJECT_NAME}_PROFILING 1)
ELSE ()
  SET(${PROJECT_NAME}_PROFILING 0)
ENDIF()

# Version numbers
set(${PROJECT_NAME}_VERSION_MAJOR 0)
set(${PROJECT_NAME}_VERSION_MINOR 1)
set(${PROJECT_NAME}_VERSION_PATCH 0)
set(${PROJECT_NAME}_VERSION
    ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH})

SET(${PROJECT_NAME}_LIBRARY_BASE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/)

# Add CMake dir to the module path
list(APPEND CMAKE_MODULE_PATH 
  ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

# Standard configs like version, debug as default
include(StandardConfig)

# Custom flags for Release and Debug
if (MSVC)
set(CMAKE_CXX_FLAGS 
  "${CMAKE_CXX_FLAGS} -std=c++11 /W4")
else() 
set(CMAKE_CXX_FLAGS 
  "${CMAKE_CXX_FLAGS} -std=c++11  -Wall -Wno-unknown-pragmas")
endif()

set(CMAKE_CXX_FLAGS_DEBUG 
  "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

  
SET(LINK_MODE "STATIC")
IF (PROFILE)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
	SET(LINK_MODE "STATIC")
	MESSAGE("${PROJECT_NAME} Profiling ON, LINK MODE ${LINK_MODE}" )	
ENDIF()

#########################################################
# FIND NVIDIA OPENGL
#########################################################

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  find_package(nvidiaOpenGL)
  if ( NVIDIA_OPENGL_gl_LIBRARY )
    set( CMAKE_EXE_LINKER_FLAGS "-L${NVIDIA_OPENGL_gl_LIBRARY_PATH}" )
  endif()
endif()


#########################################################
# FIND OPENGL
#########################################################

if (NOT OPENGL_FOUND)
  find_package(OpenGL REQUIRED)
  include_directories(${OpenGL_INCLUDE_DIRS})
  link_directories(${OpenGL_LIBRARY_DIRS})
  add_definitions(${OpenGL_DEFINITIONS})
  if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
  endif(NOT OPENGL_FOUND)
endif()


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
# FIND GLUT
#########################################################

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)



#find_package(X11 REQUIRED)

#########################################################
# FIND GLM
#########################################################
include_directories(${GLM_INCLUDE_DIRS})
#include_directories(/usr/include/glm/)

#########################################################
# FIND CUDA
#########################################################

if (WITH_CUDA)
  find_package(CUDA REQUIRED)
  include_directories(${CUDA_INCLUDE_DIRS})
  message(${CUDA_INCLUDE_DIRS})
  link_directories(${CUDA_LIBRARY_DIRS})
  add_definitions(${CUDA_DEFINITIONS})
  if(NOT CUDA_FOUND)
    message(ERROR " CUDA not found!")
  endif(NOT CUDA_FOUND)

  set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};
    -O3 -gencode arch=compute_20,code=sm_20 -std=c++11)


endif()  

#########################################################
# FIND Thrust
#########################################################
if (WITH_CUDA)
  find_package(CUDAThrust REQUIRED)
  include_directories(${CUDATHRUST_INCLUDE_DIRS})
  link_directories(${CUDATHRUST_LIBRARY_DIRS})
  add_definitions(${CUDATHRUST_DEFINITIONS})
  if(NOT CUDATHRUST_FOUND)
    message(ERROR " CUDATHRUST not found!")
  endif(NOT CUDATHRUST_FOUND)
endif()  

#find_package(FreeImage REQUIRED)

FIND_PACKAGE(OpenSceneGraph COMPONENTS osgViewer osgGA osgDB osgUtil REQUIRED)

include(Findosg_functions)
OSG_FIND_PATH   (OSGQT osgQt/)
OSG_FIND_LIBRARY(OSGQT osgQt)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(osgQt DEFAULT_MSG
    OSGQT_LIBRARY OSGQT_INCLUDE_DIR)

INCLUDE_DIRECTORIES(BEFORE SYSTEM ${OPENSCENEGRAPH_INCLUDE_DIRS}) 


FILE(GLOB particles_SOURCES 
${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/*.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/GL/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/OSG/*.cpp)


#configure_file (
#  "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/default/GL/shd/GL.vert"
#  "${CMAKE_CURRENT_BINARY_DIR}/examples/GL.vert"
#)
#configure_file (
#  "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/default/GL/shd/GL.frag"
#  "${CMAKE_CURRENT_BINARY_DIR}/examples/GL.frag"
#)


SET( CUDA_PROPAGATE_HOST_FLAGS OFF ) 

if (NOT WITH_CUDA)
  # Build library
  add_library (${PROJECT_NAME} ${LINK_MODE}
    ${particles_SOURCES})
else()	
  message(${CUDA_VERSION})
  CUDA_ADD_LIBRARY(${PROJECT_NAME} ${LINK_MODE}
    ${particles_SOURCES}
    particles/cuda/ThrustParticleSorter.cu    
    OPTIONS -arch sm_20
    )
endif()

set_target_properties(${CMAKE_PROJECT_NAME}
  PROPERTIES VERSION ${${CMAKE_PROJECT_NAME}_VERSION} 
  SOVERSION ${${CMAKE_PROJECT_NAME}_VERSION}
)


TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
  ${OPENSCENEGRAPH_LIBRARIES}
  ${OPENGL_LIBRARIES} 
  ${GLUT_LIBRARY} 
  ${GLEW_LIBRARIES} 
  ${CUDA_LIBRARIES} 
#  ${FREEIMAGE_LIBRARIES} 
  ${CUDATHRUST_LIBRARIES}
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




