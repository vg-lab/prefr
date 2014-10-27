/*
 * DefaultParticleEmitter.h
 *
 *  Created on: 13/10/2014
 *      Author: sgalindo
 */

#ifndef DEFAULTPARTICLEEMITTER_H_
#define DEFAULTPARTICLEEMITTER_H_

#include <particles/config.h>

#include "../ParticleEmitter.h"

namespace particles
{
  namespace defaultParticleSystem
  {

    static float invRandMax = 1.0f / RAND_MAX;

    //**********************************************************
    // Default Emitter
    //**********************************************************

    class DefaultParticleEmitter : public ParticleEmitter
    {
public:


      DefaultParticleEmitter (ParticleCollection* particlesArray, ParticlePrototype* particlePrototype
                              , float _emissionRate, bool _loop);

      ~DefaultParticleEmitter();

      void EmitAll(float deltaTime);

      int EmitSingle(unsigned int i);

protected:

      virtual void EmitFunction(unsigned int i, bool override = false) = 0;


      float lifeThreshold;

    };

    //**********************************************************
    // Point Emitter
    //**********************************************************

    class PointParticleEmitter : public DefaultParticleEmitter
    {
public:
      vec3 position;

      PointParticleEmitter(ParticleCollection* particlesArray, ParticlePrototype* particlePrototype
                                   , float _emissionRate, bool _loop, vec3 position);


    protected:
      virtual void EmitFunction(unsigned int i, bool override);
    };

  }
}

#endif /* DEFAULTPARTICLEEMITTER_H_ */
