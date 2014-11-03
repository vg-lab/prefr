/*
 * ParticleUpdater.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLEUPDATER_H_
#define PARTICLEUPDATER_H_

#include <particles/config.h>
#include "ElementCollection.hpp"
#include "ParticleEmitter.h"

using namespace utils;

namespace particles
{
  // This class updates the particles through the particle life cycle
  class ParticleUpdater
  {
public:
    ParticleCollection* particles;
    PrototypesArray* prototypes;
    vector<int>* refPrototypes;

    ParticleUpdater(ParticleCollection* particlesArray, PrototypesArray* particlePrototype, vector<int>* _refPrototypes)
    : particles( particlesArray )
    , prototypes( particlePrototype )
    , refPrototypes( _refPrototypes )
    {}

    virtual ~ParticleUpdater()
    {
      delete (particles);
      delete (prototypes);
    }

    // Updates collection particles
    virtual int Update(float deltaTime) = 0;

    // Update a single particle by the given particle collection index
    virtual void Update(unsigned int i, float deltaTime) = 0;

  };
}



#endif /* PARTICLEUPDATER_H_ */
