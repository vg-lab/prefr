/*
 * ParticleRenderConfig.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLERENDERCONFIG_H_
#define PARTICLERENDERCONFIG_H_

#include <particles/config.h>
#include "ElementCollection.hpp"

using namespace utils;

namespace particles
{

  class ParticleRenderer
  {
  public:

    ParticleCollection* particles;

    ParticleRenderer(ParticleCollection* particlesArray)
    : particles(particlesArray)
    {}


    virtual ~ParticleRenderer()
    {
      delete( particles );
    }

    virtual void SetupRender(unsigned int aliveParticles = 0) = 0;

    virtual void Paint(unsigned int aliveParticles = 0) = 0;
  };

}



#endif /* PARTICLERENDERCONFIG_H_ */
