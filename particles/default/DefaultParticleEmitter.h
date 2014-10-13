/*
 * DefaultParticleEmitter.h
 *
 *  Created on: 13/10/2014
 *      Author: sgalindo
 */

#ifndef DEFAULTPARTICLEEMITTER_H_
#define DEFAULTPARTICLEEMITTER_H_

#include "../config.h"

namespace particles
{
  namespace defaultParticleSystem
  {

    class DefaultParticleEmitter: public ParticleEmitter
    {
      DefaultParticleEmitter (ParticleCollection* particlesArray, vec3 position);


    };

  }
}

#endif /* DEFAULTPARTICLEEMITTER_H_ */
