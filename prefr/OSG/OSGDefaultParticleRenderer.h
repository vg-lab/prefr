/*
 * DefaultParticleRenderConfig.h
 *
 *  Created on: 15/10/2014
 *      Author: sergio
 */

#ifndef OSGDEFAULTPARTICLERENDERCONFIG_H_
#define OSGDEFAULTPARTICLERENDERCONFIG_H_

#include <prefr/api.h>
#include "../ParticleRenderer.h"
#include "OSGRenderConfig.h"
#include <GL/glew.h>

namespace prefr
{

  class OSGDefaultParticleRenderer : public ParticleRenderer
  {
  public:

    int currentAliveParticles;

    PREFR_API OSGDefaultParticleRenderer( const ParticleCollection& particlesArray);

    PREFR_API virtual ~OSGDefaultParticleRenderer();

    PREFR_API void osgCompileGLObjects(osg::RenderInfo& renderInfo) const;

    PREFR_API virtual void SetupRender(unsigned int aliveParticles = 0);

    PREFR_API virtual void Paint(unsigned int aliveParticles = 0) const;

  };


}




#endif /* DEFAULTPARTICLERENDERCONFIG_H_ */
