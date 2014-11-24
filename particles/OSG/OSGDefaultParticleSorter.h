/*
 * DefaultParticleSorter.h
 *
 *  Created on: 15/10/2014
 *      Author: sergio
 */

#ifndef DEFAULTPARTICLESORTER_H_
#define DEFAULTPARTICLESORTER_H_

#include <particles/config.h>
#include "OSGTypes.h"
#include "../ParticleSystem.h"


namespace particles
{
  namespace defaultParticleSystem
  {
    namespace OSGParticleSystem
    {
      class OSGDefaultParticleSorter : public ParticleSorter
      {
      public:
        distanceArray* distances;

        OSGDefaultParticleSorter(ParticleCollection* particlesArray, distanceArray* distancesArray)
        : ParticleSorter(particlesArray)
        , distances(distancesArray)
        {}

        virtual ~OSGDefaultParticleSorter()
        {
        }

        virtual void Sort(SortOrder order = Descending);

        virtual void UpdateCameraDistance(const glm::vec3& cameraPosition);
        virtual void UpdateCameraDistance(unsigned int i, const glm::vec3& cameraPosition);

      };
    }
  }
}


#endif /* DEFAULTPARTICLESORTER_H_ */
