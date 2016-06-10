# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
#   PREFR
#   2014 (c) GMRV / Universidad Rey Juan Carlos
#   gmrv@gmrv.es
#   www.gmrv.es
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
cmake_minimum_required(VERSION 2.8)
project(prefr)

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

# nsol version
set(LAST_RELEASE 0.0.0)
set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION_ABI 1)

# description
set(PREFR_DESCRIPTION "PREFR - Particle Framework")
set(COMMON_PROJECT_DOMAIN gmrv.es)

# options for prefr building
option(WITH_OPENSCENEGRAPH "WITH_OPENSCENEGRAPH" OFF)
option(WITH_CUDA "WITH_CUDA" OFF)
option(MODERN_GPU_ARCH "GPU_ARCH" ON)
option(WITH_EXAMPLES "WITH_EXAMPLES" OFF)

include(Common)
# include(FindPackages)

common_package( OpenGL REQUIRED SYSTEM )
common_package( nvidiaOpenGL SYSTEM )
common_package( GLEW REQUIRED SYSTEM )
common_package( GLUT REQUIRED SYSTEM )
common_package( GLM REQUIRED )
common_package( Boost REQUIRED )
common_package( OpenMP SYSTEM )

list( APPEND PREFR_DEPENDENT_LIBRARIES OpenGL GLEW GLUT )
list( APPEND PREFR_DEPENDENT_LIBRARIES GLM Boost )

if ( WITH_OPENSCENEGRAPH )
	common_package( OpenSceneGraph COMPONENTS osgViewer osgGA osgDB osgUtil osgQt SYSTEM)
	if ( OPENSCENEGRAPH_FOUND )
		list( APPEND PREFR_DEPENDENT_LIBRARIES OpenSceneGraph )
	endif()
endif()

if ( WITH_CUDA )

	#set(CUDA_SEPARABLE_COMPILATION ON)
	
	include(CudaCommonLibrary)
	
	common_package( CUDA SYSTEM )
		
	if ( CUDA_FOUND )
		list( APPEND PREFR_DEPENDENT_LIBRARIES CUDA )

		common_package( CUDAThrust SYSTEM )
		if ( CUDAThrust_FOUND )
			list( APPEND PREFR_DEPENDENT_LIBRARIES CUDAThrust )
		endif()
	endif()
endif()

common_package_post()

set(PREFR_LIBRARY_BASE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/prefr)
#message( ${PREFR_LIBRARY_BASE_PATH} )

# library subdirectory
add_subdirectory(prefr)

# examples subdirectory
add_subdirectory(examples)

include(CPackConfig)
include(DoxygenRule)
