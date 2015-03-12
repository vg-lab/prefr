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
  , distances( nullptr )
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
    std::sort(distances->begin(), distances->end(), order == SortOrder::Descending? DistanceArray::sortDescending : DistanceArray::sortAscending);
//    std::vector<DistanceUnit>::iterator it;
//    for (it = distances->begin(); it != distances->end(); it++)
//    {
//      *it->id = rand() % particles->size;
//      *it->distance = rand() % 4;
//    }
  }

  void ParticleSorter::UpdateCameraDistance(const glm::vec3& cameraPosition)
  {
    for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
    {
      UpdateCameraDistance((*it), cameraPosition);
    }
  }

  void ParticleSorter::UpdateCameraDistance(const tparticle_ptr current, const glm::vec3& cameraPosition)
  {
    DistanceUnit& dist = distances->at(current->id);

    dist.Id() = current->id;

    dist.Distance() = current->Alive() ?  length2(current->position - cameraPosition) : -1;

  }



}



