#ifndef THRUSTPARTICLESORTER_H_
#define THRUSTPARTICLESORTER_H_

#include "../types.h"
#include "../ParticleSorter.h"

#include "CUDADistanceArray.cuh"

#include <thrust/sort.h>
#include <thrust/copy.h>

namespace prefr
{

  class ThrustParticleSorter : public ParticleSorter
  {
  public:

    ThrustParticleSorter(ParticleCollection* arrayParticles);

    void Sort(SortOrder order);

    virtual void UpdateCameraDistance(const glm::vec3& cameraPosition);
    virtual void UpdateCameraDistance(unsigned int i,
                                      const glm::vec3& cameraPosition);

    virtual void InitDistanceArray();
  };

}

#endif
