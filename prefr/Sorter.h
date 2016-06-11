/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: Sergio Galindo <sergio.galindo@urjc.es>
 *
 * This file is part of PReFr <https://gmrv.gitlab.com/nsviz/prefr>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#ifndef __PREFR__PARTICLE_SORTER__
#define __PREFR__PARTICLE_SORTER__

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

    PREFR_API virtual void UpdateCameraDistance(
      const glm::vec3& cameraPosition,
      bool renderDeadParticles = false );

    PREFR_API virtual void UpdateCameraDistance(
      const tparticle_ptr current,
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



#endif /* __PREFR__PARTICLE_SORTER__ */
