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
//    std::vector<DistanceUnit>::iterator it;
//    for (it = distances->begin(); it != distances->end(); it++)
//    {
//      *it->id = rand() % particles->size;
//      *it->distance = rand() % 4;
//    }

// #include <parallel/algorithm>
// __gnu_parallel::sort(distances->begin(),
//               distances->end() ,
//               order == SortOrder::Descending ?
//               DistanceArray::sortDescending : DistanceArray::sortAscending );

//    std::vector<DistanceUnit>::iterator it;
//    for (it = distances->begin(); it != distances->end(); it++)
//    {
//      *it->id = rand() % particles->size;
//      *it->distance = rand() % 4;
//    }

  }

  void ParticleSorter::UpdateCameraDistance(const glm::vec3& cameraPosition)
  {
    aliveParticles = 0;
    distances->ResetCounter();
    for (EmissionNodesArray::iterator nodit = emissionNodes->begin();
         nodit != emissionNodes->end();
         nodit++)
    {
      if (!(*nodit) || !(*nodit)->Active())
        continue;

      for (tparticleContainer::iterator it = (*nodit)->particles->start;
           it != (*nodit)->particles->end;
           it++)
      {
        if ((*it)->Alive())
        {
          UpdateCameraDistance((*it), cameraPosition);
          aliveParticles++;
        }
      }
    }
  }

  void ParticleSorter::UpdateCameraDistance(const tparticle_ptr current, const glm::vec3& cameraPosition)
  {
//    DistanceUnit& dist = distances->at(current->id);
    DistanceUnit& dist = distances->next();

    dist.Id() = current->id;

    dist.Distance() = current->Alive() ?  length2(current->position - cameraPosition) : -1;

  }



}



