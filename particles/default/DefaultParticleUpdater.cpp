/*
 * DefaultParticleUpdater.cpp
 *
 *  Created on: 14/10/2014
 *      Author: sergio
 */

#include "DefaultParticleUpdater.h"

namespace particles
{
  namespace defaultParticleSystem
  {

    DefaultParticleUpdater::DefaultParticleUpdater(ParticleCollection* particlesArray, PrototypesArray* particlePrototypes, vector<int>* _refPrototypes)
    : ParticleUpdater(particlesArray, particlePrototypes, _refPrototypes)
    {}

    DefaultParticleUpdater::~DefaultParticleUpdater()
    {
      delete( particles );
      delete( prototypes );
    }

    int DefaultParticleUpdater::Update(float deltaTime)
    {
      int aliveParticles = 0;
      for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
      {
        Update((*it)->id, deltaTime);

        aliveParticles += (*it)->Alive();
      }

      return aliveParticles;
    }

    void DefaultParticleUpdater::Update(unsigned int i, float deltaTime)
    {
      tparticle_ptr current = particles->elements->at(i);
      tprototype_ptr currentPrototype = prototypes->at(refPrototypes->at(i));
      float refLife;


      current->life = fmax(0, current->life - deltaTime);
      current->alive = current->life > 0;

      if (current->Alive() && currentPrototype)
      {

        refLife = 1.0f - clamp((current->life) / (currentPrototype->maxLife), 0.0f, 1.0f);

        current->color = /*HSVToRGB*/(currentPrototype->color.GetValue(refLife));

        current->size = currentPrototype->size.GetValue(refLife);
        current->velocityModule = currentPrototype->velocity.GetValue(refLife);

        current->position += current->velocity * current->velocityModule * deltaTime;

      }
    }
  }

}
