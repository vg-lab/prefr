#ifndef THRUSTPARTICLESORTER_H_
#define THRUSTPARTICLESORTER_H_

#include "../types.h"
#include "../ParticleSorter.h"

#include "CUDADistanceArray.cuh"

namespace prefr
{

  class ThrustParticleSorter : public ParticleSorter
  {
  public:

    PREFR_API ThrustParticleSorter( const ParticleCollection& arrayParticles );
    PREFR_API virtual ~ThrustParticleSorter();

    PREFR_API void Sort(SortOrder order);

    virtual void UpdateCameraDistance( const glm::vec3& cameraPosition,
                                       bool discardDeadParticles = true );

    virtual void UpdateCameraDistance( const tparticle_ptr current,
                                       const glm::vec3& cameraPosition,
                                       bool discardDeadParticles = true );

    virtual void InitDistanceArray();
  };

} // namespace prefr


#endif
