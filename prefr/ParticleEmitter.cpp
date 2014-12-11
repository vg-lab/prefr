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
    {
      maxParticles = particles->size;

      normalizationFactor = 1.0f/particles->size;
    }

    ParticleEmitter::~ParticleEmitter()
    {
      delete( particles );
      delete( prototypes );
    }

    void ParticleEmitter::EmitAll(float deltaTime)
    {

      if (!active)
        return;

      StartEmission(deltaTime);

//      int* nodeParticlesPerCycle;
//      int emissionNodeID;
      EmissionNode* node;
      tparticle_ptr current;
      for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
      {
        current = (*it);

        node = (*emissionNodes)[ (*refEmissionNodes)[ current->id ] ];

        if (!node->active)
          continue;

        if (node->particlesBudget && !current->Alive())
        {
          this->EmitFunction(current);
          node->particlesBudget--;
        }

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

      EmissionNode* node = (*emissionNodes)[ (*refEmissionNodes)[ current->id ] ];
      if (node->particlesBudget && !current->Alive() && node->active)
      {
        this->EmitFunction(current);
        node->particlesBudget--;
      }

      // This might be used as signal to stop looping through this emitter, returning zero after the last particle emitted.
      return node->particlesBudget;
    }

    void ParticleEmitter::EmitFunction(const tparticle_ptr current, bool override)
    {
       tprototype_ptr currentPrototype = (*prototypes)[ (*refPrototypes)[ current->id ] ];
       EmissionNode* node = (*emissionNodes)[ (*refEmissionNodes)[ current->id ] ];

       if (currentPrototype && (!current->Alive() || override))
       {
         current->life = glm::clamp(rand() * invRandMax, 0.0f, 1.0f) *
             currentPrototype->lifeInterval + currentPrototype->minLife;

         current->velocity = node->GetEmissionVelocityDirection();
         current->position = node->GetEmissionPosition();

         current->velocityModule = currentPrototype->velocity.GetFirstValue();
         current->color = currentPrototype->color.GetFirstValue();
         current->size = currentPrototype->size.GetFirstValue();

         current->newborn = true;

     }
    }


}



