/*
 * DefaultParticleEmitter.h
 *
 *  Created on: 13/10/2014
 *      Author: sgalindo
 */

#ifndef DEFAULTPARTICLEEMITTER_H_
#define DEFAULTPARTICLEEMITTER_H_

#include <particles/config.h>

#include "../Particle.h"
#include "../ParticleEmitter.h"

namespace particles
{
  namespace defaultParticleSystem
  {



    //**********************************************************
    // Default Emitter
    //**********************************************************

    class DefaultParticleEmitter : public ParticleEmitter
    {
public:


      DefaultParticleEmitter (ParticleCollection* particlesArray, PrototypesArray* particlePrototype, vector<int>* _refPrototypes
                              , float _emissionRate, bool _loop);

      ~DefaultParticleEmitter();

      void EmitAll(float deltaTime);

      int EmitSingle(unsigned int i);

protected:

      virtual void EmitFunction(unsigned int i, bool override = false) = 0;

    };

    //**********************************************************
    // Point Emitter
    //**********************************************************

    class PointParticleEmitter : public DefaultParticleEmitter
    {
public:
      vec3 position;

      PointParticleEmitter(ParticleCollection* particlesArray, PrototypesArray* particlePrototype
                           , vector<int>* _refPrototypes, float _emissionRate, bool _loop, vec3 position);


    protected:
      virtual void EmitFunction(unsigned int i, bool override);
    };

  }
}

#endif /* DEFAULTPARTICLEEMITTER_H_ */
