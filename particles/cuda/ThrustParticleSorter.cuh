#ifndef THRUSTPARTICLESORTER_H_
#define THRUSTPARTICLESORTER_H_

#include <particles/config.h>
#include "CUDAThrustTypes.cuh"
#include "../ParticleSorter.h"


#include <thrust/sort.h>
#include <thrust/copy.h>

namespace particles
{
  namespace defaultParticleSystem
  {

    namespace CUDATHRUST
    {

      class ThrustParticleSorter : public ParticleSorter
      {
      public:

        distanceArray* distances;

        ThrustParticleSorter(ParticleCollection* arrayParticles, distanceArray* distanceArray);

        void Sort(SortOrder order);

        virtual void UpdateCameraDistance(const glm::vec3& cameraPosition);
        virtual void UpdateCameraDistance(unsigned int i, const glm::vec3& cameraPosition);

      };

    }
  }

}

#endif
