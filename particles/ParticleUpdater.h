/*
 * ParticleUpdater.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLEUPDATER_H_
#define PARTICLEUPDATER_H_

#include "config.h"
#include "ElementCollection.hpp"

using namespace utils;

namespace particles
{
  // This class updates the particles through the particle life cycle
  class ParticleUpdater
  {
public:
    ParticleCollection* particles;

    // Updates collection particles
    void Update(float deltaTime);

    // Update a single particle by the given particle collection index
    void Update(unsigned int i, float deltaTime);

  };
}



#endif /* PARTICLEUPDATER_H_ */
