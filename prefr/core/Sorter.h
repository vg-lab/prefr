/*
 * Copyright (c) 2014-2020 VG-Lab/URJC.
 *
 * Authors: Sergio E. Galindo <sergio.galindo@urjc.es>
 *
 * This file is part of PReFr <https://github.com/gmrvvis/prefr>
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
    return ( elem.x * elem.x + elem.y * elem.y + elem.z * elem.z );
  }

  class PREFR_API Sorter
  {
    friend class ParticleSystem;

  public:

    enum SortOrder
    {
      Descending = 0 ,
      Ascending = 1 ,
    };

    Sorter( );

    virtual ~Sorter( );

    const DistanceArray* getDistanceArray( ) const;

    virtual void sort( SortOrder order = Descending );

    virtual void updateCameraDistance( const glm::vec3& cameraPosition ,
                                       bool renderDeadParticles = false );


    void updateCameraDistance( bool renderDeadParticles = false );


    virtual void updateParticleDistance( const tparticle_ptr current ,
                                         const glm::vec3& cameraPosition ,
                                         bool renderDeadParticles = false );

    virtual void initDistanceArray( ICamera* camera );

    void particles( const ParticleRange& particles );

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
