/*
 * GLParticleSystem.cpp
 *
 *  Created on: 13/10/2014
 *      Author: sgalindo
 */
#include "DefaultParticleSystem.h"

namespace defaultParticleSystem
{

  DefaultGLParticleSystem::DefaultGLParticleSystem (int initialParticlesNumber, int _maxParticles, float _emissionRate
                                                   , bool _loop)
  : ParticleSystem( initialParticlesNumber, _maxParticles, _emissionRate, _loop )
  , vao( -1 )
  , vboBillboardVertex( -1 )
  , vboParticlesPositions( -1 )
  , vboParticlesColor( -1 )
  {
    distances = new distanceArray(this->maxParticles);

    billboardVertices = new glvectorf( 4 );
    particlePositions = new glvectorf( this->maxParticles * 4 );
    particleColors = new glvectorch( this->maxParticles * 4 );

  }


  void DefaultGLParticleSystem::UpdateUnified(float deltaTime)
  {

  }
  void DefaultGLParticleSystem::UpdateSeparated(float deltaTime)
  {

  }

  void DefaultGLParticleSystem::Render()
  {

  }



}




