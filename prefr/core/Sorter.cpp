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
    : _sources( nullptr )
    , _distances( nullptr )
    , _aliveParticles( 0 )
    , _parallel( false )
  { }

  Sorter::~Sorter( )
  {
    delete _distances;
  }

  const DistanceArray* Sorter::getDistanceArray( ) const
  {
    return _distances;
  }

  void Sorter::initDistanceArray( ICamera* camera )
  {
    _distances = new DistanceArray( _particles.size( ) , camera );
  }

  void Sorter::sort( SortOrder /*order*/ )
  {
    TDistUnitContainer::iterator end;
#ifndef PREFR_USE_OPENMP
    end = _distances->begin( ) + _aliveParticles;
#else
    end = _distances->end( );
#endif

#ifdef PREFR_USE_OPENMP
    if ( _parallel )
    {
#ifdef _WINDOWS
      concurrency::parallel_sort(_distances->begin( ), end,
                                    DistanceArray::sortDescending );
#else
      __gnu_parallel::sort( _distances->begin( ) , end ,
                            DistanceArray::sortDescending );
#endif
    }
    else
    {
      std::sort( _distances->begin( ), end, DistanceArray::sortDescending );
    }
#else
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
    for( int i = 0; i < ( int ) _sources->size( ); ++i)
    {
      Source* source = ( *_sources )[ i ];
#else

    for( auto& source : *_sources )
    {

#endif
      if( source->particles( ).empty( ) || !source->active( ))
        continue;

      // TODO
      for( auto particle : source->particles( ))
      {
        {
            updateParticleDistance( &particle, cameraPosition,
                                    renderDeadParticles );
        }

      }
    }

#ifdef PREFR_WITH_LOGGING
    std::cout << "SETUP" << std::endl;
    std::cout << "CAMERA: " << cameraPosition.x << " " << cameraPosition.y << " " << cameraPosition.z << std::endl;
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
    dist.distanceSquared( current->alive( ) || renderDeadParticles ?
                          length2( current->position( ) - cameraPosition ) :
                          -1 );

#ifdef PREFR_WITH_LOGGING
    std::cout << "Particle " << current->id( )
              << "\t" << std::boolalpha << current->alive( )
              << "\t" << current->position( ).x
              << "\t" << current->position( ).y
              << "\t" << current->position( ).z
              << "\t" << dist.distance( )
              << "\t" << _distances->elements[ current->id( )].distance( )
              << std::endl;
#endif
  }

  void Sorter::sources( std::vector< Source* >* sources_ )
  {
    _sources = sources_;
  }

  void Sorter::particles( const ParticleRange& particles_ )
  {
    _particles = particles_;
  }
}
