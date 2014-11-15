/*
 * DefaultParticleSorter.cpp
 *
 *  Created on: 15/10/2014
 *      Author: sergio
 */

#include "OSGDefaultParticleSorter.h"

namespace particles
{
  namespace defaultParticleSystem
  {
    namespace OSGParticleSystem
    {
      void OSGDefaultParticleSorter::Sort(SortOrder order)
      {
        std::sort(distances->begin(), distances->end(), order == SortOrder::Descending? DistanceArray::sortDescending : DistanceArray::sortAscending);
      }

      void OSGDefaultParticleSorter::UpdateCameraDistance(const vec3& cameraPosition)
      {
        for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
        {
          UpdateCameraDistance((*it)->id, cameraPosition);
        }
      }

      void OSGDefaultParticleSorter::UpdateCameraDistance(unsigned int i, const vec3& cameraPosition)
      {
        tparticle_ptr current = particles->elements->at(i);

        distances->elements->at(i).Id() = current->id;
  //      distances->ids->at(i) = current->id;
  //      distances->elements->at(i).id = &distances->ids->at(i);
        distances->elements->at(i).Distance() = current->Alive() ?  glm::length(current->position - cameraPosition) : -1;
  //      distances->distances->at(i) = current->Alive() ?  glm::length(current->position - cameraPosition) : -1;
  //      distances->elements->at(i).distance = &distances->distances->at(i);
      }
    }
  }

}


