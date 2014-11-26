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
#include "ElementCollection.hpp"
#include "DistanceArray.hpp"
#include "RenderConfig.h"

using namespace utils;

namespace prefr
{

  class ParticleRenderer
  {
  public:

    ParticleCollection* particles;

    DistanceArray* distances;
    RenderConfig* renderConfig;

    ParticleRenderer(ParticleCollection* particlesArray)
    : particles(particlesArray)
    , distances( nullptr )
    , renderConfig( nullptr )
    {}


    virtual ~ParticleRenderer()
    {
      delete( particles );

      if (renderConfig)
        delete( renderConfig );
    }

    virtual void SetupRender(unsigned int aliveParticles = 0) = 0;

    virtual void Paint(unsigned int aliveParticles = 0) const = 0;
  };

}



#endif /* PARTICLERENDERCONFIG_H_ */
