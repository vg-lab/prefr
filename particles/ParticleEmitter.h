/*
 * ParticleEmitter.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLEEMITTER_H_
#define PARTICLEEMITTER_H_

#include "config.h"
#include "InterpolationSet.hpp"
#include "ElementCollection.hpp"

using namespace utils;

namespace particles
{
  typedef InterpolationSet<float> vectortfloat;
  typedef InterpolationSet<vec3> vectortvec3;
  typedef InterpolationSet<vec4> vectortvec4;

  class ParticlePrototype
  {
  public:
    float minLife = 0;
    float maxLife;
    float dispersion;

    vectortfloat size;

    vec3 positionOffset;

    vectortfloat velocity;
    vectortvec4 color;

  };


  class ParticleEmitter
  {
  public:

    ParticleCollection* particles;

    ParticlePrototype* particleBase;

    int maxParticles;
    int particlesPerCycle;
    float emissionRate;
    bool loop;

    ParticleEmitter(ParticleCollection* particlesArray, ParticlePrototype* particlePrototype, float _emissionRate, bool _loop)
    : particles(particlesArray)
    , particleBase(particlePrototype)
    , particlesPerCycle(0)
    , emissionRate(_emissionRate)
    , loop (_loop)
    {
      maxParticles = particlesArray->size;
    }

    virtual ~ParticleEmitter()
    {
      delete( particles );
      delete( particleBase );

    }



    virtual void StartEmission(float deltaTime){ particlesPerCycle = emissionRate * maxParticles;}
    virtual int EmitSingle(unsigned int i) = 0;

    virtual void EmitAll(float deltaTime) = 0;

    virtual void EmitFunction(unsigned int i, bool override = false) = 0;

  };


}



#endif /* PARTICLEEMITTER_H_ */
