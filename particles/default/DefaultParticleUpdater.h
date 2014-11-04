/*
 * DefaultParticleUpdater.h
 *
 *  Created on: 14/10/2014
 *      Author: sergio
 */

#ifndef DEFAULTPARTICLEUPDATER_H_
#define DEFAULTPARTICLEUPDATER_H_


#include <particles/config.h>
#include "DefaultParticleSystem.h"
#include "../ParticleUpdater.h"


namespace particles
{
  namespace defaultParticleSystem
  {
    class DefaultParticleUpdater : public ParticleUpdater
    {
public:

      DefaultParticleUpdater(ParticleCollection* particlesArray, PrototypesArray* particlePrototype, vector<int>* _refPrototypes);
      virtual ~DefaultParticleUpdater();

      virtual int Update(float deltaTime);
      virtual void Update(unsigned int i, float deltaTime);

    };


  }
}


#endif /* DEFAULTPARTICLEUPDATER_H_ */