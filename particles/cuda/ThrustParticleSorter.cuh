#ifndef THRUSTPARTICLESORTER_H_
#define THRUSTPARTICLESORTER_H_

#include <particles/config.h>
#include "../default/DefaultTypes.h"
#include "../ParticleSorter.h"
#include "../default/DefaultParticleSorter.h"


#include <thrust/sort.h>
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/copy.h>

namespace particles
{
    namespace defaultParticleSystem
    {

        class ThrustParticleSorter : public DefaultParticleSorter
        {
		public:


          ThrustParticleSorter(ParticleCollection* arrayParticles, DistanceArray* distanceArray);

          void Sort(SortOrder order);

          virtual void UpdateCameraDistance(const vec3& cameraPosition);
          virtual void UpdateCameraDistance(unsigned int i, const vec3& cameraPosition);

        };


    }

}

#endif
