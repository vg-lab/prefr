/*
 * Renderer.cpp
 *
 *  Created on: 25 de abr. de 2016
 *      Author: sgalindo
 */

#include "Renderer.h"

namespace prefr
{

  Renderer::Renderer( )
  : _particles( nullptr )
  , distances( nullptr )
  , renderConfig( nullptr )
  {}

  Renderer::~Renderer()
  {
    if (renderConfig)
      delete( renderConfig );
  }

  void Renderer::particles( const ParticleRange& particles_ )
  {
    _particles = particles_;
  }

}


