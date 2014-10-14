/*
 * DefaultParticleRenderConfig.h
 *
 *  Created on: 15/10/2014
 *      Author: sergio
 */

#ifndef DEFAULTPARTICLERENDERCONFIG_H_
#define DEFAULTPARTICLERENDERCONFIG_H_

#include "../config.h"
#include "DefaultParticleSystem.h"
#include "DefaultParticleRenderConfig.h"

namespace particles
{
  namespace defaultParticleSystem
  {

    class DefaultParticleRenderConfig : public ParticleRenderConfig
    {

      distanceArray* distances;

      RenderConfig* renderConfig;

      DefaultParticleRenderConfig(ParticleCollection* particlesArray, distanceArray* distancesArray
                                  , RenderConfig* renderConfiguration);

      virtual ~DefaultParticleRenderConfig();

      virtual void SetupRender(unsigned int aliveParticles = 0);

    };

  }

}




#endif /* DEFAULTPARTICLERENDERCONFIG_H_ */
