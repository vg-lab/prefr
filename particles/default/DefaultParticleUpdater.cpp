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

    DefaultParticleUpdater::DefaultParticleUpdater(ParticleCollection* particlesArray, ParticlePrototype* particlePrototype, distanceArray* distancesArray)
    : ParticleUpdater(particlesArray, particlePrototype)
    , distances(distancesArray)
    {}

    DefaultParticleUpdater::~DefaultParticleUpdater()
    {
      delete( particles );
      delete( particleBase );
      delete( distances );
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
      tparticleptr current = particles->elements->at(i);
      float refLife;


      current->life = fmax(0, current->life - deltaTime);
      current->alive = current->life > 0;

      if (current->Alive())
      {

        refLife = 1.0f - clamp((current->life) / (particleBase->maxLife), 0.0f, 1.0f);
//        std::cout << refLife << std::endl;
        current->color = /*HSVToRGB*/(particleBase->color.GetValue(refLife));
//        std::cout << current->color.x << " " << current->color.y << " " << current->color.z<< " " << current->color.w << std::endl;
        current->size = particleBase->size.GetValue(refLife);
        current->velocityModule = particleBase->velocity.GetValue(refLife);
//        std::cout << refLife << " : " << current->velocityModule << std::endl;
        current->position += current->velocity * current->velocityModule * deltaTime;

//        std::cout << current->position.x << " " << current->position.y << " " << current->position.z << std::endl;
      }
    }
  }

}
