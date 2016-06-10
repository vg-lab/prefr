# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   PREFR
#   2014 (c) GMRV / Universidad Rey Juan Carlos
#   gmrv@gmrv.es
#   www.gmrv.es
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

# prefr project and version
project(prefr VERSION 0.1.0)
set(prefr_VERSION_ABI 1)

# Disable in source building
if("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
  message(FATAL "no in source building allowed.")
endif()

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake
  ${CMAKE_SOURCE_DIR}/CMake/common)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

if (CMAKE_BUILD_TYPE MATCHES "Debug")
  add_definitions(-DDEBUG)
endif()

include(GitExternal)

# description
set(PREFR_DESCRIPTION "PREFR - Particle Framework")
set(COMMON_PROJECT_DOMAIN gmrv.es)

# options for prefr building
option(WITH_OPENSCENEGRAPH "WITH_OPENSCENEGRAPH" OFF)
option(WITH_CUDA "WITH_CUDA" OFF)
option(MODERN_GPU_ARCH "GPU_ARCH" ON)
option(WITH_EXAMPLES "WITH_EXAMPLES" OFF)

include(Common)

common_find_package( OpenGL REQUIRED SYSTEM )
common_find_package( nvidiaOpenGL SYSTEM )
common_find_package( GLEW REQUIRED SYSTEM )
common_find_package( GLUT REQUIRED SYSTEM )
common_find_package( GLM REQUIRED )
common_find_package( Boost REQUIRED )
common_find_package( OpenMP SYSTEM )

list( APPEND PREFR_DEPENDENT_LIBRARIES OpenGL GLEW GLUT )
list( APPEND PREFR_DEPENDENT_LIBRARIES GLM Boost )

if ( WITH_OPENSCENEGRAPH )
	common_find_package( OpenSceneGraph COMPONENTS osgViewer osgGA osgDB osgUtil osgQt SYSTEM)
	if ( OPENSCENEGRAPH_FOUND )
		list( APPEND PREFR_DEPENDENT_LIBRARIES OpenSceneGraph )
	endif()
endif()

if ( WITH_CUDA )

	#set(CUDA_SEPARABLE_COMPILATION ON)
	
	include(CudaCommonLibrary)
	
	common_find_package( CUDA SYSTEM )
		
	if ( CUDA_FOUND )
		list( APPEND PREFR_DEPENDENT_LIBRARIES CUDA )

		common_find_package( CUDAThrust SYSTEM )
		if ( CUDAThrust_FOUND )
			list( APPEND PREFR_DEPENDENT_LIBRARIES CUDAThrust )
		endif()
	endif()
endif()

common_find_package_post()

set(PREFR_LIBRARY_BASE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/prefr)

# library subdirectory
add_subdirectory(prefr)

# examples subdirectory
add_subdirectory(examples)

include(CPackConfig)
include(DoxygenRule)
