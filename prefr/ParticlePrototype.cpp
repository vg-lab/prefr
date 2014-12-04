/*
 * ParticlePrototype.cpp
 *
 *  Created on: 03/11/2014
 *      Author: sgalindo
 */

#include "ParticlePrototype.h"

namespace prefr
{
  ParticlePrototype::ParticlePrototype()
  : minLife( 0.0f )
  , maxLife( 0.0f )
  , lifeInterval( 0.0f )
  , lifeNormalization( 1.0f )
  , dispersion( 0.0f )
  , particles( nullptr )
  {
  }

  ParticlePrototype::ParticlePrototype(float min, float max, const ParticleCollection& particlesArray)
  : dispersion( 0.0f )
  , particles( new ParticleCollection( particlesArray ) )
  {
    SetLife(min, max);
  }

  void ParticlePrototype::SetLife(float min, float max)
  {
    minLife = min;
    maxLife = max;
    lifeInterval = maxLife - minLife;
    lifeNormalization = 1.0f/maxLife;
  }
}


