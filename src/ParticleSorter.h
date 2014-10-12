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

    ParticleCollection* particles;

    void Sort();
  };
}



#endif /* PARTICLESORTER_H_ */
