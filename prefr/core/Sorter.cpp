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
  : _clusters( nullptr )
  , _emissionNodes( nullptr )
  , _distances( nullptr )
  , _aliveParticles( 0 )
  , _parallel( false )
  {}

  Sorter::~Sorter()
  {

    if ( _distances )
      delete( _distances );
  }

  void Sorter::initDistanceArray( ICamera* camera )
  {
    _distances = new DistanceArray( _particles.size( ), camera );
  }

  void Sorter::sort(SortOrder /*order*/)
  {

    TDistUnitContainer::iterator end;
#ifndef PREFR_USE_OPENMP
    end = _distances->begin( ) + _aliveParticles;
#else
    end = _distances->end( );
#endif

#ifdef PREFR_USE_OPENMP
    if( _parallel )
    {
#ifdef _WINDOWS
    concurrency::parallel_sort(_distances->begin( ), end,
                                  DistanceArray::sortDescending );
#else
    __gnu_parallel::sort( _distances->begin( ), end,
                          DistanceArray::sortDescending );
#endif
    }
    else
      std::sort( _distances->begin( ), end, DistanceArray::sortDescending );
#endif

#ifdef PREFR_WITH_LOGGING
    std::cout << "SORT" << std::endl;
    std::cout << "IDS:";
    for( auto id : _distances->elements )
    {
      std::cout << "\t" << id.id( );
    }
    std::cout << std::endl;

    std::cout << "DIST:";
    for( auto dist : _distances->elements )
    {
      std::cout << "\t" << dist.distance( );
    }
    std::cout << std::endl;
#endif
  }

  void Sorter::updateCameraDistance( const glm::vec3& cameraPosition,
                                     bool renderDeadParticles )
  {
    _distances->resetCounter( );

#ifdef PREFR_USE_OPENMP

    #pragma omp parallel for if( _parallel )
    for( int i = 0; i < ( int ) _clusters->size( ); ++i)
    {
      Cluster* cluster = ( *_clusters )[ i ];

#else

    for( auto cluster : *_clusters )
    {

#endif

      if( cluster->active( ) || renderDeadParticles )
      {
//        for( tparticle particle = cluster->particles( ).begin( );
//             particle != cluster->particles( ).end( );
//             particle++ )
        for( auto particle : cluster->particles( ))
        {
#ifndef PREFR_USE_OPENMP
          if( particle.alive( ) || renderDeadParticles )
#endif
          updateParticleDistance( &particle, cameraPosition,
                                  renderDeadParticles );

        }
      }
    }

#ifdef PREFR_WITH_LOGGING
    std::cout << "SETUP" << std::endl;
    std::cout << "IDS:";
    for( auto id : _distances->elements )
    {
      std::cout << "\t" << id.id( );
    }
    std::cout << std::endl;

    std::cout << "DIST:";
    for( auto dist : _distances->elements )
    {
      std::cout << "\t" << dist.distance( );
    }
    std::cout << std::endl;
#endif
  }

  void Sorter::updateCameraDistance( bool renderDeadParticles )
  {
    assert( _distances->_camera );

    updateCameraDistance( _distances->_camera->PReFrCameraPosition( ),
                          renderDeadParticles );

  }

  void Sorter::updateParticleDistance( const tparticle_ptr current,
                                       const glm::vec3& cameraPosition,
                                       bool renderDeadParticles )
  {

    DistanceUnit& dist =
#ifdef PREFR_USE_OPENMP
        _distances->at( current->id( ));
#else
        *_distances->next( );
#endif

    dist.id( current->id( ));

    dist.distance( current->alive() || renderDeadParticles ?
                   length2(current->position( ) - cameraPosition ) :
                   -1 );

#ifdef PREFR_WITH_LOGGING
    std::cout << "Particle " << current->id( )
              << " " << std::boolalpha << current->alive( )
              << "\t" << dist.distance( )
              << "\t" << _distances->elements[ current->id( )].distance( )
              << std::endl;
#endif
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
