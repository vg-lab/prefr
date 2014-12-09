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

    glm::vec3 GetRandomDirection()
    {
      float theta, phi, vxz;

      theta = glm::clamp(rand()*invRandMax, 0.0f, 1.0f) * 2.0f * M_PI;//asinf(clamp(rand()*invRandMax, 0.0f, 1.0f));
      phi = glm::clamp(rand()*invRandMax, 0.0f, 1.0f) * 2.0f * M_PI;
      vxz = sinf(theta);

      return glm::vec3 (cosf(phi)*vxz, cosf(theta), sinf(phi)*vxz);
    }


    PointEmissionNode::PointEmissionNode( const ParticleCollection& arrayParticles, glm::vec3 _position)
    : EmissionNode( arrayParticles )
    ,position( _position )
    {}

    PointEmissionNode::~PointEmissionNode()
    {
      delete ( particles );
    }

    glm::vec3 PointEmissionNode::GetEmissionPosition()
    {
      return position;
    }

    glm::vec3 PointEmissionNode::GetEmissionVelocityDirection()
    {
      return GetRandomDirection();
    }



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
    , particlesPerCycle( 0 )
    , emissionRate( _emissionRate )
    , loop( _loop )
    , active( true )
    {
      maxParticles = particles->size;

      normalizationFactor = 1.0f/particles->size;

      UpdateConfiguration();
    }

    ParticleEmitter::~ParticleEmitter()
    {
      delete( particles );
      delete( prototypes );
    }

    void ParticleEmitter::UpdateConfiguration()
    {
      if (emissionNodes)
        emissionNodeParticlesPerCycle.resize(emissionNodes->size());
    }

    void ParticleEmitter::EmitAll(float deltaTime)
    {

      if (!active)
        return;

      this->particlesPerCycle = emissionRate * deltaTime * maxParticles;

      for (unsigned int i = 0; i < emissionNodes->size(); i ++)
      {
        emissionNodeParticlesPerCycle[i] =
            std::max(1, int (particlesPerCycle * ((*emissionNodes)[i]->particles->size * normalizationFactor)));
      }

      int* nodeParticlesPerCycle;
      int emissionNodeID;
      tparticle_ptr current;
      for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
      {
        current = (*it);

        emissionNodeID = (*refEmissionNodes)[ current->id ];

        if (!(*emissionNodes)[emissionNodeID]->active)
          continue;

        nodeParticlesPerCycle = &emissionNodeParticlesPerCycle[emissionNodeID];
        if (*nodeParticlesPerCycle && !current->Alive())
        {
          this->EmitFunction(current);
          (*nodeParticlesPerCycle)--;
        }

      }

    }

    void ParticleEmitter::StartEmission(float deltaTime)
    {
      particlesPerCycle = emissionRate * maxParticles * deltaTime;

      for (unsigned int i = 0; i < emissionNodes->size(); i ++)
      {

        emissionNodeParticlesPerCycle[i] =
            std::max(1, int (particlesPerCycle * ((*emissionNodes)[i]->particles->size * normalizationFactor)));
//        std::cout << particlesPerCycle << " " << emissionNodes->at(i)->particles->size << " "  << emissionNodeParticlesPerCycle[i] << std::endl;
      }
    }

    int ParticleEmitter::EmitSingle(const tparticle_ptr current)
    {
      if (!active)
        return 0;

      int nodeID = (*refEmissionNodes)[ current->id ];
      int* nodeParticlesPerCycle = &emissionNodeParticlesPerCycle[nodeID];
      if (*nodeParticlesPerCycle && (*emissionNodes)[nodeID]->active && !current->Alive())
      {
        this->EmitFunction(current);
        (*nodeParticlesPerCycle)--;
      }

      // This might be used as signal to stop looping through this emitter, returning zero after the last particle emitted.
      return (*nodeParticlesPerCycle);
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



