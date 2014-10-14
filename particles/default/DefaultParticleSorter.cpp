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
      std::sort(distances->begin(), distances->end(), order == SortOrder::Descending? SortUnit::sortDescending : SortUnit::sortAscending);
    }

  }

}


