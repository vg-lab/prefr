/*
 * ParticleSorter.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLESORTER_H_
#define PARTICLESORTER_H_

#include "config.h"
#include "ElementCollection.hpp"

using namespace utils;

namespace particles
{
  class ParticleSorter
  {
public:

    enum SortOrder
    {
      Descending = 0,
      Ascending = 1,
    };

    ParticleCollection* particles;

    ParticleSorter(ParticleCollection* particlesArray)
    : particles(particlesArray)
    {}

    virtual ~ParticleSorter()
    {
      delete( particles );
    }

    virtual void Sort(SortOrder order = Descending) = 0;
  };
}



#endif /* PARTICLESORTER_H_ */
