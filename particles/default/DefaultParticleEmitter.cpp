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

    static float invRandMax = 1.0f / RAND_MAX;

    glm::vec3 GetRandomDirection()
    {
      float theta, phi, vxz;

      theta = glm::clamp(rand()*invRandMax, 0.0f, 1.0f) * 2.0f * M_PI;//asinf(clamp(rand()*invRandMax, 0.0f, 1.0f));
      phi = glm::clamp(rand()*invRandMax, 0.0f, 1.0f) * 2.0f * M_PI;
      vxz = sinf(theta);

      return glm::vec3 (cosf(phi)*vxz, cosf(theta), sinf(phi)*vxz);
    }


    PointEmissionNode::PointEmissionNode(ParticleCollection* arrayParticles, glm::vec3 _position)
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


    DefaultParticleEmitter::DefaultParticleEmitter (ParticleCollection* particlesArray
                                                    , float _emissionRate, bool _loop)
    : ParticleEmitter(particlesArray, _emissionRate, loop)
    {
      normalizationFactor = 1.0f/particles->size;

      UpdateConfiguration();
    }

    DefaultParticleEmitter::~DefaultParticleEmitter()
    {
      delete( particles );
      delete( prototypes );
    }

    void DefaultParticleEmitter::UpdateConfiguration()
    {
      if (emissionNodes)
        emissionNodeParticlesPerCycle.resize(emissionNodes->size());
    }

    void DefaultParticleEmitter::EmitAll(float deltaTime)
    {

      this->particlesPerCycle = emissionRate * deltaTime * maxParticles;

      for (unsigned int i = 0; i < emissionNodes->size(); i ++)
      {
        emissionNodeParticlesPerCycle[i] = particlesPerCycle * normalizationFactor;
        std::cout << emissionNodeParticlesPerCycle[i] << std::endl;
      }

      int* nodeParticlesPerCycle;
      for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
      {
        nodeParticlesPerCycle = &emissionNodeParticlesPerCycle[refEmissionNodes->at( (*it)->id )];
        if (*nodeParticlesPerCycle)
        {
          this->EmitFunction((*it)->id);
          (*nodeParticlesPerCycle)--;
        }
        else
        {
          break;
        }
      }

    }

    void DefaultParticleEmitter::StartEmission(float deltaTime)
    {
      particlesPerCycle = emissionRate * maxParticles * deltaTime;

      for (unsigned int i = 0; i < emissionNodes->size(); i ++)
      {

        emissionNodeParticlesPerCycle[i] = std::max(1.0f, particlesPerCycle * emissionNodes->at(i)->particles->size * normalizationFactor);
//        std::cout << particlesPerCycle << " " << emissionNodes->at(i)->particles->size << " "  << emissionNodeParticlesPerCycle[i] << std::endl;
      }
    }

    int DefaultParticleEmitter::EmitSingle(unsigned int i)
    {
      int* nodeParticlesPerCycle = &emissionNodeParticlesPerCycle[refEmissionNodes->at( i )];
      if (*nodeParticlesPerCycle && !particles->elements->at( i )->Alive())
      {
        this->EmitFunction(i);
        (*nodeParticlesPerCycle)--;
      }

      // This might be used as signal to stop looping through this emitter, returning zero after the last particle emitted.
      return (*nodeParticlesPerCycle);
    }

    void DefaultParticleEmitter::EmitFunction(unsigned int i, bool override)
       {

           tparticle_ptr current = particles->elements->at(i);
           tprototype_ptr currentPrototype = prototypes->at(refPrototypes->at(i));

           if (currentPrototype && (!current->Alive() || override))
           {
             current->life = glm::clamp(rand() * invRandMax, 0.0f, 1.0f) * currentPrototype->lifeInterval + currentPrototype->minLife;

             current->velocity = emissionNodes->at(refEmissionNodes->at(i))->GetEmissionVelocityDirection();
             current->position = emissionNodes->at(refEmissionNodes->at(i))->GetEmissionPosition();
   //          current->position = this->position;// + 3.0f * current->velocity;


             current->velocityModule = currentPrototype->velocity.GetFirstValue();
             current->color = currentPrototype->color.GetFirstValue();
             current->size = currentPrototype->size.GetFirstValue();

         }
       }



    //**********************************************************
    // Point Emitter
    //**********************************************************


//    PointParticleEmitter::PointParticleEmitter (ParticleCollection* particlesArray
//                                                , float _emissionRate, bool _loop, vec3 position)
//    : DefaultParticleEmitter(particlesArray, _emissionRate, loop)
//    , position(position)
//    {
//
//    }



//    void PointParticleEmitter::EmitFunction(unsigned int i, bool override)
//    {
//
//        tparticle_ptr current = particles->elements->at(i);
//        tprototype_ptr currentPrototype = prototypes->at(refPrototypes->at(i));
//
//        if (currentPrototype && (!current->Alive() || override))
//        {
//          current->life = clamp(rand() * invRandMax, 0.0f, 1.0f) * currentPrototype->lifeInterval + currentPrototype->minLife;
//
//          current->velocity = GetRandomVelocityDirection();
//          current->position = emissionNodes->at(refEmissionNodes->at(i))->GetEmissionPosition();
////          current->position = this->position;// + 3.0f * current->velocity;
//
//
//          current->velocityModule = currentPrototype->velocity.GetValue(0);
//          current->color = currentPrototype->color.GetValue(0);
//          current->size = currentPrototype->size.GetValue(0);
//
//      }
//    }


  }

}



