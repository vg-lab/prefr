/*
 * DefaultParticleRenderConfig.h
 *
 *  Created on: 15/10/2014
 *      Author: sergio
 */

#ifndef OSGDEFAULTPARTICLERENDERCONFIG_H_
#define OSGDEFAULTPARTICLERENDERCONFIG_H_

#include "../ParticleRenderer.h"
#include "OSGRenderConfig.h"
#include <GL/glew.h>

namespace prefr
{

  class OSGDefaultParticleRenderer : public ParticleRenderer
  {
  public:

    int currentAliveParticles;

    OSGDefaultParticleRenderer( const ParticleCollection& particlesArray);

    virtual ~OSGDefaultParticleRenderer();

    void osgCompileGLObjects(osg::RenderInfo& renderInfo) const;

    virtual void SetupRender(unsigned int aliveParticles = 0);

    virtual void Paint(unsigned int aliveParticles = 0) const;

  };


}




#endif /* DEFAULTPARTICLERENDERCONFIG_H_ */
