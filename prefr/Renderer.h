/*
 * ParticleRenderConfig.h
 *
 *  Created on: 10/10/2014
 *      Author: sergio
 */

#ifndef PARTICLERENDERCONFIG_H_
#define PARTICLERENDERCONFIG_H_

// #include <particles/config.h>
#include "types.h"

#include "Particles.h"

#include "DistanceArray.hpp"
#include "RenderConfig.h"

namespace prefr
{

  class Renderer
  {
    friend class ParticleSystem;

  public:

    Renderer( );

    virtual ~Renderer();

    virtual void SetupRender(unsigned int aliveParticles = 0) = 0;

    virtual void Paint(unsigned int aliveParticles = 0) const = 0;

  protected:

    void particles( const ParticleRange& particles );

    ParticleCollection _particles;

    DistanceArray* distances;
    RenderConfig* renderConfig;

  };

}



#endif /* PARTICLERENDERCONFIG_H_ */
