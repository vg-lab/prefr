/*
 * DefaultParticleRenderConfig.h
 *
 *  Created on: 15/10/2014
 *      Author: sergio
 */

#ifndef OSGDEFAULTPARTICLERENDERCONFIG_H_
#define OSGDEFAULTPARTICLERENDERCONFIG_H_

#include <prefr/api.h>
#include "../Renderer.h"
#include "OSGRenderConfig.h"
#ifndef PREFR_SKIP_GLEW_INCLUDE
#include <GL/glew.h>
#endif

#ifdef PREFR_USE_OPENSCENEGRAPH

namespace prefr
{

  class OSGRenderer : public Renderer
  {
  public:

    int currentAliveParticles;

    PREFR_API OSGRenderer( );

    PREFR_API virtual ~OSGRenderer();

    PREFR_API void osgCompileGLObjects(osg::RenderInfo& renderInfo) const;

    PREFR_API virtual void SetupRender(unsigned int aliveParticles = 0);

    PREFR_API virtual void Paint(unsigned int aliveParticles = 0) const;

  };


}

#endif


#endif /* DEFAULTPARTICLERENDERCONFIG_H_ */
