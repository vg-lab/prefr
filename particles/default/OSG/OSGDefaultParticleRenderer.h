/*
 * DefaultParticleRenderConfig.h
 *
 *  Created on: 15/10/2014
 *      Author: sergio
 */

#ifndef OSGDEFAULTPARTICLERENDERCONFIG_H_
#define OSGDEFAULTPARTICLERENDERCONFIG_H_

#include <particles/config.h>
#include "OSGTypes.h"
#include "../../ParticleRenderer.h"



namespace particles
{
  namespace defaultParticleSystem
  {
    namespace OSGParticleSystem
    {
      class OSGDefaultParticleRenderer : public ParticleRenderer
      {
      public:
        distanceArray* distances;

        RenderConfig* renderConfig;

        int currentAliveParticles;

        OSGDefaultParticleRenderer(ParticleCollection* particlesArray, 
				   distanceArray* distancesArray,
				   RenderConfig* renderConfiguration);

        virtual ~OSGDefaultParticleRenderer();

        void osgCompileGLObjects(osg::RenderInfo& renderInfo) const;

        virtual void SetupRender(unsigned int aliveParticles = 0);

        virtual void Paint(unsigned int aliveParticles = 0) const;

      };
    }
  }

}




#endif /* DEFAULTPARTICLERENDERCONFIG_H_ */
