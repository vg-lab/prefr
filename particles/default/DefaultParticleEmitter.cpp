/*
 * DefaultParticleEmitter.cpp
 *
 *  Created on: 14/10/2014
 *      Author: sergio
 */
#include "DefaultParticleEmitter.h"

namespace particles
{
  namespace defaultParticleSystem
  {


    //**********************************************************
    // Default Emitter
    //**********************************************************


    DefaultParticleEmitter::DefaultParticleEmitter (ParticleCollection* particlesArray, ParticlePrototype* particlePrototype
                                  , float _emissionRate, bool _loop)
     : ParticleEmitter(particlesArray, particlePrototype, _emissionRate, loop)
    {

      lifeThreshold = particlePrototype->maxLife - particlePrototype->minLife;

    }

    DefaultParticleEmitter::~DefaultParticleEmitter()
    {
      delete( particles );
      delete( particleBase );
    }


    void DefaultParticleEmitter::EmitAll(float deltaTime)
    {

      this->particlesPerCycle = emissionRate * deltaTime;

      for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
      {
        if (particlesPerCycle)
        {
          this->EmitFunction((*it)->id);
          particlesPerCycle--;
        }
        else
        {
          break;
        }
      }

    }

    int DefaultParticleEmitter::EmitSingle(unsigned int i)
    {
      if (this->particlesPerCycle)
      {
        this->EmitFunction(i);
        this->particlesPerCycle--;
      }

      // This might be used as signal to stop looping through this emitter, returning zero after the last particle emitted.
      return particlesPerCycle;
    }



    //**********************************************************
    // Point Emitter
    //**********************************************************


    PointParticleEmitter::PointParticleEmitter (ParticleCollection* particlesArray, ParticlePrototype* particlePrototype
                                      , float _emissionRate, bool _loop, vec3 position)
         : DefaultParticleEmitter(particlesArray, particlePrototype, _emissionRate, loop)
         , position(position)
    {

    }


    vec3 GetRandomVelocityDirection()
    {
      float theta, phi, vxz;

      theta = asinf(clamp(rand()*invRandMax, 0.0f, 1.0f));
      phi = clamp(rand()*invRandMax, 0.0f, 1.0f) * 4.0f * pi<float>();
      vxz = sinf(theta);

      return vec3 (cosf(phi)*vxz, cosf(theta), sinf(phi)*vxz);
    }

    void PointParticleEmitter::EmitFunction(unsigned int i, bool override)
    {

        tparticleptr current = particles->elements->at(i);

        if (!current->Alive() || override)
        {
          current->life = clamp(rand() * invRandMax, 0.0f, 1.0f) * lifeThreshold + particleBase->minLife;

          current->position = this->position;

          current->velocity = GetRandomVelocityDirection();
          current->velocityModule = this->particleBase->velocity.GetValue(0);
          current->color = this->particleBase->color.GetValue(0);
          current->size = this->particleBase->size.GetValue(0);

      }
    }


  }

}



