/*
 * Copyright (c) 2014-2018 GMRV/URJC.
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

#ifndef __PREFR__SORTER__
#define __PREFR__SORTER__

#include <prefr/api.h>

#include <iostream>

#include "../utils/types.h"

#include "Particles.h"
#include "DistanceArray.hpp"
#include "Source.h"

namespace prefr
{
  static inline float length2( const glm::vec3& elem )
  {
    return( elem.x * elem.x + elem.y * elem.y + elem.z * elem.z );
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

    PREFR_API virtual ~Sorter( );

    PREFR_API virtual void sort( SortOrder order = Descending );

    PREFR_API
    virtual void updateCameraDistance( const glm::vec3& cameraPosition,
                                       bool renderDeadParticles = false );

    PREFR_API
    void updateCameraDistance( bool renderDeadParticles = false );

    PREFR_API
    virtual void updateParticleDistance( const tparticle_ptr current,
                                         const glm::vec3& cameraPosition,
                                         bool renderDeadParticles = false );

    PREFR_API virtual void initDistanceArray( ICamera* camera );

    PREFR_API void particles( const ParticleRange& particles );

    PREFR_API void aliveParticles( unsigned int alive );
protected:

    void sources( std::vector< Source* >* sources_ );

    ParticleCollection _particles;

    std::vector< Source* >* _sources;

    DistanceArray* _distances;

    unsigned int _aliveParticles;

    bool _parallel;

  };
}



#endif /* __PREFR__SORTER__ */
