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


    DefaultParticleEmitter::DefaultParticleEmitter (ParticleCollection* particlesArray, PrototypesArray* particlePrototype
                                                    , vector<int>* _refPrototypes
                                                    , float _emissionRate, bool _loop)
    : ParticleEmitter(particlesArray, particlePrototype, _refPrototypes, _emissionRate, loop)
    {}

    DefaultParticleEmitter::~DefaultParticleEmitter()
    {
      delete( particles );
      delete( prototypes );
    }


    void DefaultParticleEmitter::EmitAll(float deltaTime)
    {

      this->particlesPerCycle = emissionRate * deltaTime * maxParticles;

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
      if (this->particlesPerCycle && !particles->elements->at(i)->Alive())
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


    PointParticleEmitter::PointParticleEmitter (ParticleCollection* particlesArray, PrototypesArray* particlePrototypes
                                                , vector<int>* _refPrototypes
                                                , float _emissionRate, bool _loop, vec3 position)
    : DefaultParticleEmitter(particlesArray, particlePrototypes, _refPrototypes, _emissionRate, loop)
    , position(position)
    {

    }

    static float invRandMax = 1.0f / RAND_MAX;

    vec3 GetRandomVelocityDirection()
    {
      float theta, phi, vxz;

      theta = clamp(rand()*invRandMax, 0.0f, 1.0f) * 2.0f * pi<float>();//asinf(clamp(rand()*invRandMax, 0.0f, 1.0f));
      phi = clamp(rand()*invRandMax, 0.0f, 1.0f) * 2.0f * pi<float>();
      vxz = sinf(theta);

      return vec3 (cosf(phi)*vxz, cosf(theta), sinf(phi)*vxz);
    }

    void PointParticleEmitter::EmitFunction(unsigned int i, bool override)
    {

        tparticle_ptr current = particles->elements->at(i);
        tprototype_ptr currentPrototype = prototypes->at(refPrototypes->at(i));

        if (currentPrototype && (!current->Alive() || override))
        {
          current->life = clamp(rand() * invRandMax, 0.0f, 1.0f) * currentPrototype->lifeInterval + currentPrototype->minLife;

          current->velocity = GetRandomVelocityDirection();
          current->position = this->position;// + 3.0f * current->velocity;


          current->velocityModule = currentPrototype->velocity.GetValue(0);
          current->color = currentPrototype->color.GetValue(0);
          current->size = currentPrototype->size.GetValue(0);

      }
    }


  }

}



