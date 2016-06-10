/*
 * ParticleSorter.cpp
 *
 *  Created on: 25/11/2014
 *      Author: sgalindo
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

    tdcontainter::iterator end = distances->begin() + _aliveParticles;

//    std::sort(distances->begin(), end, DistanceArray::sortDescending );
    __gnu_parallel::sort( distances->begin( ), end, DistanceArray::sortDescending );
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
          UpdateCameraDistance( &particle, cameraPosition, renderDeadParticles );

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



