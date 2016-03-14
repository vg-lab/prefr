/*
 * ParticleSorter.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLESORTER_H_
#define PARTICLESORTER_H_

#include <prefr/api.h>
#include "types.h"
#include "ElementCollection.hpp"
#include "DistanceArray.hpp"
#include <iostream>
#include "ParticleEmissionNode.h"

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

    PREFR_API ParticleSorter( const ParticleCollection& particlesArray );

    PREFR_API virtual ~ParticleSorter();

    PREFR_API virtual void Sort(SortOrder order = Descending);

    PREFR_API virtual void UpdateCameraDistance( const glm::vec3& cameraPosition,
                                                 bool renderDeadParticles = false );

    PREFR_API virtual void UpdateCameraDistance( const tparticle_ptr current,
                                                 const glm::vec3& cameraPosition,
                                                 bool renderDeadParticles = false );

    PREFR_API virtual void InitDistanceArray();

#ifdef SERIALIZE_BEFORE_SORT
    virtual void SerializeAttributes( );
#endif

    ParticleCollection* particles;

    EmissionNodesArray* emissionNodes;

    DistanceArray* distances;

    unsigned int aliveParticles;
  };
}



#endif /* PARTICLESORTER_H_ */
