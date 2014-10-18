#ifndef THRUSTPARTICLESORTER_H_
#define THRUSTPARTICLESORTER_H_

#include "../config.h"
#include "../default/DefaultTypes.h"
#include "../ParticleSorter.h"
#include "../default/DefaultParticleSorter.h"

#include <thrust/sort.h>
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>

namespace particles
{
    namespace defaultParticleSystem
    {

        class ThrustParticleSorter : DefaultParticleSorter
        {



          ThrustParticleSorter(ParticleCollection* arrayParticles, DistanceArray* distanceArray);

          void Sort(SortOrder order);

//          virtual void UpdateCameraDistance(vec3 cameraPosition);
//          virtual void UpdateCameraDistance(unsigned int i, vec3 cameraPosition);

        };


    }

}

#endif
