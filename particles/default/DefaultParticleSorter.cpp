/*
 * DefaultParticleSorter.cpp
 *
 *  Created on: 15/10/2014
 *      Author: sergio
 */

#include "DefaultParticleSorter.h"

namespace particles
{
  namespace defaultParticleSystem
  {

    void DefaultParticleSorter::Sort(SortOrder order)
    {
      std::sort(distances->begin(), distances->end(), order == SortOrder::Descending? DistanceArray::sortDescending : DistanceArray::sortAscending);
    }

    void DefaultParticleSorter::UpdateCameraDistance(const vec3& cameraPosition)
    {
      for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
      {
        UpdateCameraDistance((*it)->id, cameraPosition);
      }
    }

    void DefaultParticleSorter::UpdateCameraDistance(unsigned int i, const vec3& cameraPosition)
    {
      tparticleptr current = particles->elements->at(i);

      distances->ids->at(i) = current->id;
//      distances->elements->at(i).id = &distances->ids->at(i);
      distances->distances->at(i) = current->Alive() ?  glm::length(current->position - cameraPosition) : -1;
//      distances->elements->at(i).distance = &distances->distances->at(i);
    }

  }

}


