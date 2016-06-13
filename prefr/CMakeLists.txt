source_group(\\ FILES CMakeLists.txt)

set(PREFR_PUBLIC_HEADERS
  # To ensure that Glew is included before GL this
  # should be the first file included by the library
  types.h
  #
  Cluster.h
  ElementCollection.hpp
  InterpolationSet.hpp
  DistanceArray.hpp
  Timer.hpp
  Particles.h
  Model.h
  OSG/OSGRenderConfig.h
  OSG/OSGDefaultParticleSystem.h
  OSG/OSGRenderer.h
  RenderConfig.h
  cuda/ThrustSorter.cuh
  cuda/CUDADistanceArray.cuh
  Renderer.h
  Sorter.h
  Updater.h
  GL/GLRenderer.h
  Log.h
  ParticleSystem.h
  Source.h
  GL/shd/GL-frag.glsl
  GL/shd/GL-vert.glsl
  Config.h
)

set(PREFR_HEADERS )

set(PREFR_SOURCES
  Cluster.cpp
  Particles.cpp
  Sorter.cpp
  Updater.cpp
  Renderer.cpp
  OSG/OSGRenderer.cpp
  OSG/OSGDefaultParticleSystem.cpp
  Log.cpp
  ParticleSystem.cpp
  cuda/ThrustSorter.cu
  Model.cpp
  GL/GLRenderer.cpp
  Source.cpp
  Config.cpp
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
