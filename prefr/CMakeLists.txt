source_group(\\ FILES CMakeLists.txt)

set(PREFR_PUBLIC_HEADERS ${COMMON_INCLUDES}
ParticlePrototype.h
ParticleDrawable.h
OSG/OSGRenderConfig.h
OSG/OSGDefaultParticleSystem.h
OSG/OSGDefaultParticleRenderer.h
RenderConfig.h
cuda/ThrustParticleSorter.cuh
cuda/CUDADistanceArray.cuh
ParticleEmitter.h
types.h
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
./ParticleEmitter.cpp
./ParticleSorter.cpp
./ParticleUpdater.cpp
./OSG/OSGDefaultParticleRenderer.cpp
./OSG/OSGDefaultParticleSystem.cpp
./ParticleSystem.cpp
./cuda/ThrustParticleSorter.cuh
./cuda/CUDADistanceArray.cuh
./cuda/ThrustParticleSorter.cu
./ParticlePrototype.cpp
./GL/GLDefaultParticleRenderer.cpp
)

common_library(prefr)
