/*
 * DefaultParticleRenderConfig.h
 *
 *  Created on: 15/10/2014
 *      Author: sergio
 */

#ifndef DEFAULTPARTICLERENDERCONFIG_H_
#define DEFAULTPARTICLERENDERCONFIG_H_

#include <particles/config.h>
#include "GLTypes.h"
#include "../../ParticleRenderer.h"
#include "../DefaultParticleSystem.h"


namespace particles
{
  namespace defaultParticleSystem
  {
    namespace GL
    {
      class GLDefaultParticleRenderer : public ParticleRenderer
      {
      public:
        distanceArray* distances;

        RenderConfig* renderConfig;

        GLDefaultParticleRenderer(ParticleCollection* particlesArray, distanceArray* distancesArray
                                    , RenderConfig* renderConfiguration);

        virtual ~GLDefaultParticleRenderer();

        virtual void SetupRender(unsigned int aliveParticles = 0);

        virtual void Paint(unsigned int aliveParticles = 0) const;

      };
    }
  }

}




#endif /* DEFAULTPARTICLERENDERCONFIG_H_ */
