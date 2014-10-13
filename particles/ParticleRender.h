/*
 * ParticleRender.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLERENDER_H_
#define PARTICLERENDER_H_

#include "config.h"
#include "ElementCollection.hpp"

using namespace utils;

namespace particles
{

  class ParticleRender
  {
public:
    ParticleCollection* particles;

    ParticleRender(ParticleCollection* particlesArray): particles(particlesArray){}

    virtual ~ParticleRender();

    virtual void Paint();

  };

}



#endif /* PARTICLERENDER_H_ */
