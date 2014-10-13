/*
 * ParticleEmitter.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLEEMITTER_H_
#define PARTICLEEMITTER_H_

#include "config.h"
#include "ElementCollection.hpp"

using namespace utils;

namespace particles
{
  class ParticleEmitter
  {
  public:

    ParticleCollection* particles;


    ParticleEmitter(ParticleCollection* particlesArray) : particles(particlesArray){}
    virtual ~ParticleEmitter(){}

    virtual void Update(float deltaTime);

    virtual void Emit(unsigned int i, float deltaTime);

  };


}



#endif /* PARTICLEEMITTER_H_ */
