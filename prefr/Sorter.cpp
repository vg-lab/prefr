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

#include <parallel/algorithm>

namespace prefr
{

  Sorter::Sorter( )
  : emissionNodes( nullptr )
  , distances( nullptr )
  , _aliveParticles( 0 )
  {}

  Sorter::~Sorter()
  {

    if ( distances )
      delete( distances );
  }

  void Sorter::InitDistanceArray()
  {
    distances = new DistanceArray( _particles.size );
  }

  void Sorter::Sort(SortOrder /*order*/)
  {

    TDistUnitContainer::iterator end = distances->begin() + _aliveParticles;

//    std::sort(distances->begin(), end, DistanceArray::sortDescending );
    __gnu_parallel::sort( distances->begin( ), end,
                          DistanceArray::sortDescending );
  }

  void Sorter::UpdateCameraDistance( const glm::vec3& cameraPosition,
                                     bool renderDeadParticles )
  {
//    _aliveParticles = 0;
    distances->ResetCounter();

    #pragma omp parallel for
    for( unsigned int i = 0; i < _clusters->size( ); ++i)
//    for( auto cluster : *_clusters )
    {
      Cluster* cluster = (*_clusters)[ i ];

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

    DistanceUnit& dist = distances->at( current->id( ) );

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
