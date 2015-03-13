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
    , lastParticleNodeID( -1 )
    , lastParticlePrototypeID( -1 )
    , currentNodeID( -1 )
    , currentPrototypeID( -1 )
    , currentNode( nullptr )
    , currentPrototype( nullptr )
    {}

    ParticleUpdater::~ParticleUpdater()
    {
      delete( particles );
      delete( prototypes );
    }

    EmissionNode* ParticleUpdater::GetCurrentNode( const int& particleID )
    {
      if (particleID == lastParticleNodeID)
        return currentNode;

      int nodeID = (*refEmissionNodes)[ particleID ];

      if (nodeID < 0)
        currentNode = nullptr;
      else if (nodeID != currentNodeID)
        currentNode = (*emissionNodes)[ nodeID ];

      currentNodeID = nodeID;
      lastParticleNodeID = particleID;

      return currentNode;
    }


    tprototype_ptr ParticleUpdater::GetCurrentPrototype( const int& particleID )
    {
      if (particleID == lastParticlePrototypeID)
        return currentPrototype;

      int prototypeID = (*refPrototypes)[particleID];

      if (prototypeID < 0)
        currentPrototype = nullptr;
      else if (prototypeID != currentPrototypeID)
        currentPrototype = (*prototypes)[ prototypeID ];

      currentPrototypeID = prototypeID;
      lastParticlePrototypeID = particleID;

      return currentPrototype;
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
      currentNode = GetCurrentNode( current->id );
      currentPrototype = GetCurrentPrototype( current->id );

      if (!currentNode || !currentPrototype)
        return;

      current->life = std::max(0.0f, current->life - deltaTime);
      current->alive = (current->life > 0) &&
          (!(currentNode->killParticlesIfInactive && !currentNode->active));

      if (current->Alive() && currentPrototype && !current->Newborn())
      {

        float refLife = 1.0f - glm::clamp((current->life) * (currentPrototype->lifeNormalization), 0.0f, 1.0f);

        current->color = (currentPrototype->color.GetValue(refLife));

        current->size = currentPrototype->size.GetValue(refLife);
        current->velocityModule = currentPrototype->velocity.GetValue(refLife);

        current->position += current->velocity * current->velocityModule * deltaTime;

      }

      current->newborn = false;

    }

}
