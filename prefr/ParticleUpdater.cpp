/*
 * DefaultParticleUpdater.cpp
 *
 *  Created on: 14/10/2014
 *      Author: sergio
 */

#include "ParticleUpdater.h"

namespace prefr
{

    ParticleUpdater::ParticleUpdater(const ParticleCollection& particlesArray)
    : particles( new ParticleCollection( particlesArray ) )
    , prototypes( nullptr )
    , refPrototypes( nullptr )
    , emissionNodes( nullptr )
    , refEmissionNodes( nullptr )
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
        Update((*it), deltaTime);

        aliveParticles += (*it)->Alive();
      }

      return aliveParticles;
    }

    void ParticleUpdater::Update(const tparticle_ptr current, float deltaTime)
    {
      tprototype_ptr currentPrototype = (*prototypes)[(*refPrototypes)[current->id]];
      float refLife;

      current->life = std::max(0.0f, current->life - deltaTime);
      current->alive = current->life > 0;

      if (current->Alive() && currentPrototype && !current->Newborn())
      {

        refLife = 1.0f - glm::clamp((current->life) * (currentPrototype->lifeNormalization), 0.0f, 1.0f);

        current->color = (currentPrototype->color.GetValue(refLife));

        current->size = currentPrototype->size.GetValue(refLife);
        current->velocityModule = currentPrototype->velocity.GetValue(refLife);

        current->position += current->velocity * current->velocityModule * deltaTime;

      }

      current->newborn = false;

    }

}
