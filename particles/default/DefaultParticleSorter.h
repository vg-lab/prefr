/*
 * DefaultParticleSorter.h
 *
 *  Created on: 15/10/2014
 *      Author: sergio
 */

#ifndef DEFAULTPARTICLESORTER_H_
#define DEFAULTPARTICLESORTER_H_

#include <particles/config.h>
#include "DefaultParticleSystem.h"


namespace particles
{
  namespace defaultParticleSystem
  {

    class DefaultParticleSorter : public ParticleSorter
    {
    public:
      distanceArray* distances;

      DefaultParticleSorter(ParticleCollection* particlesArray, distanceArray* distancesArray)
      : ParticleSorter(particlesArray)
      , distances(distancesArray)
      {}

      virtual ~DefaultParticleSorter()
      {
        delete( particles );
        delete( distances );
      }

      virtual void Sort(SortOrder order = Descending);

      virtual void UpdateCameraDistance(const vec3& cameraPosition);
      virtual void UpdateCameraDistance(unsigned int i, const vec3& cameraPosition);

    };

  }
}


#endif /* DEFAULTPARTICLESORTER_H_ */
