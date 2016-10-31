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
#ifndef __PREFR__CUDA_DISTANCE_ARRAY__
#define __PREFR__CUDA_DISTANCE_ARRAY__

#include "../core/DistanceArray.hpp"

#ifdef PREFR_USE_CUDA
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#endif

namespace prefr
{

  class CUDADistanceArray : public DistanceArray
  {
  public:

#ifdef PREFR_USE_CUDA

    thrust::device_vector< int > deviceID;
    thrust::device_vector< float > deviceDistances;

    std::vector< int > translatedIDs;

#endif

    CUDADistanceArray ( unsigned int size)
    : DistanceArray( size )
    {
      translatedIDs.resize( size );
    }

    virtual inline const int& getID( unsigned int i ) const
    {
      return translatedIDs[ ids[ i ]];
    }

    virtual inline float& getDistance( unsigned int i )
    {
      return distances[ i ];
    }

  };

}

#endif /* __PREFR__CUDA_DISTANCE_ARRAY__ */
