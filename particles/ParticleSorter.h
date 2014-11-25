/*
 * ParticleSorter.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLESORTER_H_
#define PARTICLESORTER_H_

#include <particles/config.h>
#include "ElementCollection.hpp"
#include "DistanceArray.hpp"

using namespace utils;

namespace particles
{
  class ParticleSorter
  {
  public:

    enum SortOrder
    {
      Descending = 0,
      Ascending = 1,
    };

    ParticleCollection* particles;

    DistanceArray* distances;

    ParticleSorter(ParticleCollection* particlesArray);

    virtual ~ParticleSorter();

    virtual void Sort(SortOrder order = Descending);

    virtual void UpdateCameraDistance(const glm::vec3& cameraPosition);
    virtual void UpdateCameraDistance(unsigned int i, const glm::vec3& cameraPosition);

    virtual void InitDistanceArray();
  };
}



#endif /* PARTICLESORTER_H_ */
