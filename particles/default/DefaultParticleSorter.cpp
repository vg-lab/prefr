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

    void DefaultParticleSorter::UpdateCameraDistance(vec3 cameraPosition)
    {
      for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
      {
        UpdateCameraDistance((*it)->id, cameraPosition);
      }
    }

    void DefaultParticleSorter::UpdateCameraDistance(unsigned int i, vec3 cameraPosition)
    {
      tparticleptr current = particles->elements->at(i);

      distances->at(i).Id() = current->id;
      distances->at(i).Distance() = current->Alive() ?  (current->position - cameraPosition).length() : -1;
    }

  }

}


