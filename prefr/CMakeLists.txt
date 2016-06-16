source_group(\\ FILES CMakeLists.txt)

set(PREFR_PUBLIC_HEADERS

  # To ensure that Glew is included before GL this
  # should be the first file included by the library    
  utils/types.h
  
  utils/Log.h
  utils/Config.h
  utils/Timer.hpp
  utils/InterpolationSet.hpp
  utils/ElementCollection.hpp   
  
  core/ParticleSystem.h
  core/Particles.h
  core/Cluster.h
  core/Updater.h
  core/Source.h
  core/Model.h
  core/DistanceArray.hpp
  core/Sorter.h
  core/RenderConfig.h
  core/Renderer.h
  
  GL/GLRenderer.h
  #GL/shd/GL-frag.glsl
  #GL/shd/GL-vert.glsl
  
  OSG/OSGRenderConfig.h
  OSG/OSGManager.h
  OSG/OSGRenderer.h
  
  cuda/ThrustSorter.cuh
  cuda/CUDADistanceArray.cuh
)

set(PREFR_HEADERS )

set(PREFR_SOURCES
    
  utils/Log.cpp
  utils/Config.cpp
  
  core/ParticleSystem.cpp
  core/Particles.cpp
  core/Cluster.cpp
  core/Updater.cpp
  core/Source.cpp
  core/Model.cpp  
  core/Sorter.cpp  
  core/Renderer.cpp
  
  GL/GLRenderer.cpp
      
  OSG/OSGManager.cpp
  OSG/OSGRenderer.cpp
  
  cuda/ThrustSorter.cu
)

set(PREFR_LINK_LIBRARIES
  ${GLEW_LIBRARIES}
  ${BOOST_LIBRARIES}
)

if ( GLUT_FOUND )
  set( PREFR_LINK_LIBRARIES ${PREFR_LINK_LIBRARIES} ${GLUT_LIBRARIES} )
endif ( )

if ( NVIDIAOPENGL_FOUND )
  link_directories(${NVIDIA_OPENGL_gl_LIBRARY_PATH})
  set(PREFR_LINK_LIBRARIES ${PREFR_LINK_LIBRARIES} ${NVIDIA_OPENGL_gl_LIBRARY})
else( )
  set(PREFR_LINK_LIBRARIES ${PREFR_LINK_LIBRARIES} ${OPENGL_LIBRARIES})
endif( )

if (OPENSCENEGRAPH_FOUND)
  set(PREFR_LINK_LIBRARIES ${PREFR_LINK_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES})
endif()

if (CUDA_FOUND)
  set(CUDA_PROPAGATE_HOST_FLAGS OFF)
  list(APPEND CUDA_NVCC_FLAGS -std=c++11 )

  if (MODERN_GPU_ARCH)
    list(APPEND CUDA_NVCC_FLAGS -arch=sm_20)
  else()
    list(APPEND CUDA_NVCC_FLAGS -arch=sm_11)
  endif()

  cuda_common_library( PReFr )
else()

  common_library( PReFr )

endif()
