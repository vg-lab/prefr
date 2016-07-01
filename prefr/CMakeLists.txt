source_group(\\ FILES CMakeLists.txt)

set(PREFR_PUBLIC_HEADERS
  # To ensure that Glew is included before GL this
  # should be the first file included by the library
  types.h
  #  
  ElementCollection.hpp
  InterpolationSet.hpp
  DistanceArray.hpp
  Timer.hpp
  Particles.h
  ParticlePrototype.h
  OSG/OSGRenderConfig.h
  OSG/OSGDefaultParticleSystem.h
  OSG/OSGDefaultParticleRenderer.h
  RenderConfig.h
  cuda/ThrustParticleSorter.cuh
  cuda/CUDADistanceArray.cuh
  ParticleEmitter.h
  ParticleRenderer.h
  ParticleSorter.h
  ParticleUpdater.h
  GL/GLDefaultParticleRenderer.h  
  log.h
  ParticleSystem.h
  ParticleEmissionNode.h
  GL/shd/GL-frag.glsl
  GL/shd/GL-vert.glsl
  Config.h
)

set(PREFR_HEADERS )

set(PREFR_SOURCES
  Particles.cpp
  ParticleEmitter.cpp
  ParticleSorter.cpp
  ParticleUpdater.cpp
  OSG/OSGDefaultParticleRenderer.cpp
  OSG/OSGDefaultParticleSystem.cpp
  ParticleSystem.cpp
  cuda/ThrustParticleSorter.cu
  ParticlePrototype.cpp
  GL/GLDefaultParticleRenderer.cpp
  ParticleEmissionNode.cpp
  Config.cpp
)

set(PREFR_LINK_LIBRARIES
  ${GLEW_LIBRARIES}
  ${GLUT_LIBRARIES}
  ${BOOST_LIBRARIES}
)

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
  
  cuda_common_library(prefr)
    
else()
  common_library(prefr)
endif()
