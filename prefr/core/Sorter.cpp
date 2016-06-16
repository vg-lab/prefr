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
#include "Sorter.h"

#ifdef PREFR_USE_OPENMP
#ifdef _WINDOWS
#include <ppl.h>
#else
#include <parallel/algorithm>
#endif
#endif

namespace prefr
{

  Sorter::Sorter( )
  : _emissionNodes( nullptr )
  , _distances( nullptr )
  , _aliveParticles( 0 )
  {}

  Sorter::~Sorter()
  {

    if ( _distances )
      delete( _distances );
  }

  void Sorter::InitDistanceArray()
  {
    _distances = new DistanceArray( _particles.size );
  }

  void Sorter::Sort(SortOrder /*order*/)
  {

    TDistUnitContainer::iterator end = _distances->begin() + _aliveParticles;

#ifdef PREFR_USE_OPENMP
#ifdef _WINDOWS
    concurrency::parallel_sort(_distances->begin( ), end,
                                  DistanceArray::sortDescending );
#else
    __gnu_parallel::sort( _distances->begin( ), end,
                          DistanceArray::sortDescending );
#endif
#else
    std::sort( _distances->begin(), end, DistanceArray::sortDescending );
#endif

  }

  void Sorter::UpdateCameraDistance( const glm::vec3& cameraPosition,
                                     bool renderDeadParticles )
  {
//    _aliveParticles = 0;
    _distances->ResetCounter();

#ifdef PREFR_USE_OPENMP
    #pragma omp parallel for
    for( int i = 0; i < ( int ) _clusters->size( ); ++i)
    {
      Cluster* cluster = (*_clusters)[ i ];
#else
    for( auto cluster : *_clusters )
    {
#endif

      if( cluster->active( ) || renderDeadParticles )
      {
        for( tparticle particle = cluster->particles( ).begin( );
             particle != cluster->particles( ).end( );
             particle++ )
        {
          UpdateCameraDistance( &particle, cameraPosition,
                                renderDeadParticles );

        }
      }
    }

  }

  void Sorter::UpdateCameraDistance( const tparticle_ptr current,
                                     const glm::vec3& cameraPosition,
                                     bool renderDeadParticles )
  {

    DistanceUnit& dist = _distances->at( current->id( ) );

    dist.Id( current->id( ));

    dist.Distance( current->alive() || renderDeadParticles ?
                   length2(current->position( ) - cameraPosition) :
                   -1);
  }


  void Sorter::clusters( ClustersArray* clusters_ )
  {
    _clusters = clusters_;
  }

  void Sorter::particles( const ParticleRange& particles_ )
  {
    _particles = particles_;
  }

}
