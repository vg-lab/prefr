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

    DefaultGLParticleSystem::DefaultGLParticleSystem (int initialParticlesNumber, int _maxParticles
                                                      , float _emissionRate, bool _loop)
    : ParticleSystem( initialParticlesNumber, _maxParticles, _emissionRate, _loop )
    , vao( -1 )
    , vboBillboardVertex( -1 )
    , vboParticlesPositions( -1 )
    , vboParticlesColor( -1 )
    , updateLoopUnified( true )
    {
      distances = new distanceArray(this->maxParticles);

      billboardVertices = new glvectorf( 4 );
      particlePositions = new glvectorf( this->maxParticles * 4 );
      particleColors = new glvectorch( this->maxParticles * 4 );

    }

    void DefaultGLParticleSystem::Update(float deltaTime)
    {

      if (updateLoopUnified)
        UpdateUnified(deltaTime);
      else
        UpdateSeparated(deltaTime);

    }

    void DefaultGLParticleSystem::UpdateUnified(float deltaTime)
    {
      int i = 0;
      for (tparticleContainer::iterator it = particles->start; it != particles->end; it++)
      {

        // Emit each particle with its own emitter
        emitters->at(particleEmitter[i])->Emit(i, deltaTime);

        // Update each particle with its own updater
        updaters->at(particleUpdater[i])->Update(i, deltaTime);

        i++;
      }
    }

    void DefaultGLParticleSystem::UpdateSeparated(float deltaTime)
    {
      for (int i = 0; i < emitters->size(); i++)
      {
        emitters->at(i)->Update(deltaTime);
      }

      for (int i = 0; i < updaters->size(); i++)
      {
        updaters->at(i)->Update(deltaTime);
      }

    }


    void DefaultGLParticleSystem::UpdateRender()
    {
      this->sorter->Sort();

      this->renderConfigurer->SetupRender();
    }

    void DefaultGLParticleSystem::Render()
    {
      this->render->Paint();
    }

  }

}
