/*
 * DefaultParticleRenderConfig.h
 *
 *  Created on: 15/10/2014
 *      Author: sergio
 */

#ifndef DEFAULTPARTICLERENDERCONFIG_H_
#define DEFAULTPARTICLERENDERCONFIG_H_

#include "../config.h"
#include "../ParticleRenderer.h"
#include "DefaultParticleSystem.h"


namespace particles
{
  namespace defaultParticleSystem
  {

    class DefaultParticleRenderer : public ParticleRenderer
    {
    public:
      distanceArray* distances;

      RenderConfig* renderConfig;

      DefaultParticleRenderer(ParticleCollection* particlesArray, distanceArray* distancesArray
                                  , RenderConfig* renderConfiguration);

      virtual ~DefaultParticleRenderer();

      virtual void SetupRender(unsigned int aliveParticles = 0);

      virtual void Paint(unsigned int aliveParticles = 0);

    };

  }

}




#endif /* DEFAULTPARTICLERENDERCONFIG_H_ */
