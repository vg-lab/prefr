/*
 * ParticlePrototype.cpp
 *
 *  Created on: 03/11/2014
 *      Author: sgalindo
 */

#include "Model.h"

namespace prefr
{
  Model::Model()
  : minLife( 0.0f )
  , maxLife( 0.0f )
  , lifeInterval( 0.0f )
  , lifeNormalization( 1.0f )
  , dispersion( 0.0f )
  {
  }

  Model::Model(float min, float max )
  : dispersion( 0.0f )
  {
    SetLife(min, max);
  }

  Model::~Model()
  {

  }

  void Model::SetLife(float min, float max)
  {
    minLife = min;
    maxLife = max;
    lifeInterval = maxLife - minLife;
    lifeNormalization = 1.0f/maxLife;
  }
}


