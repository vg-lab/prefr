source_group(\\ FILES CMakeLists.txt)

set(PREFR_PUBLIC_HEADERS ${COMMON_INCLUDES}
  # To ensure that Glew is included before GL this
  # should be the first file included by the library
  types.h
  #
  ElementCollection.hpp
  InterpolationSet.hpp
  DistanceArray.hpp
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
  Particle.h
  log.h
  ParticleSystem.h
)

set(PREFR_HEADERS )

set(PREFR_SOURCES ${COMMON_SOURCES}
  ParticleEmitter.cpp
  ParticleSorter.cpp
  ParticleUpdater.cpp
  OSG/OSGDefaultParticleRenderer.cpp
  OSG/OSGDefaultParticleSystem.cpp
  ParticleSystem.cpp
  cuda/ThrustParticleSorter.cu
  ParticlePrototype.cpp
  GL/GLDefaultParticleRenderer.cpp
)

if (WITH_CUDA)
  cuda_common_library(prefr)
else()
  common_library(prefr)
endif()
