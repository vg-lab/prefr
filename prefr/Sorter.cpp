/*
 * ParticleSorter.cpp
 *
 *  Created on: 25/11/2014
 *      Author: sgalindo
 */

#include "Sorter.h"

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

#ifdef SERIALIZE_BEFORE_SORT
  void Sorter::SerializeAttributes( )
  {
    auto posIt = distances->positions->begin( );
    auto sizeIt = distances->sizes->begin( );
    auto colorIt = distances->colors->begin( );

    for( Particles::iterator it = _particles.begin( );
         it != _particles.end( ); it++)
    {

      *posIt = it.position( );
      *sizeIt = it.size( );
      *colorIt = it.color( );

      posIt++;
      sizeIt++;
      colorIt++;
    }

  }
#endif

  void Sorter::Sort(SortOrder order)
  {

    tdcontainter::iterator end = distances->begin() + _aliveParticles;

    std::sort(distances->begin(), end, order == SortOrder::Descending? DistanceArray::sortDescending : DistanceArray::sortAscending);

  }

  void Sorter::UpdateCameraDistance( const glm::vec3& cameraPosition,
                                             bool renderDeadParticles )
  {
    _aliveParticles = 0;
    distances->ResetCounter();

    for( auto cluster : *_clusters )
    {
      if( cluster->active( ) || renderDeadParticles )
      {
        for( tparticle particle = _particles.begin( );
             particle != _particles.end( );
             particle++ )
        {
          UpdateCameraDistance( &particle, cameraPosition, renderDeadParticles );
          _aliveParticles++;
        }
      }
    }
  }

  void Sorter::UpdateCameraDistance( const tparticle_ptr current,
                                     const glm::vec3& cameraPosition,
                                     bool renderDeadParticles )
  {

    DistanceUnit& dist = distances->next();

    dist.Id() = current->id( );

    dist.Distance() = current->alive() || renderDeadParticles ?
                      length2(current->position( ) - cameraPosition) :
                      -1;

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



