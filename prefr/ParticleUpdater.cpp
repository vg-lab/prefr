/*
 * DefaultParticleUpdater.cpp
 *
 *  Created on: 14/10/2014
 *      Author: sergio
 */

#include "ParticleUpdater.h"

namespace prefr
{

    ParticleUpdater::ParticleUpdater(ParticleCollection* particlesArray)
    : particles( particlesArray )
    , prototypes( nullptr )
    , refPrototypes( nullptr )
    {}

    ParticleUpdater::~ParticleUpdater()
    {
      delete( particles );
      delete( prototypes );
    }

    int ParticleUpdater::Update(float deltaTime)
    {
      int aliveParticles = 0;
      for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
      {
        Update((*it)->id, deltaTime);

        aliveParticles += (*it)->Alive();
      }

      return aliveParticles;
    }

    void ParticleUpdater::Update(unsigned int i, float deltaTime)
    {
      tparticle_ptr current = particles->elements->at(i);
      tprototype_ptr currentPrototype = prototypes->at(refPrototypes->at(i));
      float refLife;


      current->life = std::max(0.0f, current->life - deltaTime);
      current->alive = current->life > 0;

      if (current->Alive() && currentPrototype)
      {

        refLife = 1.0f - glm::clamp((current->life) / (currentPrototype->maxLife), 0.0f, 1.0f);

        current->color = /*HSVToRGB*/(currentPrototype->color.GetValue(refLife));

        current->size = currentPrototype->size.GetValue(refLife);
        current->velocityModule = currentPrototype->velocity.GetValue(refLife);

        current->position += current->velocity * current->velocityModule * deltaTime;

      }
    }

}
