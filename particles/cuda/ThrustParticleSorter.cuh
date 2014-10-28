#ifndef THRUSTPARTICLESORTER_H_
#define THRUSTPARTICLESORTER_H_

#include <particles/config.h>
#include "../default/DefaultTypes.h"
#include "../ParticleSorter.h"
#include "../default/GL/GLDefaultParticleSorter.h"


#include <thrust/sort.h>
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/copy.h>

namespace particles
{
  namespace defaultParticleSystem
  {

    namespace GL
    {

      class ThrustParticleSorter : public GLDefaultParticleSorter
      {
      public:


        ThrustParticleSorter(ParticleCollection* arrayParticles, DistanceArray* distanceArray);

        void Sort(SortOrder order);

        virtual void UpdateCameraDistance(const vec3& cameraPosition);
        virtual void UpdateCameraDistance(unsigned int i, const vec3& cameraPosition);

      };

    }
  }

}

#endif
