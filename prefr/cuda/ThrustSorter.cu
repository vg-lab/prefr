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
#include <thrust/sort.h>
#include <thrust/copy.h>
#include "ThrustSorter.cuh"

namespace prefr
{

  ThrustSorter::ThrustSorter( )
  { }

  ThrustSorter::~ThrustSorter( )
  { }

  void ThrustSorter::initDistanceArray( )
  {
    _distances = new CUDADistanceArray( _particles.size );

    CUDADistanceArray* cda = static_cast< CUDADistanceArray* >( _distances );

    cda->deviceID.resize( _particles.size );
    cda->deviceDistances.resize( _particles.size );
  }

  void ThrustSorter::sort( SortOrder order )
  {

    CUDADistanceArray* cda = static_cast< CUDADistanceArray* >( _distances );

    std::vector< int >::iterator hostidbegin = _distances->ids->begin( );
    std::vector< int >::iterator hostidend = hostidbegin + _aliveParticles;

    std::vector< float >::iterator hostdistbegin =
        _distances->distances->begin( );

    std::vector< float >::iterator hostdistend =
        hostdistbegin  + _aliveParticles;

    thrust::device_vector< int >::iterator deviceidbegin =
        cda->deviceID.begin( );

    thrust::device_vector< int >::iterator deviceidend =
        deviceidbegin + _aliveParticles;

    thrust::device_vector< float >::iterator devicedistbegin =
        cda->deviceDistances.begin( );

    thrust::device_vector< float >::iterator devicedistend =
        devicedistbegin + _aliveParticles;

    thrust::sequence( deviceidbegin, deviceidend );
    thrust::copy( hostdistbegin, hostdistend, devicedistbegin );

    if( order == SortOrder::Ascending )
      thrust::sort_by_key( devicedistbegin, devicedistend,
                           deviceidbegin, thrust::less< float >( ));
    else
      thrust::sort_by_key( devicedistbegin, devicedistend,
                           deviceidbegin, thrust::greater< float >( ));

    thrust::copy( deviceidbegin, deviceidend, hostidbegin );

  }

  void ThrustSorter::updateCameraDistance( const glm::vec3& cameraPosition,
                                           bool renderDeadParticles )
  {
    _aliveParticles = 0;
    _distances->resetCounter( );

    for( auto cluster : *_clusters )
    {
      if( cluster->active( ) || renderDeadParticles )
      {
        for( tparticle particle = cluster->particles( ).begin( );
             particle != cluster->particles( ).end( );
             particle++ )
        {
          updateParticleDistance( &particle, cameraPosition,
                                  renderDeadParticles );
          _aliveParticles++;
        }
      }
    }


  }
  void ThrustSorter::updateParticleDistance( const tparticle_ptr current,
                                             const glm::vec3& cameraPosition,
                                             bool renderDeadParticles )
  {
    DistanceUnit* dist = _distances->next( );
    CUDADistanceArray* cda = static_cast< CUDADistanceArray* >( _distances );
    cda->translatedIDs[ _distances->current ] = current->id( );

    ( *_distances->distances )[ _distances->current ] =
        current->alive( )  || renderDeadParticles ?
        length2( current->position( ) - cameraPosition ) :
        -1;

  }

}
