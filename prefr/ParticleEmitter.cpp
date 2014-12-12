/*
 * DefaultParticleEmitter.cpp
 *
 *  Created on: 14/10/2014
 *      Author: sergio
 */
#include "ParticleEmitter.h"

namespace prefr
{

    static float invRandMax = 1.0f / RAND_MAX;

    //**********************************************************
    // Default Emitter
    //**********************************************************


    ParticleEmitter::ParticleEmitter ( const ParticleCollection& particlesArray,
                                      float _emissionRate, bool _loop)
    : particles( new ParticleCollection( particlesArray ) )
    , emissionNodes( nullptr )
    , refEmissionNodes( nullptr )
    , prototypes( nullptr )
    , refPrototypes( nullptr )
    , particlesBudget( 0 )
    , emissionRate( _emissionRate )
    , loop( _loop )
    , active( true )
    , lastParticleNodeID( -1 )
    , lastParticlePrototypeID( -1 )
    , currentNodeID( -1 )
    , currentPrototypeID( -1 )
    , currentNode( nullptr )
    , currentPrototype( nullptr )
    {
      maxParticles = particles->size;

      normalizationFactor = 1.0f/particles->size;
    }

    ParticleEmitter::~ParticleEmitter()
    {
      delete( particles );
      delete( prototypes );
    }

    EmissionNode* ParticleEmitter::GetCurrentNode( const int& particleID )
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


    tprototype_ptr ParticleEmitter::GetCurrentPrototype( const int& particleID )
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

    void ParticleEmitter::EmitAll(float deltaTime)
    {

      if (!active)
        return;

      StartEmission(deltaTime);

//      int* nodeParticlesPerCycle;
//      int previousNodeID;
//      int emissionNodeID;
      tparticle_ptr current;
      for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
      {
        current = (*it);

        currentNode = GetCurrentNode(current->id);

        if (!currentNode)
          continue;

        if (currentNode->particlesBudget && !current->Alive() && currentNode->active)
        {
          this->EmitFunction(current);
          currentNode->particlesBudget--;
        }

//        previousNodeID = emissionNodeID;
      }

    }

    void ParticleEmitter::StartEmission(float deltaTime)
    {
      particlesBudget = emissionRate * maxParticles * deltaTime;

      EmissionNode* node;
      for (EmissionNodesArray::iterator it = emissionNodes->begin();
           it != emissionNodes->end(); it++)
      {
        node = *it;
        node->emissionAcc +=
            particlesBudget * (node->particles->size * normalizationFactor);

        node->particlesBudget = int(floor(node->emissionAcc));
        node->emissionAcc -= node->particlesBudget;
//        std::cout << particlesPerCycle << " " << emissionNodes->at(i)->particles->size << " "  << emissionNodeParticlesPerCycle[i] << std::endl;
      }
    }

    int ParticleEmitter::EmitSingle(const tparticle_ptr current)
    {
      if (!active)
        return 0;

      currentNode = GetCurrentNode(current->id);

      if (!currentNode)
        return 0;

      if (currentNode->particlesBudget && !current->Alive() && currentNode->active)
      {
        this->EmitFunction(current);
        currentNode->particlesBudget--;
      }

      // This might be used as signal to stop looping through this emitter, returning zero after the last particle emitted.
      return currentNode->particlesBudget;
    }

    void ParticleEmitter::EmitFunction(const tparticle_ptr current, bool override)
    {
      currentNode = GetCurrentNode(current->id);
      currentPrototype = GetCurrentPrototype(current->id);

      if (!currentNode || !currentPrototype)
        return;

      if ((!current->Alive() || override))
      {
       current->life = glm::clamp(rand() * invRandMax, 0.0f, 1.0f) *
           currentPrototype->lifeInterval + currentPrototype->minLife;

       current->velocity = currentNode->GetEmissionVelocityDirection();
       current->position = currentNode->GetEmissionPosition();

       current->velocityModule = currentPrototype->velocity.GetFirstValue();
       current->color = currentPrototype->color.GetFirstValue();
       current->size = currentPrototype->size.GetFirstValue();

       current->newborn = true;

     }
    }


}



