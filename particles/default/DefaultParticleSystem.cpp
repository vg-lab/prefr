/*
 * GLParticleSystem.cpp
 *
 *  Created on: 13/10/2014
 *      Author: sgalindo
 */
#include "DefaultParticleSystem.h"

namespace particles
{
  namespace defaultParticleSystem
  {

    DefaultParticleSystem::DefaultParticleSystem (int initialParticlesNumber, int _maxParticles
                                                      , float _emissionRate, bool _loop)
    : ParticleSystem( initialParticlesNumber, _maxParticles, _emissionRate, _loop )
    , updateLoopUnified( true )
    {}


    void DefaultParticleSystem::Start()
    {
      for (unsigned int i = 0; i < aliveParticles; i++)
      {
        emitters->at(particleEmitter[i])->EmitFunction(i, true);
      }

    }

    void DefaultParticleSystem::Update(float deltaTime)
    {
      if (updateLoopUnified)
        UpdateUnified(deltaTime);
      else
        UpdateSeparated(deltaTime);

    }

    void DefaultParticleSystem::UpdateUnified(float deltaTime)
    {
      unsigned int i = 0;


      // Set emitter delta time to calculate the number of particles to emit this frame
      for (i = 0; i < emitters->size(); i++)
      {
        emitters->at(i)->StartEmission(deltaTime);
      }

      int accumulator = 0;
      for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
      {
        i = ((tparticle_ptr) *it)->id;

        // Emit each particle with its own emitter
        emitters->at(particleEmitter[i])->EmitSingle(i);

        // Update each particle with its own updater
        updaters->at(particleUpdater[i])->Update(i, deltaTime);

        accumulator += (*it)->Alive();
      }

      this->aliveParticles = accumulator;
    }

    void DefaultParticleSystem::UpdateSeparated(float deltaTime)
    {
      for (unsigned int i = 0; i < emitters->size(); i++)
      {
        emitters->at(i)->EmitAll(deltaTime);
      }

      int accumulator = 0;
      for (unsigned int i = 0; i < updaters->size(); i++)
      {
        accumulator += updaters->at(i)->Update(deltaTime);
      }

      this->aliveParticles = accumulator;
    }

  }


}
