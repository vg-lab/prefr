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
#include "Particles.h"
#include "DistanceArray.hpp"
#include <iostream>

#include "Source.h"

namespace prefr
{
  static inline float length2(const glm::vec3& elem)
  {
    return (elem.x * elem.x + elem.y * elem.y + elem.z * elem.z);
  }

  class Sorter
  {
    friend class ParticleSystem;

  public:

    enum SortOrder
    {
      Descending = 0,
      Ascending = 1,
    };

    PREFR_API Sorter( );

    PREFR_API virtual ~Sorter();

    PREFR_API virtual void Sort(SortOrder order = Descending);

    PREFR_API virtual void UpdateCameraDistance( const glm::vec3& cameraPosition,
                                                 bool renderDeadParticles = false );

    PREFR_API virtual void UpdateCameraDistance( const tparticle_ptr current,
                                                 const glm::vec3& cameraPosition,
                                                 bool renderDeadParticles = false );

    PREFR_API virtual void InitDistanceArray();

protected:

    void clusters( ClustersArray* clusters );

    void particles( const ParticleRange& particles );

    ParticleCollection _particles;

    ClustersArray* _clusters;

    SourcesArray* emissionNodes;

    DistanceArray* distances;

    unsigned int _aliveParticles;
  };
}



#endif /* PARTICLESORTER_H_ */
