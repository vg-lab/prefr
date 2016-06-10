/*
 * ParticleSorter.cpp
 *
 *  Created on: 25/11/2014
 *      Author: sgalindo
 */

#include "ParticleSorter.h"

namespace prefr
{

  ParticleSorter::ParticleSorter( const ParticleCollection& particlesArray  )
  : particles( new ParticleCollection( particlesArray ) )
  , emissionNodes( nullptr )
  , distances( nullptr )
  , aliveParticles( 0 )
  {}

  ParticleSorter::~ParticleSorter()
  {
    delete( particles );

    if ( distances )
      delete( distances );
  }

  void ParticleSorter::InitDistanceArray()
  {
    distances = new DistanceArray( particles->size );
  }

  void ParticleSorter::Sort(SortOrder order)
  {

    tdcontainter::iterator end = distances->begin() + aliveParticles;

    std::sort(distances->begin(), end, order == SortOrder::Descending? DistanceArray::sortDescending : DistanceArray::sortAscending);
  }

  void ParticleSorter::UpdateCameraDistance( const glm::vec3& cameraPosition,
                                             bool renderDeadParticles )
  {
    aliveParticles = 0;
    distances->ResetCounter();
    for (EmissionNodesArray::iterator nodit = emissionNodes->begin();
         nodit != emissionNodes->end();
         nodit++)
    {
      if (!(*nodit) || !(*nodit)->Active())
        continue;

      for ( tparticle it = (*nodit)->particles->begin( );
            it != (*nodit)->particles->end( );
            it++)
      {
        if (it.alive())
        {
          UpdateCameraDistance( &it, cameraPosition, renderDeadParticles );
          aliveParticles++;
        }
      }
    }


//    SerializeAttributes( );
  }

  void ParticleSorter::UpdateCameraDistance( const tparticle_ptr current,
                                             const glm::vec3& cameraPosition,
                                             bool renderDeadParticles )
  {
//    DistanceUnit& dist = distances->at(current->id);
    DistanceUnit* dist = distances->next();

    dist->Id( current->id( ));

    dist->Distance( current->alive( ) || renderDeadParticles ?
                   length2(current->position( ) - cameraPosition) :
                   -1 );

  }



}



