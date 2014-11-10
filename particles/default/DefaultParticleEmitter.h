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

    class PointEmissionNode : public EmissionNode
    {
    public:

      vec3 position;

      PointEmissionNode(ParticleCollection* arrayParticles, vec3 _position);
      virtual ~PointEmissionNode();

      virtual vec3 GetEmissionPosition();
      virtual vec3 GetEmissionVelocityDirection();
    };



    //**********************************************************
    // Default Emitter
    //**********************************************************

    class DefaultParticleEmitter : public ParticleEmitter
    {
    private:

      float normalizationFactor;

    public:

      vector<int> emissionNodeParticlesPerCycle;

      DefaultParticleEmitter (ParticleCollection* particlesArray, float _emissionRate, bool _loop);

      ~DefaultParticleEmitter();

      virtual void EmitAll(float deltaTime);

      virtual void StartEmission(float deltaTime);
      virtual int EmitSingle(unsigned int i);

      virtual void UpdateConfiguration();


    protected:

      virtual void EmitFunction(unsigned int i, bool override = false);

    };

    //**********************************************************
    // Point Emitter
    //**********************************************************

//    class PointParticleEmitter : public DefaultParticleEmitter
//    {
//public:
//      vec3 position;
//
//      PointParticleEmitter(ParticleCollection* particlesArray, float _emissionRate, bool _loop, vec3 position);
//
//
//    protected:
//      virtual void EmitFunction(unsigned int i, bool override);
//    };

  }
}

#endif /* DEFAULTPARTICLEEMITTER_H_ */
