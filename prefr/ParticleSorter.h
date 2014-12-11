/*
 * ParticleSorter.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLESORTER_H_
#define PARTICLESORTER_H_

#include "types.h"
#include "ElementCollection.hpp"
#include "DistanceArray.hpp"

#include <iostream>

using namespace utils;

namespace prefr
{
  static inline float length2(const glm::vec3& elem)
  {
    return (elem.x * elem.x + elem.y * elem.y + elem.z * elem.z);
  }

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

    ParticleSorter( const ParticleCollection& particlesArray );

    virtual ~ParticleSorter();

    virtual void Sort(SortOrder order = Descending);

    virtual void UpdateCameraDistance(const glm::vec3& cameraPosition);
    virtual void UpdateCameraDistance(const tparticle_ptr current,
                                      const glm::vec3& cameraPosition);

    virtual void InitDistanceArray();
  };
}



#endif /* PARTICLESORTER_H_ */
