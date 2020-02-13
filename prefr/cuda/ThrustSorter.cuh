/*
 * Copyright (c) 2014-2020 GMRV/URJC.
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

#ifndef __PREFR__THRUST_SORTER__
#define __PREFR__THRUST_SORTER__

#include "../core/Sorter.h"
#include "../utils/types.h"
#include "CUDADistanceArray.cuh"

namespace prefr
{

  class ThrustSorter : public Sorter
  {
  public:

    PREFR_API ThrustSorter(  );
    PREFR_API virtual ~ThrustSorter( );

    PREFR_API virtual void sort( SortOrder order );

    PREFR_API
    virtual void updateCameraDistance( const glm::vec3& cameraPosition,
                                       bool renderDeadParticles = false );

    PREFR_API
    virtual void updateParticleDistance( const tparticle_ptr current,
                                         const glm::vec3& cameraPosition,
                                         bool renderDeadParticles = false );

    PREFR_API virtual void initDistanceArray( ICamera* camera );

  };

} // namespace prefr


#endif
