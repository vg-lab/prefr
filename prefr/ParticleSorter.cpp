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
  }

  void ParticleSorter::UpdateCameraDistance(const glm::vec3& cameraPosition)
  {
    for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
    {
      UpdateCameraDistance((*it)->id, cameraPosition);
    }
  }

  void ParticleSorter::UpdateCameraDistance(unsigned int i, const glm::vec3& cameraPosition)
  {
    tparticle_ptr current = particles->elements->at(i);

    distances->elements->at(i).Id() = current->id;

    distances->elements->at(i).Distance() = current->Alive() ?  glm::length(current->position - cameraPosition) : -1;

  }



}



